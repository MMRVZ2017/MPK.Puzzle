#include "step.h"

int16_t Step::nrUsedPartType[NR_PART_TYPES] = {0};

Step::Step() {
    for(int i = 0; i < NR_PART_TYPES; i++) possiblePartType[i] = -1;        // Set impossible for every position at init
    orientation = 0;
    position.setPosCol(NR_ROWS+1);
    position.setPosRow(NR_COLS+1);
}

ostream &operator<<(ostream &ostr, const Step &myStep) {
    myStep.printPossiblePartTypeArr();
    return  ostr << "Ori.:" << unsigned(myStep.getOrientation()) << \
            " | Pos. (R, C):" << unsigned(myStep.getPosition().getRow()) << \
            ", " << unsigned(myStep.getPosition().getCol());
}

uint8_t Step::rotate() {
    if(getOrientation() >= 3) return 4;
    else {
        int newOrientation = getOrientation()+1;
        setOrientation(uint8_t(newOrientation));
        return uint8_t(newOrientation);
    }
}

int8_t Step::getFittingPartTypeIdx() const {
    for(uint8_t i = 0; i < NR_PART_TYPES; i++){
        if(possiblePartType[i] == 1) return i;
    }
    return -1;
}

void Step::printPossiblePartTypeArr() const {
    cout << "PosTypes: ";
    for(int i = 0; i < NR_PART_TYPES; i++){
        cout << signed(possiblePartType[i]) << " ";
    }
    cout << endl;
}

void Step::printNrUsedPartType(){
    cout << "Types Used:      ";
    for(int i = 0; i < NR_PART_TYPES; i++){
        cout << unsigned(nrUsedPartType[i]) << " ";
    }
    cout << endl;
}

