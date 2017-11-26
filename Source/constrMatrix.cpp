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

    uint8_t temp = upper | lower | left | right;

    return temp;
}

void ConstrMatrix::set_constraints(int row, int col, uint8_t connectors) {
    matrix[row+1][col+1]=connectors;
}

void ConstrMatrix::set_constraints(PuzzlePosition actPosition, int8_t partType, int8_t orientation) {
    matrix[actPosition.getRow() + 1][actPosition.getCol() + 1] = rotate_part(partType, orientation);
}

void ConstrMatrix::remove_constraints(int row, int col) {
    matrix[row+1][col+1]=0b00000000;
}

void ConstrMatrix::print_constraints(int row, int col) const {
    cout << "Constraints for(" << row << ";" << col <<"):" << bitset<sizeof(unsigned char)*8>(get_constraints(row,col)) << endl;
}

void ConstrMatrix::rotate_part(uint8_t &part, int steps){
    for (int i=0; i < steps; i++){
        part = (part << 2) | (part >> 6);
        //cout << "rotated part to " << bitset<sizeof(unsigned char)*8>(part) << endl;
    }
}

uint8_t ConstrMatrix::rotate_part(int8_t partType, int steps){
    uint8_t part = Puzzlebox::getBaseTypeConnections(partType);
    for (int i=0; i < steps; i++){
        part = (part << 2) | (part >> 6);
        //cout << "rotated part to " << bitset<sizeof(unsigned char)*8>(part) << endl;
    }
    return part;
}

int8_t ConstrMatrix::check_constraints (PuzzlePosition actPosition, int8_t partType, int8_t orientation ){
    // Get part and constraints
    uint8_t part = Puzzlebox::getBaseTypeConnections(partType);
    uint8_t mask = get_constraints(actPosition.getRow(), actPosition.getCol());

    if(orientation != 0) rotate_part(part,orientation);

    for (; orientation < 4; orientation++){
        if ((mask & part) == 0b00000000) return orientation;
        else rotate_part(part,1);
    }
    return -1;                                                                               //part did not fit -> return a none valid value
}
