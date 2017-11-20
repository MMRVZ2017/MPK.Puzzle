#include <iostream>
#include <bitset>


#define NR_PARTS 25
#define NR_CORNERS 4
#define NR_EDGES 12
#define NR_INNERS 9
#define NR_LINES 5
#define NR_COLUMNS 5

class ConstrMatrix{
public:
    ConstrMatrix(); //initialises matrix
    ~ConstrMatrix(){}
    uint8_t get_constraints(int row, int col) const; //returns constraints from surrounding parts for a given position
    void set_constraints(int row, int col, uint8_t connectors){ matrix[row+1][col+1]=connectors;} //writes constraints to a given position
    void remove_constraints(int row, int col){ matrix[row+1][col+1]=0b11111111;} //removes constraints for a given position
    void print_constraints(int row, int col) const {cout << "Constraints for(" << row << ";" << col <<"):" << bitset<sizeof(unsigned char)*8>(get_constraints(row,col)) << endl;}
    void print_matrix() const; //prints matrix

private:
    uint8_t matrix[NR_LINES+2][NR_COLUMNS+2]; //extra row on each side; position of first part: 1,1 TODO: replace later as vector?
};

/*********public function prototypes***********/
uint8_t check_constraints (uint8_t mask , uint8_t part, int orientation = 0);
void rotate_part(uint8_t &part, int steps);