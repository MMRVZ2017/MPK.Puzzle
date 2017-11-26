#include <iostream>
#include <vector>
#include "path.h"
#include "constrMatrix.h"
#include "test_puzzle_long40x40.h"
#include "puzzleGenerator.hpp"

using namespace std;

int main() {
    // INTERFACE TO GROUP REITER, BAUMGARTNER
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);

    // INIT PUZZLE GENERATOR
    randomBox myRandomBox(NR_COLS, NR_ROWS); // Just change the define values for NR_COLS and NR_ROWS in defines.h
    myRandomBox.createRandomPuzzle();
    part_array = myRandomBox.shuffle();
    //makehard4040puzzle(part_array);

    // INIT PUZZLE SOLVER
    Path puzzlePath(&part_array);                           // Make a new path
    puzzlePath.setPositions();                              // Give each step a position and the resulting constraints (edges, corners, inners)
    //puzzlePath.getPuzzleBox()->printAllVectors();

    // PRINT INFORMATION
    Puzzlebox::printTypeArr();                              // Print the number of available parts of each part-type
    Step::printNrUsedPartType();                            // Print the number of used parts of each part-type
    puzzlePath.count_printNumTypesUsed();                         // Print the number of actually used parts (1 set in possiblePartType[])
    //puzzlePath.printPath();
    cout << "Matrix at the beginning" << endl;
    puzzlePath.getConstrMatrix()->print_matrix();

    // SOLVE PUZZLE         -------*******------- THIS IS WHERE THE MAGIC HAPPENS -------*******-------
    int32_t limit = 50000;
    int32_t numTries = 0;
    numTries = puzzlePath.solvePuzzle(limit);               // This solves the puzzle

    // EXPORT SOLUTION
    // TODO: To be implemented

    // PRINT INFORMATION
    cout << "Matrix at the end" << endl;
    puzzlePath.getConstrMatrix()->print_matrix();
    Puzzlebox::printTypeArr();                              // Print the number of available parts of each part-type
    Step::printNrUsedPartType();                            // Print the number of used parts of each part-type
    puzzlePath.count_printNumTypesUsed();                         // Print the number of actually used parts (1 set in possiblePartType[])
    //puzzlePath.printPath();
    //puzzlePath.printDebug();

    cout << "Tries: " << numTries << " Go Back: " << puzzlePath.numGoBack << " Furthest Step: " << signed(puzzlePath.furthestStep) << endl;

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

// THIS IS HOW THE PATH CAN BE PRINTED
//puzzlePath.printPath(2, 2);     // only 2nd element
//puzzlePath.printPath(4,7);      // 4-7
//puzzlePath.printPath();         // all
