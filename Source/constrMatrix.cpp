
ConstrMatrix::ConstrMatrix() {
    for(int i=0; i < NR_LINES+2; i++){
        for (int j=0; j < NR_COLUMNS+2; j++){
            if (i == 0 || i == NR_LINES +1 || j == 0 || j == NR_COLUMNS +1) matrix[i][j]=0b11111111; //no constraints
            else matrix[i][j]=0b00000000; //no constraints
        }
    }
}

void ConstrMatrix::print_matrix() const {
    for (int i = 0; i < NR_LINES + 2; i++) {
        for (int j = 0; j < NR_COLUMNS + 2; j++) {
            cout << bitset<sizeof(unsigned char)*8>(matrix[i][j]) << "  ";
        }
        cout << endl;
    }
}

uint8_t ConstrMatrix::get_constraints(int row, int col) const {
    if (row > NR_LINES || col > NR_COLUMNS) return 0b00000000; //check if out of range

    uint8_t upper = (matrix[row][col+1] << 4) & 0b11000000;
    uint8_t lower = (matrix[row+2][col+1] >> 4) & 0b00001100;
    uint8_t left = (matrix[row+1][col]  >> 4)& 0b00000011;
    uint8_t right = (matrix[row+1][col+2] << 4) & 0b00110000;

    return upper | lower | left | right;
}

void rotate_part(uint8_t &part, int steps){ // ToDo: move function to different class?
    for (int i=0; i < steps; i++){
        part = (part << 2) | (part >> 6);
        cout << "rotated part to " << bitset<sizeof(unsigned char)*8>(part) << endl; //debug only ToDO: remove cout when finished
    }
}

uint8_t check_constraints (uint8_t mask , uint8_t part, int orientation = 0){ // ToDo: move function to different class?
    for (; orientation < 3; orientation++){
        if ((mask & part) == 0b00000000) return orientation;
        else rotate_part(part,1);
    }
    return 4; //part did not fit -> return a none valid value
}
