#include "PuzzleSolverGrp3.h"
#include "../Helper/HelperFunctions.h"
#include <bitset>
#include <map>

PuzzleSolverGrp3::PuzzleSolverGrp3(vector <Part>* part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array)
	: m_part_array(part_array), m_corners_array(corners_array), m_edges_array(edges_array), m_inners_array(inners_array)
{
	int numSpalten = 0;
	int numZeilen = 0;
	for (int kanteHtemp = 1; kanteHtemp <= (m_edges_array.size() / 2) - 1; kanteHtemp++)
	{
		int kanteVtemp = (m_edges_array.size() / 2) - kanteHtemp;

		if ((kanteHtemp * kanteVtemp) == m_inners_array.size()) // 884
		{
			if (kanteHtemp >= kanteVtemp)
			{
                numSpalten = kanteHtemp + 2;
                numZeilen = kanteVtemp + 2;
			}
			/*cout << "Zeilen: " << kanteVtemp + 2 << endl;
			cout << "Spalten: " << kanteHtemp + 2 << endl;
			cout << "Anzahl Kanten: " << kanteVtemp * 2 + kanteHtemp * 2 << endl; // 36*28 = 884
			cout << "Matrixgroesse = " << (kanteVtemp + 2)*(kanteHtemp + 2) << endl;*/
		}
	}

	m_solutionVector = vector<vector<SolutionElement>>(numSpalten, vector<SolutionElement>(numZeilen));
}


PuzzleSolverGrp3::~PuzzleSolverGrp3()
{
}

void PuzzleSolverGrp3::ClearPuzzle()
{
    int numSpalten = 0;
    int numZeilen = 0;
    for (int kanteHtemp = 1; kanteHtemp <= (m_edges_array.size() / 2) - 1; kanteHtemp++)
    {
        int kanteVtemp = (m_edges_array.size() / 2) - kanteHtemp;

        if ((kanteHtemp * kanteVtemp) == m_inners_array.size()) // 884
        {
            if (kanteHtemp >= kanteVtemp)
            {
                numSpalten = kanteHtemp + 2;
                numZeilen = kanteVtemp + 2;
            }
            /*cout << "Zeilen: " << kanteVtemp + 2 << endl;
            cout << "Spalten: " << kanteHtemp + 2 << endl;
            cout << "Anzahl Kanten: " << kanteVtemp * 2 + kanteHtemp * 2 << endl; // 36*28 = 884
            cout << "Matrixgroesse = " << (kanteVtemp + 2)*(kanteHtemp + 2) << endl;*/
        }
    }

    m_solutionVector.clear();
    m_solutionVector = vector<vector<SolutionElement>>(numSpalten, vector<SolutionElement>(numZeilen));
}

