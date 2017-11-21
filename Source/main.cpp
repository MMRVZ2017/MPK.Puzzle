#include <iostream>
#include <vector>
#include "path.h"
#include "constrMatrix.h"

using namespace std;

int main() {
    ConstrMatrix myfirstMatrix;
    myfirstMatrix.print_matrix();
    

    cout << endl;
    myfirstMatrix.print_constraints(0,0);
    myfirstMatrix.print_constraints(1,0);
    myfirstMatrix.print_constraints(1,1);
    myfirstMatrix.print_constraints(0,NR_COLS-1);
    myfirstMatrix.print_constraints(NR_ROWS-1,0);
    myfirstMatrix.print_constraints(NR_ROWS-1,NR_COLS-1);
    cout << endl;

    myfirstMatrix.set_constraints(0,1,0b00110111);
    myfirstMatrix.set_constraints(2,1,0b10111111);
    myfirstMatrix.set_constraints(1,0,0b11011100);
    myfirstMatrix.set_constraints(1,2,0b11111110);

    myfirstMatrix.print_matrix();
    myfirstMatrix.print_constraints(1,1);

    uint8_t testPart[5]{
            0b00000000,
            0b11111111,
            0b01101001,
            0b00011011,
            0b10010110,
    };
    for (int i = 0; i < 5 ; i++) {
        cout << endl << "Checking position (1,1) with Part " << bitset<sizeof(unsigned char)*8>(testPart[i])<< endl;
        cout << "Result:" << unsigned(check_constraints(myfirstMatrix.get_constraints(1, 1), testPart[i], 0)) << endl;

    }

//    return 1;


    /* ------------------------------ START OF PRE-CALC ------------------------------*/
    // INTERFACE TO GROUP REITER, BAUMGARTNER
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);
    // --------------------------------------

    // EXAMPLE PUZZLE
    part_array[0].setConnections(0b00010100); // Piece 1
    part_array[1].setConnections(0b00011010); // Piece 2
    part_array[2].setConnections(0b00100110); // Piece 3
    part_array[3].setConnections(0b00001001); // Piece 4
    part_array[4].setConnections(0b10101000); // Piece 5
    part_array[5].setConnections(0b01011001); // Piece 6
    part_array[6].setConnections(0b10101010); // Piece 7
    part_array[7].setConnections(0b01000101); // Piece 8
    part_array[8].setConnections(0b01010000); // Piece 9
    part_array[9].setConnections(0b01010010); // Piece 10
    part_array[10].setConnections(0b01010010); // Piece 11
    part_array[11].setConnections(0b10000010); // Piece 12
    // END OF PUZZLE DECLARATION

    /* ------------------------------ END OF PRE-CALC ----------------------------------*/
    /* ------------------------------ START OF PATH-STUFF ------------------------------*/

    Path puzzlePath;
    puzzlePath.setPositions();                              // Give each step a position and the resulting constraints (edges, corners, inners)
    puzzlePath.getPuzzleBox()->sortParts(&part_array);      // Sort Array of Input-Parts (Christoph's Part)
    //puzzlePath.getPuzzleBox()->printAllVectors();

    puzzlePath.getPuzzleBox()->printTypeArr();              // Prints the number of available parts of each part-type
    Step::printNrUsedPartType();                            // Prints the number of used parts of each part-type

    for (uint16_t i = 0; i < NR_POSITIONS; i++){
        puzzlePath.placePart(puzzlePath.getStep(i));        // Places a part for every step (No part-constraints, just edges,corners,inners-constraints)
    }

    cout << "Rotated to: " << unsigned(puzzlePath.getStep(2)->rotate()) << endl;        // This rotates a part and returns it's new positions

    puzzlePath.printPath();                                 // Where "1" is visible, this part is possible

    puzzlePath.getPuzzleBox()->printTypeArr();
    Step::printNrUsedPartType();

    return 1;
}

// THIS IS HOW A STEP-ITERATOR WOULD WORK
//auto stepIt = puzzlePath.getPath()->begin();
//stepIt = stepIt + 3;
//cout << *stepIt << endl;

// POSITION AND ROTATION COULD BE SET THIS WAY (NOT RECOMMENED)
//puzzlePath.getStep(1)->setOrientation(2);
//puzzlePath.getStep(1)->setPosition(2,3);

// THIS IS HOW THE PATH CAN BE PRINTED
//puzzlePath.printPath(2, 2);     // only 2nd element
//puzzlePath.printPath(4,7);      // 4-7
//puzzlePath.printPath();         // all
