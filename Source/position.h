#include <iostream>
#include "defines.h"

using namespace std;

#ifndef POSITION_H
#define POSITION_H


/* -------------------------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------- PUZZLE_POSITION ------------------------------------------------------ */
/* -------------------------------------------------------------------------------------------------------------------------- */

// Stores x,y (row, col) Position in Puzzle
class PuzzlePosition {
public:
    void setPosRow(uint8_t rowPos){row = rowPos;}
    void setPosCol(uint8_t colPos){col = colPos;}
    uint8_t getRow() const {return row;}
    uint8_t getCol() const {return col;}

private:
    uint8_t row;
    uint8_t col;
};


#endif //POSITION_H
