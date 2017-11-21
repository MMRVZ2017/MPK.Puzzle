#include "path.h"

int debug = 0;

void Path::placePart(Step* myStep) {
    int8_t partType = getNextPossiblePartTypeIdx(myStep);
    int8_t orientation;
    if (partType == -1){
        goBack();
        cout << endl << "   Path::placePart: DeadEnd. Go-Back called." << endl;
        return;
    }

    while(true) {
        cout << bitset<8>(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol())) << endl;
        cout << bitset<8>(getPuzzleBox()->getBaseTypeConnections(partType)) << endl;
        //cout << unsigned(myStep->getOrientation()) << endl;
        orientation = getConstrMatrix()->check_constraints(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol()),\
        getPuzzleBox()->getBaseTypeConnections(partType), myStep->getOrientation());
        cout << signed(orientation) << endl;
        myConstr.print_matrix();
        if(orientation != -1) {
            cout << "I broke" << endl;
            debug++;
            break;
        } else {
            myStep->setPossiblePartType(partType, 2);
            partType = getNextPossiblePartTypeIdx(myStep);
            if (partType == -1){
                goBack();
                cout << endl << "   Path::placePart: DeadEnd. Go-Back called DOWN." << endl;
                abort();
            }
        }
    }

    if(incNrUsedPartType(myStep, partType)){               // if a part of this type is available
        myStep->setPossiblePartType(partType, 1);          // Part of this type is set
        myStep->setOrientation(orientation);           // Set orientation of part
        uint8_t temp = getPuzzleBox()->getBaseTypeConnections(partType);
        myConstr.rotate_part(temp, orientation);
        myConstr.set_constraints(myStep->getPosition().getRow(), myStep->getPosition().getCol(), temp);
    } else {
        cout << endl << "  Path::placePart: Something went wrong!" << endl;
    }
    myConstr.print_matrix();
    //if (debug == 5) abort();
}

int8_t Path::getNextPossiblePartTypeIdx(Step *myStep) const {
    int8_t idxActUsed = myStep->getFittingPartTypeIdx();           // Search after already used part
    for (int8_t i = int8_t(idxActUsed+1); i < NR_PART_TYPES; i++){
        if (myStep->getPossiblePartType(i) != 2 && myStep->getPossiblePartType(i) == 0 && myStep->getNrUsedPartType(i) != myBox.countType(i)) return i;
    }
    for (int8_t i = 0; i < idxActUsed; i++){                       // Search before already used part
        if (myStep->getPossiblePartType(i) != 2 && myStep->getPossiblePartType(i) == 0 && myStep->getNrUsedPartType(i) != myBox.countType(i)) return i;
    }
    return -1;                                                     // No fitting part left --> Something is wrong before this step
}

bool Path::incNrUsedPartType(Step* myStep, int8_t partType) {
    //myStep->printNrUsedPartType();
    //myBox.printTypeArr();
    //cout << signed(partType) << " " << myBox.countType(partType) << endl;
    if(myStep->getNrUsedPartType(partType) >= myBox.countType(partType)) return false;     // if no parts left (TODO: already checked in getNextPossiblePartTypeIdx)
    myStep->setNrUsedPartType(partType, uint16_t(myStep->getNrUsedPartType(partType)+1));
    return true;
}

bool Path::decNrUsedPartType(Step *myStep, int8_t partType) {
    if(myStep->getNrUsedPartType(partType) == 0) return false;                     // if already 0
    myStep->setNrUsedPartType(partType, uint16_t(myStep->getNrUsedPartType(partType)-1));
    return true;
}


void Path::printPath(uint16_t first2print, uint16_t last2print) {
    if (first2print <= last2print) {
        for (auto it = myPath.begin() + first2print; it != myPath.begin() + last2print + 1; it++) {        // auto = vector<Step>::iterator
            cout << distance(myPath.begin(), it) << ": ";               // however this is working
            cout << *it << endl;
        }
    } else {
        cout << endl << "    Path::printPath: invalid indizes!" << endl;
    }
}

void Path::printPath() {
    for (auto it = myPath.begin(); it != myPath.end(); it++) {        // auto = vector<Step>::iterator
        cout << distance(myPath.begin(), it) << ": ";               // however this is working
        cout << *it << endl;
    }
}

void Path::setPositions() {
    // Corners
    myPath[0].setPosition(0, 0);
    myPath[1].setPosition(0, NR_COLS-1);
    myPath[2].setPosition(NR_ROWS-1, NR_COLS-1);
    myPath[3].setPosition(NR_ROWS-1, 0);
        // Set constraints
    for(uint8_t i = 0; i < 4; i++) {
        for(uint8_t j = 0; j < 4; j++) {
            myPath[i].setPossiblePartType(j, 0);
        }
    }

    // Edges
    uint16_t idx = 4;
    for(uint8_t r = 1; r < NR_ROWS-1; r++) {
        myPath[idx].setPosition(r, 0);
        myPath[idx + NR_ROWS - 2].setPosition(r, NR_COLS - 1);
        idx++;
    }
    idx = uint16_t(idx+NR_ROWS-2);

    for(uint8_t c = 1; c < NR_COLS-1; c++){
        myPath[idx].setPosition(0, c);
        myPath[idx+NR_COLS-2].setPosition(NR_ROWS-1, c);
        idx++;
    }
    idx = uint16_t(idx+NR_COLS-2);

        // Set constraints
    for(uint8_t i = 4; i < idx; i++) {
        for(uint8_t j = 4; j <= 11; j++) {
            myPath[i].setPossiblePartType(j, 0);
        }
    }

    // Inners
    for(uint8_t c = 1; c < NR_COLS-1; c++){
        for(uint8_t r = 1; r < NR_ROWS-1; r++){
            myPath[idx].setPosition(r,c);
            idx++;
        }
    }

        // Set constraints
    for(uint8_t i = (NR_COLS * 2 + (NR_ROWS-2)*2); i < NR_POSITIONS; i++) {
        for(uint8_t j = 12; j < NR_PART_TYPES; j++) {
            myPath[i].setPossiblePartType(j, 0);
        }
    }
}
