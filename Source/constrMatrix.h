#include <iostream>
#include <bitset>
#include "defines.h"
#include "puzzleBox.h"
#include "position.h"

#ifndef CONSTRMATRIX_H
#define CONSTRMATRIX_H

using namespace std;

/* -------------------------------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------- ConstrMatrix ----------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

class ConstrMatrix{
public:
    ConstrMatrix();                                                     //initialises matrix
    uint8_t get_constraints(int row, int col) const;                    //returns constraints from surrounding parts for a given position
    void set_constraints(int row, int col, uint8_t connectors);         //writes constraints to a given position
    void set_constraints(PuzzlePosition actPosition, int8_t partType, int8_t orientation);         //writes constraints to a given position
    void remove_constraints(int row, int col);                          //removes constraints for a given position
    void print_constraints(int row, int col) const;                     //prints constraints for a given position
    void print_matrix() const;                                          //prints matrix including border lines
    int8_t check_constraints (PuzzlePosition actPosition, int8_t partType, int8_t orientation = 0);     // Returns the orientation, in which partType fits at actPosition
    void rotate_part(uint8_t &part, int steps);                         // Rotates part (ACTUAL GIVE PART!)
    uint8_t rotate_part(int8_t partType, int steps);                    // Overloaded for not really rotating a part but getting rotated constraints

private:
    uint8_t matrix[NR_ROWS+2][NR_COLS+2];                               //extra row on each side; position of first part: 1,1 ToDo: replace later as vector?
};


/* ------------------------------------------------------ Public functions ------------------------------------------------- */


#endif //CONSTRMATRIX_H