#include <iostream>
#include <vector>
#include "path.h"
#include "constrMatrix.h"
#include "test_puzzle_long40x40.h"
#include "puzzleGenerator.hpp"

using namespace std;

int main() {


    /* ------------------------------ START OF PRE-CALC ------------------------------*/
    // INTERFACE TO GROUP REITER, BAUMGARTNER
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);
    // --------------------------------------

    // EXAMPLE PUZZLE 3x4
    /*
    part_array[0].setConnections(0b01010000); // Piece 1
    part_array[1].setConnections(0b01101000); // Piece 2
    part_array[2].setConnections(0b00100110); // Piece 3
    part_array[3].setConnections(0b01000010); // Piece 4
    part_array[4].setConnections(0b10101000); // Piece 5
    part_array[5].setConnections(0b01011001); // Piece 6
    //part_array[6].setConnections(0b01011010); // Piece 7
    part_array[6].setConnections(0b10101010); // Piece 7
    part_array[7].setConnections(0b01000101); // Piece 8
    part_array[8].setConnections(0b01010000); // Piece 9
    part_array[9].setConnections(0b01010010); // Piece 10
    part_array[10].setConnections(0b01010010); // Piece 11
    part_array[11].setConnections(0b10000010); // Piece 12
    */

    // EXAMPLE PUZZLE 3x3
    /*
	part_array[0].setConnections(0b00010100); // Piece 1
    part_array[1].setConnections(0b00011010); // Piece 2
    part_array[2].setConnections(0b00000110); // Piece 3
    part_array[3].setConnections(0b10101000); // Piece 5
    part_array[4].setConnections(0b01011001); // Piece 6
    part_array[5].setConnections(0b00101010); // Piece 7
    part_array[6].setConnections(0b01010000); // Piece 9
    part_array[7].setConnections(0b01010010); // Piece 10
    part_array[8].setConnections(0b01000010); // Piece 11
	*/

    // Puzzle Generator START
    randomBox myRandomBox(NR_COLS,NR_ROWS); // Just change the define values for NR_COLS and NR_ROWS
    myRandomBox.createRandomPuzzle();
    part_array = myRandomBox.shuffle();
    // Puzzle Generator END

    //makehard1010puzzle(part_array);
    //makehard4040puzzle(part_array);

    // END OF PUZZLE DECLARATION

    /* ------------------------------ END OF PRE-CALC ----------------------------------*/
    /* ------------------------------ START OF PATH-STUFF ------------------------------*/

    Path puzzlePath;
    puzzlePath.setPositions();                              // Give each step a position and the resulting constraints (edges, corners, inners)
    puzzlePath.getPuzzleBox()->sortParts(&part_array);      // Sort Array of Input-Parts (Christoph's Part)
    //puzzlePath.getPuzzleBox()->printAllVectors();

    puzzlePath.getPuzzleBox()->printTypeArr();              // Prints the number of available parts of each part-type
    Step::printNrUsedPartType();                            // Prints the number of used parts of each part-type
    puzzlePath.countNumTypesUsed();
    //puzzlePath.printPath();

    //        puzzlePath.placePart(puzzlePath.getStep(i));        // Places a part for every step (No part-constraints, just edges,corners,inners-constraints)

    cout << "Matrix at the beginning" << endl;
    puzzlePath.getConstrMatrix()->print_matrix();

    int numTries = 0;
    int numGoBack = 0;
    int16_t furthestStep = 0;
    int tryOrder[10000] = {0};
    int orderCounter = 0;
    int counter = 0;
    int counterLimit = 1;
    int limit = 100000 ;//48000000;

    bool flag = false;
    uint16_t goBackIdx = 0;
    for (uint16_t i = 0; i < NR_POSITIONS; i++){
        if(PRINT_MESSAGES) cout << "PLACING PART " << unsigned(i) <<endl;
        numTries++;
        if(i > furthestStep) furthestStep = i;
        //tryOrder[orderCounter] = i; orderCounter ++;
        if (counter >= counterLimit) break;
        flag = puzzlePath.placePart(puzzlePath.getStep(i), 0);
        if (flag == false) {
            for (goBackIdx = uint16_t(i); flag != true; goBackIdx--) {
                if(PRINT_MESSAGES) cout << "GO BACK FROM PART" << unsigned(goBackIdx) << endl;
                numGoBack++;
                puzzlePath.goBack(goBackIdx);
                if(PRINT_MESSAGES) cout << "PLACING " << goBackIdx-1 << " Was Part: " << signed(puzzlePath.getStep(goBackIdx-1)->getFittingPartTypeIdx()) << endl;
                if(PRINT_MESSAGES) puzzlePath.getStep(uint16_t(goBackIdx-1))->printPossiblePartTypeArr();
                //tryOrder[orderCounter] = goBackIdx-1; orderCounter ++;
                flag = puzzlePath.placePart(puzzlePath.getStep(uint16_t(goBackIdx-1)), 1);
            }
            i = goBackIdx;
        }
        if (numTries >= limit){
            i = 0;
            counter++;
            cout << counter << endl;
            //puzzlePath.getConstrMatrix()->print_matrix();
            //puzzlePath.resetPath();
            //puzzlePath.printPath();
            //puzzlePath.getConstrMatrix()->print_matrix();
        }
    }   // TODO: Catch wrong puzzles, Catch goBack to index 0

    //cout << "Rotated to: " << unsigned(puzzlePath.getStep(2)->rotate()) << endl;        // This rotates a part and returns it's new positions

    cout << "Matrix at the end" << endl;
    puzzlePath.getConstrMatrix()->print_matrix();

    puzzlePath.printPath();                                 // Where "1" is visible, this part is possible

    puzzlePath.getPuzzleBox()->printTypeArr();
    Step::printNrUsedPartType();
    /*for (int i = 0; i < orderCounter; i++){
        cout << tryOrder[i] << endl;
        //if (counter % 100) cout << endl;
    }
    cout << endl;*/
    //puzzlePath.printSolution();

    puzzlePath.getConstrMatrix()->print_matrix();

    cout << "Tries: " << numTries << " Go Back: " << numGoBack << " Furthest Step: " << signed(furthestStep) << endl;

    if(numTries >= limit) {
        cout << "#####################" << endl;
        cout << "#       ERROR       #" << endl;
        cout << "#####################" << endl;
    } else {
        cout << "#####################" << endl;
        cout << "#      SUCCESS      #" << endl;
        cout << "#####################" << endl;
    }

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
