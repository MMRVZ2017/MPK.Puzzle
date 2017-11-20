#include <iostream>
#include <bitset>
#include "defines.h"

#ifndef CONSTRMATRIX_H
#define CONSTRMATRIX_H

using namespace std;

/* -------------------------------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------- ConstrMatrix ----------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

class ConstrMatrix{
public:
    ConstrMatrix();                                                     //initialises matrix
    ~ConstrMatrix(){}
    uint8_t get_constraints(int row, int col) const;                    //returns constraints from surrounding parts for a given position
    void set_constraints(int row, int col, uint8_t connectors)          //writes constraints to a given position
    void remove_constraints(int row, int col)                           //removes constraints for a given position
    void print_constraints(int row, int col) const                      //prints constraints for a given position
    void print_matrix() const;                                          //prints matrix including border lines

private:
    uint8_t matrix[NR_ROWS+2][NR_COLS+2];                               //extra row on each side; position of first part: 1,1 ToDo: replace later as vector?
};


/* ------------------------------------------------------ Public functions ------------------------------------------------- */

uint8_t check_constraints (uint8_t mask , uint8_t part, int orientation = 0);   // may be includes in a different class later
void rotate_part(uint8_t &part, int steps);                                     // may be includes in a different class later

#endif //CONSTRMATRIX_H