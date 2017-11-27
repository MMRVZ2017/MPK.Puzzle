#include "PuzzleSolverGrp3.h"

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
				numZeilen = kanteVtemp + 2;
				numSpalten = kanteHtemp  + 2;
			}
			cout << "Zeilen: " << kanteVtemp + 2 << endl;
			cout << "Spalten: " << kanteHtemp + 2 << endl;
			cout << "Anzahl Kanten: " << kanteVtemp * 2 + kanteHtemp * 2 << endl; // 36*28 = 884
			cout << "Matrixgroesse = " << (kanteVtemp + 2)*(kanteHtemp + 2) << endl << endl;
		}
	}

	m_solutionVector = vector<vector<SolutionElement>>(numSpalten, vector<SolutionElement>(numZeilen));
}

void PuzzleSolverGrp3::SolvePuzzle()
{
    srand(time (NULL));
    /*
	cout << "Groesse:" << m_solutionVector.size() << endl;
	cout << "Groesse:" << m_solutionVector[1].size() << endl;
	cout << "Groesse:" << m_solutionVector[1][0].index << endl;
     */

    int posX = 0, posY = 0, countParts = 0, rotatedPart = 0, retrysPuzzleField = 0;

    uint8_t currentPart = 0b11111111;
    uint8_t tempCurrentPart = 0b11111111;
    //Part currentPart;
    //Part tempCurrentPart = currentPart;
    cout << "currentPart: " << bitset<8>(currentPart).to_string() << endl;

    // check for neighbours
    bool checkNeighbourLeft = false, checkNeighbourRight = false, checkNeighbourUpper = false, checkNeighbourUnder = false;
    // check for conflicts with neighbors
    bool checkConflictLeft = false, checkConflictRight = false, checkConflictUpper = false, checkConflictUnder = false;

    uint8_t upperPartFemale = 0b01000000, upperPartMale = 0b10000000;
    uint8_t rightPartFemale = 0b00010000, rightPartMale = 0b00100000;
    uint8_t underPartFemale = 0b00000100, underPartMale = 0b00001000;
    uint8_t leftPartFemale = 0b00000001, leftPartMale = 0b00000010;

    for (countParts = 0; (countParts < m_inners_array.size()) && (retrysPuzzleField < 1500); countParts++)
    {
        // Method setAllFalse();
        checkNeighbourLeft = false;
        checkNeighbourRight = false;
        checkNeighbourUpper = false;
        checkNeighbourUnder = false;
        checkConflictLeft = false;
        checkConflictRight = false;
        checkConflictUpper = false;
        checkConflictUnder = false;

        cout << "countParts = " << countParts << endl;
        currentPart = m_inners_array[countParts]->getConnections(); // Funktioniert noch nicht richtig (Teile muessen noch befuellt werden
        cout << "currentPart = " << bitset<8>(currentPart).to_string() << endl;

        // Matrix = 36*28 // 28*36
        posX = rand() % (m_solutionVector.size()-2) + 1; // row 1-34 (0-35)
        posY = rand() % (m_solutionVector[1].size()-2) + 1; // column 1-26 (0-27)
        cout << "X: " << posX << ", Y: " << posY << endl;

        if (m_solutionVector[posX][posY].index == 65535) // -1=65535, Field is empty => check neighbours
        {
            cout << "Current Field is free!" << endl;

            // check if neighbours are around this field
            if ((m_solutionVector[posX-1][posY].index == 65535) && (m_solutionVector[posX+1][posY].index == 65535) &&
                (m_solutionVector[posX][posY-1].index == 65535) && (m_solutionVector[posX][posY+1].index == 65535))
            {
                // no neighbours found: Place part at current place
                cout << "No neighbours here. Placing puzzle part at: " << posX << ", " << posY << endl;

                m_solutionVector[posX][posY].index = GetIndexFromPartArray(m_inners_array, countParts); // Fehler: Benoetige index von part_array
                m_solutionVector[posX][posY].orientation = 0;
            }
            else
            {
                cout << "Neighbours found. Checking conflicts with neighbours" << endl;

                // check all neighbours of current puzzle part
                if (m_solutionVector[posX-1][posY].index != -1)
                {
                    checkNeighbourLeft = true; // mark left puzzle for checking male/female
                }
                if (m_solutionVector[posX+1][posY].index != -1)
                {
                    checkNeighbourRight = true; // mark right puzzle for checking male/female
                }
                if (m_solutionVector[posX][posY-1].index != -1)
                {
                    checkNeighbourUpper = true; // mark upper puzzle for checking male/female
                }
                if (m_solutionVector[posX][posY+1].index != -1)
                {
                    checkNeighbourUnder = true; // mark under puzzle for checking male/female
                }

                // check conflict with neighbours:
                if (checkNeighbourLeft == true)
                {
                    if ((leftPartFemale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000 ||
                        (leftPartMale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000)
                    {
                        checkConflictLeft = false; // no conflict found
                    }
                    else
                    {
                        checkConflictLeft = true; // conflict is found => male/male or female/female
                    }
                }
                if (checkNeighbourRight == true)
                {
                    if ((rightPartFemale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000 ||
                        (rightPartMale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000)
                    {
                        checkConflictRight = false; // no conflict found
                    } else
                    {
                        checkConflictRight = true; // conflict is found => male/male or female/female
                    }
                }
                if (checkNeighbourUnder == true)
                {
                    if ((underPartFemale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000 ||
                        (underPartMale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000)
                    {
                        checkConflictUnder = false; // no conflict found
                    }
                    else
                    {
                        checkConflictUnder = true; // conflict is found => male/male or female/female
                    }
                }
                if (checkNeighbourUpper == true)
                {
                    if ((upperPartFemale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000 ||
                        (upperPartMale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000)
                    {
                        checkConflictUpper = false; // no conflict found
                    } else
                    {
                        checkConflictUpper = true; // conflict is found => male/male or female/female
                    }
                }


                // rotate until maximum (=3) reached
                if ( (checkConflictLeft == false &&  checkConflictRight == false &&  checkConflictUnder == false &&  checkConflictUpper == false))
                     //&& (checkNeighbourLeft == false || checkNeighbourRight == false || checkNeighbourUnder == false || checkNeighbourUpper == false) )
                {
                    // there where no conflicts found with neighbour parts
                    cout << "No conflicts with neighbour parts. Place Puzzle part at: " << "X:[" << posX << "]" << " Y:[" << posY << "]" << endl;

                    m_solutionVector[posX][posY].index = GetIndexFromPartArray(m_inners_array, countParts);
                    m_solutionVector[posX][posY].orientation = rotatedPart;

                    rotatedPart = 0; // reset rotatedPart
                }
                //else if ( checkConflictLeft == true ||  checkConflictRight == true ||  checkConflictUnder == true ||  checkConflictUpper == true && (rotatedPart < 3))
                else if (rotatedPart < 3)
                {
                    // rotate part until it was 3 times rotated
                    cout << "Rotating part. counterRotatedPart = " << rotatedPart << endl;
                    currentPart = HelperFunctions::ContinuousShift(currentPart, -2); // rotate part for 90° in right direction
                    rotatedPart++;
                }
                else
                {
                    // no solution for this part found
                    cout << "Found no solution for this Puzzle-Part, searching another Place in the Field!" << endl;

                    countParts--;
                    rotatedPart = 0;
                    continue;
                }
            }

            retrysPuzzleField = 0;
        }
        else // Field is not empty => search another Field for puzzle-part
        {
            cout << "X: " << posX << ", Y: " << posY << endl;
            cout << "m_solutionVector at [posX][posY].index = " << m_solutionVector[posX][posY].index << endl;
            cout << "Field is already used, searching another Field" << endl;
            cout << "---" << endl;

            retrysPuzzleField++;
            countParts--;
            rotatedPart = 0;
            continue;
        }
    }


    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < m_solutionVector.size(); i++)
    {
        for (int j = 0; j < m_solutionVector[1].size(); j++)
        {
            cout << " " << m_solutionVector[i][j].index;
            //cout << "||" << bitset<8>(part_array[index].getConnections()).to_string();
        }
        cout << endl;
    }
    cout << "----------------------------------------------" << endl;
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

PuzzleSolverGrp3::~PuzzleSolverGrp3()
{
}
