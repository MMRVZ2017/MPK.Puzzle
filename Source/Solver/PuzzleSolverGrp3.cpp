#include "PuzzleSolverGrp3.h"
#include "../Helper/HelperFunctions.h"
#include <bitset>
#include <map>

PuzzleSolverGrp3::PuzzleSolverGrp3(vector <Part>* part_array) : m_part_array(part_array)
{
    InitializeVectors();
    InitializeSolutionMatrix();
}

PuzzleSolverGrp3::~PuzzleSolverGrp3()
{
}

void PuzzleSolverGrp3::InitializeVectors()
{
    for(int index = 0; index < m_part_array->size(); index++)
    {
        bitset<8> partConnections = m_part_array->at(index).getConnections();

        switch(partConnections.count())
        {
            case 2:
                m_corners_array.push_back(&m_part_array->at(index));
                break;
            case 3:
                m_edges_array.push_back(&m_part_array->at(index));
                break;
            case 4:
                m_inners_array.push_back(&m_part_array->at(index));
                break;
            default:
                cout << "Error: Should not be called\n";
                break;
        }
    }
}

void PuzzleSolverGrp3::InitializeSolutionMatrix()
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
        }
    }

    m_solutionVector = vector<vector<SolutionElement>>(numSpalten, vector<SolutionElement>(numZeilen));
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
                //cout << "numTries: " << numTries << " Index: " << index << endl;
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

    // Vector mit möglichen Andockstellen erzeugen
    // The left upper corner always at first
    vector<pair<int,int>> nextPuzzlePlaces;
    nextPuzzlePlaces.push_back(make_pair(0, 0));

    for(int cornerIndex = 0; (cornerIndex < m_corners_array.size() && puzzleSolved == false); cornerIndex++)
    {
        bool firstPartSet = false;
        int numRotations = 0;
        int partIndex = 0;
        uint8_t partOrientation = 0;

        // Setzen des ersten Bausteins --> Muss eine Ecke sein
        while((!firstPartSet) && (numRotations < 4))
        {
            partIndex = GetIndexFromPart(m_corners_array[cornerIndex]);
            partOrientation = m_part_array->at(partIndex).getConnections();
            partOrientation = HelperFunctions::ContinuousShift(partOrientation, (numRotations * 2));
            firstPartSet = PuzzleLogic_v2(partOrientation, 0, 0);
            if(!firstPartSet)
            {
                numRotations++;
            }
        }



        vector<pair<int,int>> tempPuzzle;
        tempPuzzle.push_back(make_pair(partIndex, partOrientation));

        //Rekursive Logik zum Erkennen von möglichen Anbausteinen und testen des Steins mit den geringsten Möglichkeiten
        puzzleSolved = RecursiveFindAndPlace(nextPuzzlePlaces, tempPuzzle);
    }

    // Dann immer dort wo am meisten Möglichkeiten
}

bool PuzzleSolverGrp3::SolvePuzzleDave()
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

                m_solutionVector[posX][posY].index = GetIndexFromPart(m_inners_array[countParts]); // Fehler: Benoetige index von part_array
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

                    m_solutionVector[posX][posY].index = GetIndexFromPart(m_inners_array[countParts]);
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

    return true;
}

bool PuzzleSolverGrp3::SolvePuzzleNiko() {

    cout << "Groesse Zeilen:" << m_solutionVector.size() << endl;
    cout << "Groesse Spalten:" << m_solutionVector[1].size() << endl;
    cout << "Groesse:" << m_solutionVector[1][0].index << endl;

    /*for (int i = 0; i < 500; i++)
    {
        posX = rand() % (m_solutionVector.size()-2) + 1;
        cout << posX << " ,";
    }*/

    //niko part start
    solveOuterEdge();
    solveCorners();
    //niko part end

    return true;
}

