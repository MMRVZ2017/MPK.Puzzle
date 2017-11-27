#include "PuzzleSolverGrp3.h"
#include <bitset>

PuzzleSolverGrp3::PuzzleSolverGrp3(vector<Part> *part_array, vector<Part *> corners_array, vector<Part *> edges_array,
                                   vector<Part *> inners_array)
        : m_part_array(part_array), m_corners_array(corners_array), m_edges_array(edges_array),
          m_inners_array(inners_array) {

    {
        int numSpalten = 0;
        int numZeilen = 0;
        for (int kanteHtemp = 1; kanteHtemp <= (m_edges_array.size() / 2) - 1; kanteHtemp++) {
            int kanteVtemp = (m_edges_array.size() / 2) - kanteHtemp;

            if ((kanteHtemp * kanteVtemp) == m_inners_array.size()) // 884
            {
                if (kanteHtemp >= kanteVtemp) {
                    numZeilen = kanteHtemp + 2;
                    numSpalten = kanteVtemp + 2;
                }
                /*cout << "Zeilen: " << kanteVtemp + 2 << endl;
                cout << "Spalten: " << kanteHtemp + 2 << endl;
                cout << "Anzahl Kanten: " << kanteVtemp * 2 + kanteHtemp * 2 << endl; // 36*28 = 884
                cout << "Matrixgroesse = " << (kanteVtemp + 2)*(kanteHtemp + 2) << endl;*/
            }
        }

        m_solutionVector = vector<vector<SolutionElement>>(numSpalten, vector<SolutionElement>(numZeilen));
    }

}
void PuzzleSolverGrp3::SolvePuzzle() {

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
}
PuzzleSolverGrp3::~PuzzleSolverGrp3() {
}
uint8_t getEdgeOrientation(Part inputPart) {
    //Edge top
    if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
        ((inputPart.getConnections() & 0b00010101) == 0b00010101)
            ) { return 0b00010101; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00100101) == 0b00100101)
            ) { return 0b00100101; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00101001) == 0b00101001)
            ) { return 0b00101001; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00101010) == 0b00101010)
            ) { return 0b00101010; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00011001) == 0b00011001)
            ) { return 0b00011001; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00011010) == 0b00011010)
            ) { return 0b00011010; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00100110) == 0b00100110)
            ) { return 0b00100110; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00010110) == 0b00010110)
            ) { return 0b00010110; }
    else if (((inputPart.getConnections() & 0b00000000) == 0b00000000) &&
             ((inputPart.getConnections() & 0b00011010) == 0b00011010)
            ) { return 0b00011010; }
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
                        m_solutionVector[x][y].index = GetIndexFromPartArray(m_edges_array, countParts);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 1 && (~(currentPartEdge | 0b11001111) == 0b00110000)) {
                        m_solutionVector[x][y].index = GetIndexFromPartArray(m_edges_array, countParts);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 2 && (~(currentPartEdge | 0b11110011) == 0b00001100)) {
                        m_solutionVector[x][y].index = GetIndexFromPartArray(m_edges_array, countParts);
                        m_solutionVector[x][y].orientation = rotatedPart;
                    } else if (position == 3 && (~(currentPartEdge | 0b11111100) == 0b00000011)) {
                        m_solutionVector[x][y].index = GetIndexFromPartArray(m_edges_array, countParts);
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












/*
        for(int schleife=0; schleife < m_edges_array.size();schleife++) {
            current_Part = &m_part_array->at(GetIndexFromPartArray(m_edges_array, schleife));
        }
        //Checking if both neighbors are free
        //
        //left and right part free at top ?
        if (position == 0 && x >= 2 && m_solutionVector[x - 1][y].index == -1
            && m_solutionVector[x + 1][y].index == -1) {

            if(getEdgeOrientation(current_Part)== 0b00010101 ||
               getEdgeOrientation(current_Part)== 0b00100101 ||
               getEdgeOrientation(current_Part)== 0b00101001 ||
               getEdgeOrientation(current_Part)== 0b00101010 ||
               getEdgeOrientation(current_Part)== 0b00011001 ||
               getEdgeOrientation(current_Part)== 0b00011010 ||
               getEdgeOrientation(current_Part)== 0b00100110 ||
               getEdgeOrientation(current_Part)== 0b00010110 ||
               getEdgeOrientation(current_Part)== 0b00011010){
            m_solutionVector[x][y].index = current_Part.getConnections();
            m_solutionVector[x][y].orientation = 0;}
        }
            //left and right part free at bottom ?
        else if (position == 2 && x >= 2 && m_solutionVector[x - 1][y].index == -1
                 && m_solutionVector[x + 1][y].index == -1) {

            //m_solutionVector[x][y].index =
            m_solutionVector[x][y].orientation = 0;
        }
            // upper and below part free at right ?
        else if (position == 1 && y >= 2 && m_solutionVector[x][y - 1].index == -1
                 && m_solutionVector[x][y + 1].index == -1) {

            //m_solutionVector[x][y].index =
            m_solutionVector[x][y].orientation = 0;

        }
            // upper and below part free at left ?
        else if (position == 3 && y >= 2 && m_solutionVector[x][y - 1].index == -1
                 && m_solutionVector[x][y + 1].index == -1) {

            //m_solutionVector[x][y].index =
            m_solutionVector[x][y].orientation = 0;

        }

        // Checking if one neighbor is free
        // left part free at top or bottom?
        else if (position == 0 || position == 2 && x >= 2 && m_solutionVector[x - 1][y].index == -1) {
        }
            //right part free at top or bottom?
        else if (position == 0 || position == 2 && x >= 1 && m_solutionVector[x + 1][y].index == -1) {
        }
            //upper part free at right or left?
        else if (position == 1 || position == 3 && y >= 2 && m_solutionVector[x][y - 1].index == -1) {
        }
            //below part free at right or left?
        else if (position == 1 || position == 3 && y <= 1 && m_solutionVector[x][y + 1].index == -1) {
        }

*/




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
            m_solutionVector[x][y].index = GetIndexFromPartArray(m_corners_array, countParts);
            m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==1){
                x=26;
                y=0;
                m_solutionVector[x][y].index = GetIndexFromPartArray(m_corners_array, countParts);
                m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==2){
                x=26, y=38;
                m_solutionVector[x][y].index = GetIndexFromPartArray(m_corners_array, countParts);
                m_solutionVector[x][y].orientation = rotatedPart;}

            if(pieceDecide==3){
                x=0, y=38;
                m_solutionVector[x][y].index = GetIndexFromPartArray(m_corners_array, countParts);
                m_solutionVector[x][y].orientation = rotatedPart;}
        }
    }
}

int PuzzleSolverGrp3::GetIndexFromPartArray(vector<Part *> wishedArray, int wishedIndex) {
    int partIndex = 0;
    for (int index = 0; index < m_part_array->size(); index++) {
        if (&m_part_array->at(index) == wishedArray[wishedIndex]) {
            partIndex = index;
            break;
        }
    }
    return partIndex;
}