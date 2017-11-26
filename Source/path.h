#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include "defines.h"
#include "puzzleBox.h"
#include "step.h"
#include "constrMatrix.h"

using namespace std;

#ifndef PATH_H
#define PATH_H

/*--------------------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------- PATH ---------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

// Stores vector of Steps which represents the search-tree and the PuzzleBox where the parts are from
class Path{
public:
    Path(vector <Part> *part_array): myPath(NR_POSITIONS), numTries(0), numGoBack(0), furthestStep(0){myBox.sortParts(part_array);}
    void setPositions();                                              // Sets for each step the x,y and position where the part will be placed and the resulting constraints (corners -> edges -> inners in columns)

    Puzzlebox* getPuzzleBox() {return &myBox;}
    Step* getStep(int16_t idx) {return &myPath[idx];}
    vector <Step>* getPath() {return &myPath;}
    ConstrMatrix* getConstrMatrix() {return &myConstr;}

    int32_t solvePuzzle(int32_t limit);                               // Calls functions for each step to solve the puzzle
    bool placePart(int16_t idx, int8_t state);                        // Sets the next possible part-type with orientation 0 for myStep
    int8_t getNextPossiblePartTypeIdx(Step* myStep) const;            // Returns the partType (0-15) which could be placed in the PuzzlePosition (next with ==0 in possiblePartType)
    bool incNrUsedPartType(int8_t partType);                          // Increments nrUsedPartType for partType (0-15) (call if one part of this type is placed). Returns FALSE if not possible (all parts of this category used)
    bool decNrUsedPartType(int8_t partType);                          // Decrements nrUsedPartType for partType (0-15) (call if one part is put out of this position). Returns FALSE if not possible (something is wrong)
    void goBack(uint16_t actStepIdx);                                 // Resets all constraints and displaced the part for actStepIdx
    //void exportSolution() const;                                    // TODO: To be implemented
    void count_printNumTypesUsed();                                   // Counts and prints the actual placed number of each part
    void resetPath();                                                 // Resets all features of puzzlePath to try once again

    void printPath(uint16_t first2print, uint16_t last2print);        // Prints content of Step from first2print to last2print (first=last: print only one step)
    void printPath();                                                 // Prints complete Path
    void printDebug();                                                // Prints debug-information

    // Variables for debugging
    int32_t numTries;
    int32_t numGoBack;
    int16_t furthestStep;
private:
    vector <Step> myPath;
    Puzzlebox myBox;
    ConstrMatrix myConstr;
};

#endif //PATH_H