bool PuzzleSolverGrp3::SolvePuzzle()
{
	/*cout << "Groesse:" << m_solutionVector.size() << endl;
	cout << "Groesse:" << m_solutionVector[1].size() << endl;
	cout << "Groesse:" << m_solutionVector[1][0].index << endl;

    for(int index = 0; index < m_corners_array.size(); index++)
    {
        cout << "test " << bitset<8>(m_corners_array[index]->getConnections()).to_string() << endl;
    }
    */

    /*for(int collumn = 0; collumn < m_solutionVector.size(), collumn++)
    {
        for(int row = 0; row < m_solutionVector[0].size(), row++)
        {
            if()
        }
    }*/

    for(int index = 0; index < m_part_array->size(); index++)
    {
        bool partSet = false;
        int randomCollumn = 0;
        int randomRow = 0;
        uint8_t numRotations = 0;
        uint32_t numTries = 0;
        multimap<int,int> combinationsMap;
        combinationsMap.insert(pair<int, int>(m_solutionVector.size(), m_solutionVector[0].size()));

        while(!partSet)
        {
            bool newCombination = false;
            while(!newCombination)
            {
                if(combinationsMap.size() > (m_solutionVector.size() * m_solutionVector[0].size()))
                {
                    numTries = (m_solutionVector.size() * m_solutionVector[0].size());
                    break;
                }

                //srand (time(NULL));
                randomCollumn = rand() % m_solutionVector.size();
                randomRow = rand() % m_solutionVector[0].size();

                /*bool included = false;
                for(multimap<int,int>::iterator it = combinationsMap.begin(); it != combinationsMap.end(); it++)
                {
                    if((it->first == randomCollumn) && (it->second == randomRow))
                    {
                        included = true;
                    }
                }
                if(included != true)
                {
                    combinationsMap.insert(pair<int, int>(randomCollumn, randomRow));
                    newCombination = true;
                }*/

                bool included = false;
                multimap<int,int>::iterator search = combinationsMap.find(randomCollumn);
                while(search != combinationsMap.end())
                {
                    if((search->first == randomCollumn) && (search->second == randomRow))
                    {
                        included = true;
                        break;
                    }
                    search++;
                }

                if(included == false)
                {
                    combinationsMap.insert(pair<int, int>(randomCollumn, randomRow));
                    newCombination = true;
                }
            }
            numRotations = 0;

            if(numTries >= (m_solutionVector.size() * m_solutionVector[0].size()))
            {
                cout << "numTries: " << numTries << " Index: " << index << endl;
                return false;
            }
            if(numTries >= 900 - 1)//(m_solutionVector.size() * m_solutionVector[0].size() * 100))
            {
                if(index == 0)
                {
                    cout << "test";
                }
                /*for (uint32_t spalte = 0; spalte < m_solutionVector.size(); spalte++)
                {
                    for (uint32_t zeile = 0; zeile < m_solutionVector[0].size(); zeile++)
                    {
                        cout << m_solutionVector[spalte][zeile].index << "||" ;
                    }
                    cout << endl;
                }*/
            }

            if(m_solutionVector[randomCollumn][randomRow].index != 65535)
            {
                continue;
            }
            else
            {
                numTries++;

                while((!partSet) && (numRotations < 4))
                {
                    uint8_t partOrientation = m_part_array->at(index).getConnections();
                    partOrientation = HelperFunctions::ContinuousShift(partOrientation, (numRotations * 2));
                    partSet = PuzzleLogic_v2(partOrientation, randomCollumn, randomRow);
                    if(!partSet)
                    {
                        numRotations++;
                    }
                }
            }
        }

        m_solutionVector[randomCollumn][randomRow].orientation = numRotations;
        m_solutionVector[randomCollumn][randomRow].index = index;
    }

    return true;
}

bool PuzzleSolverGrp3::SolvePuzzle2()
{
    bool puzzleSolved = false;
    //puzzleSolved = SolveBorder();

    // Zuerst erste Ecke
    for(int cornerIndex = 0; (cornerIndex < m_corners_array.size() && puzzleSolved == false); cornerIndex++)
    {
        bool firstPartSet = false;
        int numRotations = 0;
        int partIndex = 0;
        uint8_t partOrientation = 0;

        // Setzen des ersten Bausteins --> Muss eine Ecke sein
        while((!firstPartSet) && (numRotations < 4))
        {
            partIndex = GetIndexFromPartArray(m_corners_array, cornerIndex);
            partOrientation = m_part_array->at(partIndex).getConnections();
            partOrientation = HelperFunctions::ContinuousShift(partOrientation, (numRotations * 2));
            firstPartSet = PuzzleLogic_v2(partOrientation, 0, 0);
            if(!firstPartSet)
            {
                numRotations++;
            }
        }

        //Vector mit möglichen Andockstellen erzeugen
        vector<pair<int,int>> nextPuzzlePlaces;
        nextPuzzlePlaces.push_back(make_pair(1, 0));
        nextPuzzlePlaces.push_back(make_pair(0, 1));

        vector<pair<int,int>> tempPuzzle;
        tempPuzzle.push_back(make_pair(partIndex, partOrientation));

        puzzleSolved = RecursiveFindAndPlace(nextPuzzlePlaces, tempPuzzle);
        //Rekursive Logik zum Erkennen von möglichen Anbausteinen und testen des Steins mit den geringsten Möglichkeiten

    }

    // Dann immer dort wo am meisten Möglichkeiten
}

bool PuzzleSolverGrp3::SolveBorder()
{

}

int PuzzleSolverGrp3::GetIndexFromPartArray(vector <Part *> wishedArray, int wishedIndex)
{
    int partIndex = 0;
    for(int index = 0; index < m_part_array->size(); index++)
    {
        if(&m_part_array->at(index) == wishedArray[wishedIndex])
        {
            partIndex = index;
            break;
        }
    }
    return partIndex;
}

