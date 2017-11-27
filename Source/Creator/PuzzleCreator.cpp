#include "PuzzleCreator.h"
#include "../Helper/HelperFunctions.h"

PuzzleCreator::PuzzleCreator(int spalten, int zeilen, vector <Part>* part_array) : m_spalten(spalten), m_zeilen(zeilen), m_part_array(part_array)
{
}


PuzzleCreator::~PuzzleCreator()
{
}

uint8_t PuzzleCreator::CreateNose()
{
	int isNose = rand() % 2;

	if (isNose)
	{
		return 0b00000001;
	}
	else
	{
		return 0b00000010;
	}
}

uint8_t PuzzleCreator::CreateRandomPart(int wishedEdges)
{
	uint8_t newPart = 0b00000000;
	bool partCreated = false;
	while (!partCreated)
	{
		int sides[4] = { 0, 2, 4, 6 };
		random_shuffle(begin(sides), end(sides));

		if ((wishedEdges == 2) && (((sides[0] - sides[1]) == 4) || ((sides[0] - sides[1]) == -4)))
		{
			continue;
		}

		for (int numEdges = 0; numEdges < wishedEdges; numEdges++)
		{
			newPart |= 0b00000000 << sides[numEdges];
		}

		for (int numNoses = wishedEdges; numNoses < 4; numNoses++)
		{
			newPart |= CreateNose() << sides[numNoses];
		}

		partCreated = true;
	}

	return newPart;
}

/* Only look on the upper and left part, because the other are not set yet */
bool PuzzleCreator::PuzzleLogicForNewPuzzle(uint8_t newPart, int partIndex)
{
	uint8_t newPartNoseTop = newPart & 0b11000000;
	uint8_t newPartNoseLeft = newPart & 0b00000011;

	uint8_t leftPartNoseRight = 0b00000000;
	uint8_t upperPartNoseBottom = 0b00000000;

	bool topSuitable = false;
	bool leftSuitable = false;

	if (partIndex == 0)
	{
		// Set the first puzzle part in the puzzle without checking
		return true;
	}
	else if((partIndex % m_spalten) == 0)
	{
		// Puzzle parts on the first collumn are fitting left allways
		leftSuitable = true;
	}
	else
	{
		leftPartNoseRight = m_part_array->at(partIndex - 1).getConnections() & 0b00110000;
	}

	if ((partIndex - m_spalten) > 0)
	{
		upperPartNoseBottom = m_part_array->at(partIndex - m_spalten).getConnections() & 0b00001100;
	}
	else
	{
		// Puzzle parts in the first row are fitting top allways
		topSuitable = true;
	}

	uint8_t resTop = upperPartNoseBottom ^ (HelperFunctions::ContinuousShift(newPartNoseTop, 4));
	uint8_t resLeft = leftPartNoseRight ^ (HelperFunctions::ContinuousShift(newPartNoseLeft, 4));

	if((resTop == 0b00001100) && (topSuitable == false))
	{
		topSuitable = true;
	}

	if((resLeft == 0b00110000) && (leftSuitable == false))
	{
		leftSuitable = true;
	}

	if((topSuitable == true) && (leftSuitable == true))
	{
		// new part fits in the hole
		return true;
	}

	// new part does not fit in the hole --> try other (new) part
	return false;
}

vector <Part>* PuzzleCreator::CreateRandomPuzzle()
{
	int partIndex = 0;

	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			uint8_t newPart = 0b00000000;
			/*
			3 Moeglichkeiten: 00 - 01 - 10

			Ecke:	min. 6 nuller 00100100 - nuller nebeneinander != 00010010
			Kante:	min. 5 nuller 00100110
			Innen:	min. 4 nuller 10011001
			*/

			if (((zeile == 0) && (spalte == 0)) || ((zeile == m_zeilen - 1) && (spalte == 0)) || ((zeile == 0) && (spalte == m_spalten - 1)) || ((zeile == m_zeilen - 1) && (spalte == m_spalten - 1)))
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(2);
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}
			else if ((zeile == 0) || (zeile == m_zeilen - 1) || (spalte == 0) || (spalte == m_spalten - 1))
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(1);
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}
			else
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(0);
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}

			partIndex++;
		}
	}

	return m_part_array;
}

vector <Part>* PuzzleCreator::CreateRandomPuzzle2()
{
	int partIndex = 0;

	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			uint8_t newPart = 0b00000000;

			if (((zeile == 0) && (spalte == 0))
				|| ((zeile == m_zeilen - 1) && (spalte == 0))
				|| ((zeile == 0) && (spalte == m_spalten - 1))
				|| ((zeile == m_zeilen - 1) && (spalte == m_spalten - 1)))
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(2);
					if (((zeile == 0)			&& (spalte == 0)			&& ((newPart & 0b11000011) != 0b00000000)) ||
						((zeile == 0)			&& (spalte == m_spalten-1)	&& ((newPart & 0b11110000) != 0b00000000)) ||
						((zeile == m_zeilen-1)	&& (spalte == 0)			&& ((newPart & 0b00001111) != 0b00000000)) ||
						((zeile == m_zeilen-1)	&& (spalte == m_spalten-1)	&& ((newPart & 0b00111100) != 0b00000000)))
					{
						continue;
					}
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}
			else if ((zeile == 0) || (zeile == m_zeilen - 1) || (spalte == 0) || (spalte == m_spalten - 1))
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(1);
					if (((zeile == 0)				&& ((newPart & 0b11000000) != 0b00000000)) ||
						((zeile == m_zeilen-1)		&& ((newPart & 0b00001100) != 0b00000000)) ||
						((spalte == 0)				&& ((newPart & 0b00000011) != 0b00000000)) ||
						((spalte == m_spalten-1)	&& ((newPart & 0b00110000) != 0b00000000)))
					{
						continue;
					}
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}
			else
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(0);
					partSet = PuzzleLogicForNewPuzzle(newPart, partIndex);
				}
				m_part_array->at(partIndex).setConnections(newPart);
			}

			partIndex++;
		}
	}

	RotateWholePuzzle();
	ShuffleWholePuzzle();

	return m_part_array;
}

