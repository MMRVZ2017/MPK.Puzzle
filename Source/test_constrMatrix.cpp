#include <iostream>
#include <vector>
#include "path.h"
#include "constrMatrix.h"

using namespace std;

int main() {

    ConstrMatrix myfirstMatrix;
    myfirstMatrix.print_matrix();

    cout << endl;
    myfirstMatrix.print_constraints(0,0);
    myfirstMatrix.print_constraints(1,0);
    myfirstMatrix.print_constraints(1,1);
    myfirstMatrix.print_constraints(0,NR_COLS-1);
    myfirstMatrix.print_constraints(NR_ROWS-1,0);
    myfirstMatrix.print_constraints(NR_ROWS-1,NR_COLS-1);
    cout << endl;

    myfirstMatrix.set_constraints(0,1,0b00110111);
    myfirstMatrix.set_constraints(2,1,0b10111111);
    myfirstMatrix.set_constraints(1,0,0b11011100);
    myfirstMatrix.set_constraints(1,2,0b11111110);

    myfirstMatrix.print_matrix();
    myfirstMatrix.print_constraints(1,1);

    uint8_t testPart[5]{
            0b00000000,
            0b11111111,
            0b01101001,
            0b00011011,
            0b10010110,
    };
    for (int i = 0; i < 5 ; i++) {
        cout << endl << "Checking position (1,1) with Part " << bitset<sizeof(unsigned char)*8>(testPart[i])<< endl;
        cout << "Result:" << unsigned(check_constraints(myfirstMatrix.get_constraints(1, 1), testPart[i], 0)) << endl;

    }

    return 1;
}