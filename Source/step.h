#include <iostream>
#include <vector>
#include "defines.h"
#include "puzzleBox.h"
#include "position.h"

using namespace std;

#ifndef STEP_H
#define STEP_H
/* -------------------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------- STEP ---------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

// Represents one Spielzug where a part is placed on a PuzzlePosition. Also all the other possibilities for this PuzzlePosition are stored.
class Step{
    friend ostream& operator<<(ostream& ostr, const Step& myStep);              // print a step (Position and Orientation)
public:
    Step();

    void setPosition(uint8_t row, uint8_t col) {position.setPosRow(row); position.setPosCol(col);}
    PuzzlePosition getPosition() const {return position;}
    void setOrientation(int8_t newOrientation) {orientation = newOrientation;}
    int8_t getOrientation() const {return orientation;}
    void setPossiblePartType(int8_t partType, int8_t state) {possiblePartType[partType] = state;}
    void resetPossiblePartTypes();
    int16_t getPossiblePartType(int8_t partType) const {return possiblePartType[partType];}
    static void setNrUsedPartType(int8_t partType, int16_t num){nrUsedPartType[partType] = num;}
    static int16_t getNrUsedPartType(int8_t partType) {return nrUsedPartType[partType];}

    uint8_t rotate();                               // Changes rotation for part in Step; returns new orientation, 4 if part is already in position 3
    int8_t getFittingPartTypeIdx() const;           // Returns the partType (0-15) which is actually placed in the PuzzlePosition (=1 in possiblePartType)

    void printPossiblePartTypeArr() const;
    static void printNrUsedPartType();

private:
    PuzzlePosition position{};                      // Where the part of this step is placed in the puzzle
    int16_t possiblePartType[NR_PART_TYPES];        // Stores if parts of type 0-15 fit into this PuzzlePosition (1 if fit, 0 if not tried, -1 if not fit)
    int8_t orientation;                             // How the part must be rotated to fit
    static int16_t nrUsedPartType[NR_PART_TYPES];   // Stores, how many parts of type 0-15 are already used
};

#endif //STEP_H