bool PuzzleSolverGrp3::RecursiveFindAndPlace(vector<pair<int,int>> nextPuzzlePlaces, vector<pair<int,int>>& tempPuzzle)
{
    vector<pair<int,int>> tempPuzzleIntern = tempPuzzle;
    bool puzzleSolved = false;

    for(int possiblePlace = 0; possiblePlace < nextPuzzlePlaces.size(); possiblePlace++)
    {
        int spalte = nextPuzzlePlaces[possiblePlace].first;
        int zeile = nextPuzzlePlaces[possiblePlace].second;

        // Ein passendes Teil setzen && Checken ob das Teil eh noch nicht gesetzt ist
        if ((spalte == 0) || (spalte == m_solutionVector.size()) || (zeile == 0) || (zeile == m_solutionVector[0].size()))
        {
            if((spalte == zeile) || ((spalte == 0) && (zeile == m_solutionVector[0].size())) || ((spalte == m_solutionVector.size()) && (zeile == 0)))
            {
                // Corner
            }
            else
            {
                // Edge
            }
        }
        else
        {
            // Inner
        }

        // Neue nextPuzzlePlaces berechnen

        // Ausgewähltes Teil in den tempPuzzleIntern Vector speichern
        //tempPuzzleIntern.push_back(make_pair(partIndex, partOrientation));

        // Eine Ebene tiefer in der rekursiven Funktion
        //puzzleSolved = RecursiveFindAndPlace(nextPuzzlePlaces, tempPuzzleIntern);

        // Ausgewähltes Teil in den tempPuzzle Vector speichern
        if(puzzleSolved == true)
        {
            //tempPuzzle.push_back(make_pair(partIndex, partOrientation));
        }
    }
}

