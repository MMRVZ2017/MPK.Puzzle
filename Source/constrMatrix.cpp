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

void ConstrMatrix::remove_constraints(int row, int col) {
    matrix[row+1][col+1]=0b00000000;//0b11111111;
}

void ConstrMatrix::print_constraints(int row, int col) const {
    cout << "Constraints for(" << row << ";" << col <<"):" << bitset<sizeof(unsigned char)*8>(get_constraints(row,col)) << endl;
}


void ConstrMatrix::rotate_part(uint8_t &part, int steps){                                                 // ToDo: move function to different class?
    for (int i=0; i < steps; i++){
        part = (part << 2) | (part >> 6);
        //cout << "rotated part to " << bitset<sizeof(unsigned char)*8>(part) << endl;        //debug only ToDo: remove cout when finished
    }
}

int8_t ConstrMatrix::check_constraints (uint8_t mask , uint8_t part, int8_t orientation ){               // ToDo: move function to different class?

    if(orientation != 0)
    {
        rotate_part(part,orientation);
    }


    for (; orientation < 4; orientation++){

        /* WEIDI
        int containsEdge = 0;
        int invalidOrientation = 0;
        int edge1 = 0;
        int edge2 = 0;
        int edge3 = 0;
        int edge4 = 0;


        if ((part & 0b11000000) == 0b00000000) // Edge upper
        {
            containsEdge = 1;
            if((mask & 0b11000000) == 0b11000000)
            {
                edge1 = 0;
            }
            else
            {
                edge1 = 1;
            }
        }
        if ((part & 0b00110000) == 0b00000000) // Edge right
        {
            containsEdge = 1;
            if((mask & 0b00110000) == 0b00110000)
            {
                edge2 = 0;
            }
            else
            {
                edge2 = 1;
            }
        }
        if ((part & 0b00001100) == 0b00000000) // Edge lower
        {
            containsEdge = 1;
            if((mask & 0b00001100) == 0b00001100)
            {
                edge3 = 0;
            }
            else
            {
                edge3 = 1;
            }
        }
        if ((part & 0b00000011) == 0b00000000) // Edge left
        {
            containsEdge = 1;
            if((mask & 0b00000011) == 0b00000011)
            {
                edge4 = 0;
            }
            else
            {
                edge4 = 1;
            }
        }

        if (edge1 == 1 || edge2 == 1 || edge3 == 1 || edge4 == 1)
        {
            rotate_part(part,1);
        }
        else
        {
            if ((mask & part) == 0b00000000) return orientation;
            else rotate_part(part,1);
        }




        // cout << bitset<8>(mask) << endl;
        // cout << bitset<8>(part) << endl;
        */


        if ((mask & part) == 0b00000000) return orientation;
        else rotate_part(part,1);


    }
    return -1;                                                                               //part did not fit -> return a none valid value
}
