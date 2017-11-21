#include <iostream>
#include <vector>
#include "defines.h"
#include "puzzleBox.h"
#include "step.h"
#include "constrMatrix.h"

using namespace std;

#ifndef PATH_H
#define PATH_H

/* -------------------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------- PATH ---------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

// Stores vector of Steps which represents the search-tree and the PuzzleBox where the parts are from
class Path{
public:
    Path(): myPath(NR_POSITIONS){}
    void setPositions();                                              // Sets for each step the x,y and position where the part will be placed and the resulting constraints (corners -> edges -> inners in columns)

    Puzzlebox* getPuzzleBox() {return &myBox;}
    Step* getStep(uint16_t idx) {return &myPath[idx];}
    vector <Step>* getPath() {return &myPath;}
    ConstrMatrix* getConstrMatrix() {return &myConstr;}

    bool placePart(Step* myStep, int8_t state);                       // Sets the next possible part-type with orientation 0 for myStep
    int8_t getNextPossiblePartTypeIdx(Step* myStep) const;            // Returns the partType (0-15) which could be placed in the PuzzlePosition (next with ==0 in possiblePartType)
    bool incNrUsedPartType(Step* myStep, int8_t partType);            // Increments nrUsedPartType for partType (0-15) (call if one part of this type is placed). Returns FALSE if not possible (all parts of this category used)
    bool decNrUsedPartType(Step* myStep, int8_t partType);            // Decrements nrUsedPartType for partType (0-15) (call if one part is put out of this position). Returns FALSE if not possible (something is wrong)
    uint16_t goBack(uint16_t actStepIdx);
    void printSolution();
    //void exportSolution() const;

    void printPath(uint16_t first2print, uint16_t last2print);        // Prints content of Step from first2print to last2print (first=last: print only one step)
    void printPath();                                                 // Prints complete Path

private:
    vector <Step> myPath;
    Puzzlebox myBox;
    ConstrMatrix myConstr;
};

#endif //PATH_H