void PuzzleSolverGrp3::solveOuterEdge() {

    // Initialisation
    int x = 0, y = 0, position = 0, rotation = 0, countParts = 0, retriesPuzzleField = 0, rotatedPart = 0;
    bool operationPartPlacedComplete = false;
    uint8_t currentPartEdge = 0b11111111;


    // check for neighbours
    bool checkNeighbourLeft = false, checkNeighbourRight = false, checkNeighbourUpper = false, checkNeighbourUnder = false;
    // check for conflicts with neighbors
    bool checkConflictLeft = false, checkConflictRight = false, checkConflictUpper = false, checkConflictUnder = false;

    // Puzzle piece description
    uint8_t upperPartFemale = 0b01000000, upperPartMale = 0b10000000;
    uint8_t rightPartFemale = 0b00010000, rightPartMale = 0b00100000;
    uint8_t underPartFemale = 0b00000100, underPartMale = 0b00001000;
    uint8_t leftPartFemale = 0b00000001, leftPartMale = 0b00000010;

    while (operationPartPlacedComplete == false) {

        //Generate random x&y coordinates
        srand(time(NULL));
        if (rand() % 4 == 0) {
            //top
            x = (rand() % (m_solutionVector[0].size())); //oder x = (rand() % (m_solutionVector[0].size()-1));?
            y = 0;
            position = 0;

        } else if (rand() % 4 == 1) {
            //right
            x = m_solutionVector[0].size();
            y = ((rand() % (m_solutionVector.size())));
            if (y == 0) { continue; }
            position = 1;

        } else if (rand() % 4 == 2) {
            //bottom
            x = (rand() % (m_solutionVector[0].size()));
            if (x == 0) { continue; }
            y = m_solutionVector[0].size();
            position = 2;

        } else if (rand() % 4 == 3) {
            //left
            x = 0;
            y = ((rand() % (m_solutionVector.size())));
            if (y == 0) { continue; }
            position = 3;
        }
        //cout << "x= " << x << ", y= " << y << endl;

        for (countParts = 0;
             (countParts < m_edges_array.size()) && (retriesPuzzleField < (m_edges_array.size() * 100)); countParts++) {

            checkNeighbourLeft = false;
            checkNeighbourRight = false;
            checkNeighbourUpper = false;
            checkNeighbourUnder = false;
            checkConflictLeft = false;
            checkConflictRight = false;
            checkConflictUpper = false;
            checkConflictUnder = false;

            currentPartEdge = m_edges_array[countParts]->getConnections();

            if (m_solutionVector[x][y].index == 65535) // -1=65535, indicates that field is free
            {
                // Checks if there are neighbors
                if (m_solutionVector[x - 1][y].index != -1) {
                    checkNeighbourLeft = true; // mark left puzzle for checking male/female
                }
                if (m_solutionVector[x + 1][y].index != -1) {
                    checkNeighbourRight = true; // mark right puzzle for checking male/female
                }
                if (m_solutionVector[x][y - 1].index != -1) {
                    checkNeighbourUpper = true; // mark upper puzzle for checking male/female
                }
                if (m_solutionVector[x][y + 1].index != -1) {
                    checkNeighbourUnder = true; // mark under puzzle for checking male/female
                }


                //Check conflicts for edge parts
                if (
                        ((underPartFemale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000 ||
                         (underPartMale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000)) {
                    checkConflictUnder = false;
                } else { checkConflictUnder = true; }


                if (
                        ((leftPartFemale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000 ||
                         (leftPartMale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000)) {
                    checkConflictLeft = false;
                } else { checkConflictLeft = true; }


                if (
                        ((upperPartFemale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000 ||
                         (upperPartMale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000)) {
                    checkConflictUpper = false;
                } else { checkConflictUpper = true; }

                if (
                        ((rightPartFemale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000 ||
                         (rightPartMale & HelperFunctions::ContinuousShift(currentPartEdge, -4)) == 0b00000000)) {
                    checkConflictRight = false;
                } else { checkConflictRight = true; }

                // assign the solution vector with current part, other wise rotate the part and try again
                if ((checkConflictLeft == false && checkConflictRight == false && checkConflictUnder == false &&
                     checkConflictUpper == false)) {

                    if (position == 0 && (~(currentPartEdge | 0b00111111) == 0b11000000)) {
                        m_solutionVector[x][y].index = GetIndexFromPart(m_edges_array[countParts]);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 1 && (~(currentPartEdge | 0b11001111) == 0b00110000)) {
                        m_solutionVector[x][y].index = GetIndexFromPart(m_edges_array[countParts]);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 2 && (~(currentPartEdge | 0b11110011) == 0b00001100)) {
                        m_solutionVector[x][y].index = GetIndexFromPart(m_edges_array[countParts]);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 3 && (~(currentPartEdge | 0b11111100) == 0b00000011)) {
                        m_solutionVector[x][y].index = GetIndexFromPart(m_edges_array[countParts]);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    }
                    rotatedPart = 0; // reset rotatedPart
                } else if (rotatedPart < 3) {
                    // rotate part until it was 3 times rotated
                    currentPartEdge = HelperFunctions::ContinuousShift(currentPartEdge, -2); // rotate part for 90° in right direction
                    rotatedPart++;
                } else {
                    // no solution for this part found
                    countParts--;
                    rotatedPart = 0;
                    continue;
                }

            }
        }
    }
}

void PuzzleSolverGrp3::solveCorners() {
    int countParts = 0, retriesPuzzleField, pieceDecide = 0, x = 0, y = 0, rotatedPart = 0;

    uint8_t currentPart = 0b11111111;
    uint8_t upperPartFemale = 0b01000000, upperPartMale = 0b10000000;
    uint8_t rightPartFemale = 0b00010000, rightPartMale = 0b00100000;
    uint8_t underPartFemale = 0b00000100, underPartMale = 0b00001000;
    uint8_t leftPartFemale = 0b00000001, leftPartMale = 0b00000010;

    // check edges
    bool checkEdgeLeftFemale = false, checkEdgeRightFemale = false, checkEdgeUpperFemale = false, checkEdgeUnderFemale = false;
    bool checkEdgeLeftMale = false, checkEdgeRightMale = false, checkEdgeUpperMale = false, checkEdgeUnderMale = false;


    for (countParts = 0; (countParts < m_corners_array.size()) && (retriesPuzzleField < 1000); countParts++) {

        // choose current part
        srand(time(NULL));
        pieceDecide = rand() % 4;
        if (pieceDecide == 0) {
            currentPart = (m_edges_array[0]->getConnections());
        } else if (pieceDecide == 1) {
            currentPart = (m_edges_array[1]->getConnections());
        } else if (pieceDecide == 2) { currentPart = (m_edges_array[2]->getConnections()); }
        else { currentPart = (m_edges_array[3]->getConnections()); }

        // Checking if male or female parts connect with edges
        if ((leftPartFemale & HelperFunctions::ContinuousShift(currentPart, -6)) == 0b00000000 &&
            (leftPartFemale & HelperFunctions::ContinuousShift(currentPart, -5)) == 0b00000000) {
            checkEdgeLeftFemale = false; // no conflict found
        } else {
            checkEdgeLeftFemale = true; // conflict is found => male/male or female/female
        }

        if ((leftPartMale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000 &&
            (leftPartMale & HelperFunctions::ContinuousShift(currentPart, -3)) == 0b00000000) {
            checkEdgeLeftMale = false; // no conflict found
        } else {
            checkEdgeLeftMale = true; // conflict is found => male/male or female/female
        }

        if ((rightPartFemale & HelperFunctions::ContinuousShift(currentPart, -3)) == 0b00000000 &&
            (rightPartFemale & HelperFunctions::ContinuousShift(currentPart, -2)) == 0b00000000) {
            checkEdgeRightFemale = false; // no conflict found
        } else {
            checkEdgeRightFemale = true; // conflict is found => male/male or female/female
        }

        if ((rightPartMale & HelperFunctions::ContinuousShift(currentPart, -2)) == 0b00000000 &&
            (rightPartMale & HelperFunctions::ContinuousShift(currentPart, -1)) == 0b00000000) {
            checkEdgeRightMale = false; // no conflict found
        } else {
            checkEdgeRightMale = true; // conflict is found => male/male or female/female
        }

        if ((upperPartFemale & HelperFunctions::ContinuousShift(currentPart, -1)) == 0b00000000 &&
            (upperPartFemale & HelperFunctions::ContinuousShift(currentPart, 0)) == 0b00000000) {
            checkEdgeUpperFemale = false; // no conflict found
        } else {
            checkEdgeUpperFemale = true; // conflict is found => male/male or female/female
        }

        if ((upperPartMale & HelperFunctions::ContinuousShift(currentPart, 0)) == 0b00000000 &&
            (upperPartMale & HelperFunctions::ContinuousShift(currentPart, 1)) == 0b00000000) {
            checkEdgeUpperMale = false; // no conflict found
        } else {
            checkEdgeUpperMale = true; // conflict is found => male/male or female/female
        }

        if ((underPartFemale & HelperFunctions::ContinuousShift(currentPart, -5)) == 0b00000000 &&
            (underPartFemale & HelperFunctions::ContinuousShift(currentPart, -4)) == 0b00000000) {
            checkEdgeUnderFemale = false; // no conflict found
        } else {
            checkEdgeUnderFemale = true; // conflict is found => male/male or female/female
        }

        if ((underPartMale & HelperFunctions::ContinuousShift(currentPart, -3)) == 0b00000000 &&
            (underPartMale & HelperFunctions::ContinuousShift(currentPart, -2)) == 0b00000000) {
            checkEdgeUnderMale = false; // no conflict found
        } else {
            checkEdgeUnderMale = true; // conflict is found => male/male or female/female
        }

        // assign the solution vector with current part, other wise rotate the part and try again
        if ((checkEdgeLeftFemale == false) && (checkEdgeRightFemale == false)&& (checkEdgeUpperFemale == false) &&
            (checkEdgeUnderFemale == false) && (checkEdgeLeftMale == false) && (checkEdgeRightMale == false) &&
            (checkEdgeUpperMale == false)&& (checkEdgeUnderMale == false)) {

            if(pieceDecide==0){
                x=0, y=0;
                m_solutionVector[x][y].index = GetIndexFromPart(m_corners_array[countParts]);
                m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==1){
                x=26;
                y=0;
                m_solutionVector[x][y].index = GetIndexFromPart(m_corners_array[countParts]);
                m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==2){
                x=26, y=38;
                m_solutionVector[x][y].index = GetIndexFromPart(m_corners_array[countParts]);
                m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==3){
                x=0, y=38;
                m_solutionVector[x][y].index = GetIndexFromPart(m_corners_array[countParts]);
                m_solutionVector[x][y].orientation = rotatedPart;}
        }
    }
}

int PuzzleSolverGrp3::GetIndexFromPart(Part* part)
{
    int partIndex = 0;
    for(int index = 0; index < m_part_array->size(); index++)
    {
        if(&m_part_array->at(index) == part)
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
        Part* nextPart = 0;

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

        // Setzen des ersten Bausteins --> Muss eine Ecke sein
        bool PartSet = false;
        int numRotations = 0;
        int partIndex = 0;
        uint8_t partOrientation = 0;
        while((!PartSet) && (numRotations < 4))
        {
            partIndex = GetIndexFromPart(nextPart);
            partOrientation = m_part_array->at(partIndex).getConnections();
            partOrientation = HelperFunctions::ContinuousShift(partOrientation, (numRotations * 2));
            PartSet = PuzzleLogic_v2(partOrientation, 0, 0);
            if(!PartSet)
            {
                numRotations++;
            }
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

vector<vector<SolutionElement>>* PuzzleSolverGrp3::GetSolutionMatrix()
{
    return &m_solutionVector;
}