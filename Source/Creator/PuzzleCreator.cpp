#include "PuzzleCreator.h"



PuzzleCreator::PuzzleCreator(int spalten, int zeilen) : m_spalten(spalten), m_zeilen(zeilen)
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

		/*random_shuffle(begin(sides), end(sides));

		cout << bitset<8>(newPart).to_string() << endl;

		for (int i = 0; i < 4; i++)
		{
		cout << sides[i] << endl;
		}*/

		partCreated = true;
	}

	return newPart;
}

/* Only look on the upper and left part, because the other are not set yet */
bool PuzzleCreator::PuzzleLogicForNewPuzzle(vector <Part> part_array, uint8_t newPart, int partIndex)
{
	uint8_t newPartNoseTop = newPart & 0b11000000;
	uint8_t newPartNoseLeft = newPart & 0b00000011;
	uint8_t leftPartNoseRight = 0b00000000;
	uint8_t upperPartNoseBottom = 0b00000000;

	if ((partIndex - 1 >= 0) && ((partIndex % m_spalten) != 0))
	{
		leftPartNoseRight = part_array[partIndex - 1].getConnections() & 0b00110000;
	}
	else
	{
		// Set the first puzzle part in the puzzle without checking
		return true;
	}

	if (partIndex - m_spalten > 0)
	{
		upperPartNoseBottom = part_array[partIndex - m_spalten].getConnections() & 0b00001100;
	}
	else
	{
		// Set puzzle parts in the first row without checking
		return true;
	}

	/*
	0 1 2 3 4
	5 6 7 8 9
	*/
	// 0b000001000 & 0b00001000 == 0b00001000
	// 0b000001000 | 0b00000100 == 0b00001100
	// 0b000001000 & 0b01000000 == 0b00000000
	// Shift to get top nose of the new part compared to the bottom nose of the upper part
	if (((upperPartNoseBottom | (newPartNoseTop >> 4)) == 0b00001100) && ((leftPartNoseRight | (newPartNoseLeft << 4)) == 0b00110000))
	//if (((upperPartNoseBottom & (newPartNoseTop >> 4)) == 0b00000000) && ((leftPartNoseRight & (newPartNoseLeft << 4)) == 0b00000000))
	{
		// new part fits in the hole
		return true;
	}

	// new part does not fit in the hole --> try other (new) part
	return false;
}

vector <Part> PuzzleCreator::CreateRandomPuzzle(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array)
{
	int partIndex = 0;
	int countCorner = 0;
	int countEdge = 0;
	int countInners = 0;

	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			uint8_t newPart = 0b00000000;
			/*
			3 Möglichkeiten: 00 - 01 - 10

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
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				corners_array[countCorner++] = &part_array[partIndex];
			}
			else if ((zeile == 0) || (zeile == m_zeilen - 1) || (spalte == 0) || (spalte == m_spalten - 1))
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(1);
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				edges_array[countEdge++] = &part_array[partIndex];
			}
			else
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(0);
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				inners_array[countInners++] = &part_array[partIndex];
			}

			partIndex++;
		}
	}

	return part_array;
}

vector <Part> PuzzleCreator::CreateRandomPuzzle2(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array)
{
	int partIndex = 0;
	int countCorner = 0;
	int countEdge = 0;
	int countInners = 0;

	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			uint8_t newPart = 0b00000000;

			if (((zeile == 0) && (spalte == 0)) || ((zeile == m_zeilen - 1) && (spalte == 0)) || ((zeile == 0) && (spalte == m_spalten - 1)) || ((zeile == m_zeilen - 1) && (spalte == m_spalten - 1)))
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
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				corners_array[countCorner++] = &part_array[partIndex];
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
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				edges_array[countEdge++] = &part_array[partIndex];
			}
			else
			{
				bool partSet = false;
				while (!partSet)
				{
					newPart = CreateRandomPart(0);
					partSet = PuzzleLogicForNewPuzzle(part_array, newPart, partIndex);
				}
				part_array[partIndex].setConnections(newPart);
				inners_array[countInners++] = &part_array[partIndex];
			}

			partIndex++;
		}
	}

	return part_array;
}

vector <Part> PuzzleCreator::CreatePuzzle(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array)
{
	int partIndex = 0;
	int countCorner = 0;
	int countEdge = 0;
	int countInners = 0;
	for (int zeile = 0; zeile < m_zeilen; zeile++)
	{
		for (int spalte = 0; spalte < m_spalten; spalte++)
		{
			if (countInners == 884)
			{
				cout << endl;
			}
			if ((zeile == 0) || (zeile == m_zeilen))
			{
				if (spalte == 0)
				{
					part_array[partIndex].setConnections(0b00100100);
					corners_array[countCorner++] = &part_array[partIndex];
				}
				else if (spalte == m_spalten)
				{
					part_array[partIndex].setConnections(0b00000101);
					corners_array[countCorner++] = &part_array[partIndex];
				}
				else
				{
					if (zeile == 0)
					{
						if ((spalte % 2) == 0)
						{
							part_array[partIndex].setConnections(0b00100110);
							edges_array[countEdge++] = &part_array[partIndex];
						}
						else
						{
							part_array[partIndex].setConnections(0b00010101);
							edges_array[countEdge++] = &part_array[partIndex];
						}
					}
					else
					{
						if ((spalte % 2) == 0)
						{
							part_array[partIndex].setConnections(0b10010001);
							edges_array[countEdge++] = &part_array[partIndex];
						}
						else
						{
							part_array[partIndex].setConnections(0b10100010);
							edges_array[countEdge++] = &part_array[partIndex];
						}
					}
				}
			}
			else if (spalte == 0)
			{
				if ((zeile % 2) == 0)
				{
					part_array[partIndex].setConnections(0b01100100);
					edges_array[countEdge++] = &part_array[partIndex];
				}
				else
				{
					part_array[partIndex].setConnections(0b10011000);
					edges_array[countEdge++] = &part_array[partIndex];
				}
			}
			else if (spalte == m_spalten)
			{
				if ((zeile % 2) == 0)
				{
					part_array[partIndex].setConnections(0b10000101);
					edges_array[countEdge++] = &part_array[partIndex];
				}
				else
				{
					part_array[partIndex].setConnections(0b10000110);
					edges_array[countEdge++] = &part_array[partIndex];
				}
			}
			else
			{
				if ((zeile % 2) == 0)
				{
					if ((spalte % 2) == 0)
					{
						part_array[partIndex].setConnections(0b01100110);
						inners_array[countInners++] = &part_array[partIndex];
					}
					else
					{
						part_array[partIndex].setConnections(0b10010101);
						inners_array[countInners++] = &part_array[partIndex];
					}
				}
				else
				{
					if ((spalte % 2) == 0)
					{
						part_array[partIndex].setConnections(0b10011001);
						inners_array[countInners++] = &part_array[partIndex];
					}
					else
					{
						part_array[partIndex].setConnections(0b10100110);
						inners_array[countInners++] = &part_array[partIndex];
					}
				}
			}
			partIndex++;
		}
	}
	return part_array;
}