bool PuzzleSolverGrp3::PuzzleLogic(uint8_t currentPart, int collumn, int row)
{
    uint8_t currentPartNoseTop = currentPart & 0b11000000;
    uint8_t currentPartNoseRight = currentPart & 0b00110000;
    uint8_t currentPartNoseBottom = currentPart & 0b00001100;
    uint8_t currentPartNoseLeft = currentPart & 0b00000011;

    uint8_t upperPartNoseBottom = 0b00000000;
    uint8_t rightPartNoseLeft = 0b00000000;
    uint8_t lowerPartNoseTop = 0b00000000;
    uint8_t leftPartNoseRight = 0b00000000;

    bool edge = false;
    bool corner = false;

    if (collumn == 0)
    {
        if (row == 0)
        {
            corner = true;
            upperPartNoseBottom = upperPartNoseBottom | 0b00001100;
            rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
            lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
            leftPartNoseRight = leftPartNoseRight | 0b00110000;
        }
        else if(row == (m_solutionVector[0].size() - 1))
        {
            corner = true;
            upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
            rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
            lowerPartNoseTop = lowerPartNoseTop | 0b11000000;
            leftPartNoseRight = leftPartNoseRight | 0b00110000;
        }
        else
        {
            edge = true;
            upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
            rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
            lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
            leftPartNoseRight = leftPartNoseRight | 0b00110000;
        }
    }
    else if (collumn == (m_solutionVector.size() - 1))
    {
        if (row == 0)
        {
            corner = true;
            upperPartNoseBottom = upperPartNoseBottom | 0b00001100;
            rightPartNoseLeft = rightPartNoseLeft | 0b00000011;
            lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
            leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
        }
        else if(row == (m_solutionVector[0].size() - 1))
        {
            corner = true;
            upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
            rightPartNoseLeft = rightPartNoseLeft | 0b00000011;
            lowerPartNoseTop = lowerPartNoseTop | 0b11000000;
            leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
        }
        else
        {
            edge = true;
            upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
            rightPartNoseLeft = rightPartNoseLeft | 0b00000011;
            lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
            leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
        }
    }
    else if(row == 0)
    {
        edge = true;
        upperPartNoseBottom = upperPartNoseBottom | 0b00001100;
        rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
        lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
        leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
    }
    else if(row == (m_solutionVector[0].size() - 1))
    {
        edge = true;
        upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
        rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
        lowerPartNoseTop = lowerPartNoseTop | 0b11000000;
        leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
    }
    else
    {
        upperPartNoseBottom = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row-1].index, 2, currentPartNoseTop);
        rightPartNoseLeft = GetOrientationForSolutionMatrix(m_solutionVector[collumn+1][row].index, 3, currentPartNoseRight);
        lowerPartNoseTop = GetOrientationForSolutionMatrix(m_solutionVector[collumn][row+1].index, 0, currentPartNoseBottom);
        leftPartNoseRight = GetOrientationForSolutionMatrix(m_solutionVector[collumn-1][row].index, 1, currentPartNoseLeft);
    }

    uint8_t resTop = upperPartNoseBottom ^ (HelperFunctions::ContinuousShift(currentPartNoseTop, 4));
    uint8_t resRight = rightPartNoseLeft ^ (HelperFunctions::ContinuousShift(currentPartNoseRight, 4));
    uint8_t resBottom = lowerPartNoseTop ^ (HelperFunctions::ContinuousShift(currentPartNoseBottom, 4));
    uint8_t resLeft = leftPartNoseRight ^ (HelperFunctions::ContinuousShift(currentPartNoseLeft, 4));
    uint8_t resCompl = resTop | resRight | resBottom | resLeft;
    bitset<8> resBitSet (resCompl);

    /*
    0 1 2 3 4
    5 6 7 8 9
    */
    // 0b000001000 & 0b00001000 == 0b00001000
    // 0b000001000 | 0b00000100 == 0b00001100
    // 0b000001000 & 0b01000000 == 0b00000000
    // Shift to get top nose of the new part compared to the bottom nose of the upper part
    //if ((((upperPartNoseBottom ^ (currentPartNoseTop >> 4)) == 0b00001100) || (((upperPartNoseBottom ^ (currentPartNoseTop >> 4)) == (currentPartNoseTop >> 4)) && (edge == true)))
    //    && (((rightPartNoseLeft ^ (currentPartNoseRight >> 4)) == 0b00000011)  || (((upperPartNoseBottom ^ (currentPartNoseTop >> 4)) == (currentPartNoseTop >> 4)) && (edge == true)))
    //    && (((lowerPartNoseTop ^ (currentPartNoseBottom << 4)) == 0b11000000)  || (((upperPartNoseBottom ^ (currentPartNoseTop >> 4)) == (currentPartNoseTop >> 4)) && (edge == true)))
    //    && (((leftPartNoseRight ^ (currentPartNoseLeft << 4)) == 0b00110000)  || (((upperPartNoseBottom ^ (currentPartNoseTop >> 4)) == (currentPartNoseTop >> 4)) && (edge == true))))
        //if (((upperPartNoseBottom & (newPartNoseTop >> 4)) == 0b00000000) && ((leftPartNoseRight & (newPartNoseLeft << 4)) == 0b00000000))
    if(((resTop == 0b00001100) && (resRight == 0b00000011) && (resBottom == 0b11000000) && (resLeft == 0b00110000)) // part fits in the hole
        || ((resCompl ^ HelperFunctions::ContinuousShift(currentPart, 4)) == 0b00000000) // no other parts around
        || ((resBitSet.count() == 6) && (corner == true))
        || ((resBitSet.count() == 5) && (edge == true)))
    {
        // new part fits in the hole
        return true;
    }

    // new part does not fit in the hole --> try other (new) part
    return false;
}

