#include "constrMatrix.h"

ConstrMatrix::ConstrMatrix() {
    for(int i=0; i < NR_ROWS+2; i++){
        for (int j=0; j < NR_COLS+2; j++){
            if (i == 0 || i == NR_ROWS +1 || j == 0 || j == NR_COLS +1)
                matrix[i][j]=0b11111111;                                                       //no constraints
            else matrix[i][j]=0b00000000;                                                      //no constraints
        }
    }
}

void ConstrMatrix::print_matrix() const {
    for (int i = 0; i < NR_ROWS + 2; i++) {
        for (int j = 0; j < NR_COLS + 2; j++) {
            cout << bitset<sizeof(unsigned char)*8>(matrix[i][j]) << "  ";
        }
        cout << endl;
    }
}

uint8_t ConstrMatrix::get_constraints(int row, int col) const {
    if (row > NR_ROWS || col > NR_COLS) return 0b00000000;                                  //check if out of range

    uint8_t upper = (matrix[row][col+1] << 4) & 0b11000000;
    uint8_t lower = (matrix[row+2][col+1] >> 4) & 0b00001100;
    uint8_t left = (matrix[row+1][col]  >> 4)& 0b00000011;
    uint8_t right = (matrix[row+1][col+2] << 4) & 0b00110000;

    return upper | lower | left | right;
}

void ConstrMatrix::set_constraints(int row, int col, uint8_t connectors) {
    matrix[row+1][col+1]=connectors;
}

void ConstrMatrix::remove_constraints(int row, int col) {
    matrix[row+1][col+1]=0b11111111;
}

void ConstrMatrix::print_constraints(int row, int col) const {
    cout << "Constraints for(" << row << ";" << col <<"):" << bitset<sizeof(unsigned char)*8>(get_constraints(row,col)) << endl;
}


void rotate_part(uint8_t &part, int steps){                                                 // ToDo: move function to different class?
    for (int i=0; i < steps; i++){
        part = (part << 2) | (part >> 6);
        cout << "rotated part to " << bitset<sizeof(unsigned char)*8>(part) << endl;        //debug only ToDo: remove cout when finished
    }
}

uint8_t check_constraints (uint8_t mask , uint8_t part, int orientation = 0){               // ToDo: move function to different class?
    for (; orientation < 3; orientation++){
        if ((mask & part) == 0b00000000) return orientation;
        else rotate_part(part,1);
    }
    return 4;                                                                               //part did not fit -> return a none valid value
}