bool PuzzleCreator::CreateRandomPuzzle3()
{
    int partIndex = 0;

    for (int row = 0; row < m_zeilen; row++)
    {
        for (int collumn = 0; collumn < m_spalten; collumn++)
        {
            uint8_t newPartConnections = 0b00000000;

            if (((row == 0) && (collumn == 0)) || ((row == m_zeilen - 1) && (collumn == 0)) || ((row == 0) && (collumn == m_spalten - 1)) || ((row == m_zeilen - 1) && (collumn == m_spalten - 1)))
            {
                bool partSet = false;
                while (!partSet)
                {
                    newPartConnections = CreateRandomPart(2);
                    if (((row == 0)			&& (collumn == 0)			&& ((newPartConnections & 0b11000011) != 0b00000000)) ||
                        ((row == 0)			&& (collumn == m_spalten-1)	&& ((newPartConnections & 0b11110000) != 0b00000000)) ||
                        ((row == m_zeilen-1)	&& (collumn == 0)			&& ((newPartConnections & 0b00001111) != 0b00000000)) ||
                        ((row == m_zeilen-1)	&& (collumn == m_spalten-1)	&& ((newPartConnections & 0b00111100) != 0b00000000)))
                    {
                        continue;
                    }
                    partSet = PuzzleLogicForNewPuzzle(newPartConnections, partIndex);
                }

				m_part_array->at(partIndex) = newPartConnections;
            }
            else if ((row == 0) || (row == m_zeilen - 1) || (collumn == 0) || (collumn == m_spalten - 1))
            {
                bool partSet = false;
                while (!partSet)
                {
                    newPartConnections = CreateRandomPart(1);
                    if (((row == 0)					&& ((newPartConnections & 0b11000000) != 0b00000000)) ||
                        ((row == m_zeilen-1)		&& ((newPartConnections & 0b00001100) != 0b00000000)) ||
                        ((collumn == 0)				&& ((newPartConnections & 0b00000011) != 0b00000000)) ||
                        ((collumn == m_spalten-1)	&& ((newPartConnections & 0b00110000) != 0b00000000)))
                    {
                        continue;
                    }
                    partSet = PuzzleLogicForNewPuzzle(newPartConnections, partIndex);
                }

				m_part_array->at(partIndex) = newPartConnections;
            }
            else
            {
                bool partSet = false;
                while (!partSet)
                {
                    newPartConnections = CreateRandomPart(0);
                    partSet = PuzzleLogicForNewPuzzle(newPartConnections, partIndex);
                }

				m_part_array->at(partIndex) = newPartConnections;
            }

            partIndex++;
        }
    }

    RotateWholePuzzle();
    ShuffleWholePuzzle();

    return true;
}

vector <Part>* PuzzleCreator::CreatePuzzle()
{
	int partIndex = 0;

	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			if ((zeile == 0) || (zeile == m_zeilen))
			{
				if (spalte == 0)
				{
					m_part_array->at(partIndex).setConnections(0b00100100);
				}
				else if (spalte == m_spalten)
				{
					m_part_array->at(partIndex).setConnections(0b00000101);
				}
				else
				{
					if (zeile == 0)
					{
						if ((spalte % 2) == 0)
						{
							m_part_array->at(partIndex).setConnections(0b00100110);
						}
						else
						{
							m_part_array->at(partIndex).setConnections(0b00010101);
						}
					}
					else
					{
						if ((spalte % 2) == 0)
						{
							m_part_array->at(partIndex).setConnections(0b10010001);
						}
						else
						{
							m_part_array->at(partIndex).setConnections(0b10100010);
						}
					}
				}
			}
			else if (spalte == 0)
			{
				if ((zeile % 2) == 0)
				{
					m_part_array->at(partIndex).setConnections(0b01100100);
				}
				else
				{
					m_part_array->at(partIndex).setConnections(0b10011000);
				}
			}
			else if (spalte == m_spalten)
			{
				if ((zeile % 2) == 0)
				{
					m_part_array->at(partIndex).setConnections(0b10000101);
				}
				else
				{
					m_part_array->at(partIndex).setConnections(0b10000110);
				}
			}
			else
			{
				if ((zeile % 2) == 0)
				{
					if ((spalte % 2) == 0)
					{
						m_part_array->at(partIndex).setConnections(0b01100110);
					}
					else
					{
						m_part_array->at(partIndex).setConnections(0b10010101);
					}
				}
				else
				{
					if ((spalte % 2) == 0)
					{
						m_part_array->at(partIndex).setConnections(0b10011001);
					}
					else
					{
						m_part_array->at(partIndex).setConnections(0b10100110);
					}
				}
			}
			partIndex++;
		}
	}
	return m_part_array;
}

void PuzzleCreator::RotateWholePuzzle()
{
	for(int index = 0; index < m_part_array->size(); index++)
	{
		int randomNum = 2 * (rand() % 4);
		m_part_array->at(index).setConnections(HelperFunctions::ContinuousShift(m_part_array->at(index).getConnections(), randomNum));
	}
}

void PuzzleCreator::ShuffleWholePuzzle()
{
	random_shuffle(begin(*m_part_array), end(*m_part_array));
}