bool PuzzleSolverGrp3::PuzzleLogic_v2(uint8_t currentPart, int collumn, int row)
{
    uint8_t currentPartNoseTop = currentPart & 0b11000000;
    uint8_t currentPartNoseRight = currentPart & 0b00110000;
    uint8_t currentPartNoseBottom = currentPart & 0b00001100;
    uint8_t currentPartNoseLeft = currentPart & 0b00000011;

    uint8_t upperPartNoseBottom = 0b00000000;
    uint8_t rightPartNoseLeft = 0b00000000;
    uint8_t lowerPartNoseTop = 0b00000000;
    uint8_t leftPartNoseRight = 0b00000000;

    bool topSuitable = false;
    bool rightSuitable = false;
    bool bottomSuitable = false;
    bool leftSuitable = false;

    if(((row - 1) < 0) && ((collumn + 1) > (m_solutionVector.size() - 1)) || ((row + 1) > (m_solutionVector[0].size() - 1)) && ((collumn - 1) < 0))
    {
        //cout << "test";
    }

    if((row - 1) < 0)
    {
        if(currentPartNoseTop == 0b00000000)
        {
            topSuitable = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(m_solutionVector[collumn][row-1].index == 65535)
        {
            if(currentPartNoseTop != 0b00000000)
            {
                topSuitable = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            upperPartNoseBottom = HelperFunctions::ContinuousShift(m_part_array->at(m_solutionVector[collumn][row-1].index).getConnections(), (m_solutionVector[collumn][row-1].orientation * 2));
            upperPartNoseBottom = upperPartNoseBottom & HelperFunctions::ContinuousShift(0b11000000, -(2 * 2));
        }
    }

    if((collumn + 1) > (m_solutionVector.size() - 1))
    {
        if(currentPartNoseRight == 0b00000000)
        {
            rightSuitable = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(m_solutionVector[collumn+1][row].index == 65535)
        {
            if(currentPartNoseRight != 0b00000000)
            {
                rightSuitable = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            rightPartNoseLeft = HelperFunctions::ContinuousShift(m_part_array->at(m_solutionVector[collumn+1][row].index).getConnections(), (m_solutionVector[collumn+1][row].orientation * 2));
            rightPartNoseLeft = rightPartNoseLeft & HelperFunctions::ContinuousShift(0b11000000, -(3 * 2));
        }
    }

    if((row + 1) > (m_solutionVector[0].size() - 1))
    {
        if(currentPartNoseBottom == 0b00000000)
        {
            bottomSuitable = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(m_solutionVector[collumn][row+1].index == 65535)
        {
            if(currentPartNoseBottom != 0b00000000)
            {
                bottomSuitable = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            lowerPartNoseTop = HelperFunctions::ContinuousShift(m_part_array->at(m_solutionVector[collumn][row+1].index).getConnections(), (m_solutionVector[collumn][row+1].orientation * 2));
            lowerPartNoseTop = lowerPartNoseTop & HelperFunctions::ContinuousShift(0b11000000, -(0 * 2));
        }
    }

    if((collumn - 1) < 0)
    {
        if(currentPartNoseLeft == 0b00000000)
        {
            leftSuitable = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(m_solutionVector[collumn-1][row].index == 65535)
        {
            if(currentPartNoseLeft != 0b00000000)
            {
                leftSuitable = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            leftPartNoseRight = HelperFunctions::ContinuousShift(m_part_array->at(m_solutionVector[collumn-1][row].index).getConnections(), (m_solutionVector[collumn-1][row].orientation * 2));
            leftPartNoseRight = leftPartNoseRight & HelperFunctions::ContinuousShift(0b11000000, -(1 * 2));
        }
    }

    uint8_t resTop = upperPartNoseBottom ^ (HelperFunctions::ContinuousShift(currentPartNoseTop, 4));
    uint8_t resRight = rightPartNoseLeft ^ (HelperFunctions::ContinuousShift(currentPartNoseRight, 4));
    uint8_t resBottom = lowerPartNoseTop ^ (HelperFunctions::ContinuousShift(currentPartNoseBottom, 4));
    uint8_t resLeft = leftPartNoseRight ^ (HelperFunctions::ContinuousShift(currentPartNoseLeft, 4));
    uint8_t resCompl = resTop | resRight | resBottom | resLeft;
    bitset<8> resBitSet (resCompl);

    if((resTop == 0b00001100) && (topSuitable == false))
    {
        topSuitable = true;
    }

    if((resRight == 0b00000011) && (rightSuitable == false))
    {
        rightSuitable = true;
    }

    if((resBottom == 0b11000000) && (bottomSuitable == false))
    {
        bottomSuitable = true;
    }

    if((resLeft == 0b00110000) && (leftSuitable == false))
    {
        leftSuitable = true;
    }


    if(topSuitable && rightSuitable && bottomSuitable && leftSuitable)
    {
        // new part fits in the hole
        return true;
    }

    // new part does not fit in the hole --> try other (new) part
    return false;
}

uint8_t PuzzleSolverGrp3::GetOrientationForSolutionMatrix(uint16_t index, uint8_t side, uint8_t currentPartNoseOnSide)
{
    uint8_t partOrientationOnSide = 0b00000000;
    if((index == -1) || (index > m_part_array->size()))
    {
        //return ((0b11000000 >> (side * 2)));// ^ (HelperFunctions::ContinuousShift(currentPartNoseOnSide, 4)));
        partOrientationOnSide = 0b00000000;
    }
    else
    {
        partOrientationOnSide = m_part_array->at(index).getConnections() & HelperFunctions::ContinuousShift(0b11000000, -(side * 2));
    }

    return partOrientationOnSide;
}