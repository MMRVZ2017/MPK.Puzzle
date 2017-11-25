#include "path.h"
int numInc[18] = {0};
int numDec[18] = {0};
int failed1 = 0;
int failed2 = 0;
int failed3 = 0;
int debug = 0;

uint16_t Path::goBack(uint16_t actStepIdx){     // Go Back from Index actStepIdx
    Step *myStep = getStep(actStepIdx);
    int8_t actPartType = myStep->getFittingPartTypeIdx();

    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/

    if(PRINT_MESSAGES) cout << "Call reset for idx " << unsigned(actStepIdx) << endl;
    if (actPartType != -1){                                                 // A part is already at this position -> put it away
        if (decNrUsedPartType(myStep, actPartType) == false) failed1++;
        myStep->setPossiblePartType(actPartType, 0);
        myConstr.remove_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol());
    }
    myStep->resetPossiblePartTypes();                                       // Reset constraints (possible part types) and orientation anyway
    myStep->setOrientation(0);

    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/

    return 0;
}

bool Path::placePart(Step* myStep, int8_t state) {
    int8_t partType = -1;              // invalid
    int8_t orientation;
    int8_t actPartType = 0;

    // Look if there is already a part and store this information
    actPartType = myStep->getFittingPartTypeIdx();

    // Get a possible part
    partType = getNextPossiblePartTypeIdx(myStep);
    if (partType == -1){                                // No possible part left
        if(PRINT_MESSAGES) cout << "Keine Grundform mehr übrig, GROB -> Go Back" << endl;
        return false;
    }

    // When backtracking and there was a part: delete it        // TODO: REALLY WEIRD STUFF HAPPENS HERE!
    if (state == 1 || actPartType != -1) {
        //myStep->printPossiblePartTypeArr();
        //cout << signed(actPartType) << endl;
        if (decNrUsedPartType(myStep, actPartType) == false) failed2++;
        myStep->setPossiblePartType(actPartType, 0);
        myStep->setOrientation(0);
        myConstr.remove_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol());
        myStep->setPossiblePartType(actPartType, 2);
    }

    // Try until one part fits:
    while(true) {
        //cout << bitset<8>(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol())) << endl;
        //cout << bitset<8>(getPuzzleBox()->getBaseTypeConnections(partType)) << endl;
        //cout << unsigned(myStep->getOrientation()) << endl;

        // Get correct orientation for part
        orientation = getConstrMatrix()->check_constraints(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(), myStep->getPosition().getCol()), \
        getPuzzleBox()->getBaseTypeConnections(partType), myStep->getOrientation());
        //cout << signed(orientation) << endl;

        // Check if part fits
        if (orientation == -1) {
            if(PRINT_MESSAGES) cout << "Part doesn't fit in! " << "(Type: " << signed(partType) << ")" << endl;
            myStep->setPossiblePartType(partType, 2);
            myStep->setOrientation(0);
            // Get other part if it does not fit and jump back to orientation-check
            partType = getNextPossiblePartTypeIdx(myStep);
            if (partType == -1) {
                if(PRINT_MESSAGES) cout << "Keine Grundform mehr übrig, DETAIL -> Go Back" << endl;
                return false;
            }
        } else { //Part fits
            if(PRINT_MESSAGES) cout << "Part fits in! " << "(Type: " << signed(partType) << "," << signed(orientation) << ")" << endl;
            break;
        }
    }

    if (incNrUsedPartType(myStep, partType)) {              // if a part of this type is available
        myStep->setPossiblePartType(partType, 1);           // Part of this type is set
        myStep->setOrientation(orientation);                // Set orientation of part
        uint8_t temp = getPuzzleBox()->getBaseTypeConnections(partType);
        myConstr.rotate_part(temp, orientation);
        myConstr.set_constraints(myStep->getPosition().getRow(), myStep->getPosition().getCol(), temp);
    } else {
        failed3++;
        if(PRINT_MESSAGES) cout << "Path::placePart: Something went wrong!" << signed(partType) << endl << endl;
        return false;
    }
    return true;
}

int8_t Path::getNextPossiblePartTypeIdx(Step *myStep) const {
    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator

    int8_t possibilities[NR_PART_TYPES] = {-1};
    int8_t pos = 0;

    for(int8_t i = 0; i < NR_PART_TYPES; i++){
        if(myStep->getPossiblePartType(i) == 0 && Puzzlebox::countType(i) > 0 && Step::getNrUsedPartType(i) < Puzzlebox::countType(i)) {
            possibilities[pos] = i;             // Save it, if position is possible
            pos++;
        }
    }
    if(pos == 0) return -1;
    if(pos == 1) return possibilities[0];
    uniform_int_distribution<> distr(1, pos);   // Get a random position
    return possibilities[distr(eng)-1];
    /*
    int8_t idxActUsed = myStep->getFittingPartTypeIdx();
    if (idxActUsed != (NR_PART_TYPES-1)) {
        for (auto i = int8_t(idxActUsed + 1); i < NR_PART_TYPES; i++) {  // Search after already used part
            if (myStep->getPossiblePartType(i) == 0 && Puzzlebox::countType(i) > 0 && Step::getNrUsedPartType(i) < Puzzlebox::countType(i)) return i;
        }
    }
    for (int8_t i = 0; i < idxActUsed; i++){                       // Search before already used part
        if (myStep->getPossiblePartType(i) == 0 && Puzzlebox::countType(i) > 0 && Step::getNrUsedPartType(i) < Puzzlebox::countType(i)) return i;
    }
    return -1;         */                                            // No fitting part left --> Something is wrong before this step
}

bool Path::incNrUsedPartType(Step* myStep, int8_t partType) {
    //myStep->printNrUsedPartType();
    //myBox.printTypeArr();
    //cout << signed(partType) << " " << myBox.countType(partType) << endl;
    numInc[partType]++;
    int16_t nrUsedPartType = Step::getNrUsedPartType(partType);
    if(Puzzlebox::countType(partType) > 0 && Step::getNrUsedPartType(partType) < Puzzlebox::countType(partType)) {
        nrUsedPartType = int16_t(nrUsedPartType + 1);
        Step::setNrUsedPartType(partType, nrUsedPartType);
        return true;
    } else {
        return false;     // if no parts left (TODO: already checked in getNextPossiblePartTypeIdx)
    }
}

bool Path::decNrUsedPartType(Step *myStep, int8_t partType) {
    numDec[partType]++;
    if(Step::getNrUsedPartType(partType) == 0){
        return false;                            // if already 0
    } else {
        int16_t nrUsedPartType = Step::getNrUsedPartType(partType);
        nrUsedPartType = int16_t(nrUsedPartType - 1);
        Step::setNrUsedPartType(partType, nrUsedPartType);
        return true;
    }
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
    cout << "NUM INC: ";
    for (int i = 0; i < 18; i++){
        cout << numInc[i] << " ";
    }
    cout << endl;
    cout << "NUM DEC: ";
    for (int i = 0; i < 18; i++){
        cout << numDec[i] << " ";
    }
    cout << endl;
    cout << "Failed 1: " << failed1 << endl;
    cout << "Failed 2: " << failed2 << endl;
    cout << "Failed 3: " << failed3 << endl;
    countNumTypesUsed();
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
    for(uint16_t i = (NR_COLS * 2 + (NR_ROWS-2)*2); i < NR_POSITIONS; i++) {
        for(uint8_t j = 12; j < NR_PART_TYPES; j++) {
            myPath[i].setPossiblePartType(j, 0);
        }
    }
}

void Path::printSolution() {
    for (int i = 0; i < NR_POSITIONS; i++){
            cout << i << ": " << unsigned(getStep(i)->getPosition().getRow()) << " " << unsigned(getStep(i)->getPosition().getCol()) << ": " \
                 << signed(getStep(i)->getFittingPartTypeIdx()) << "  Ori: " << signed(getStep(i)->getOrientation()) << endl;
    }
}

void Path::countNumTypesUsed() {
    int numUsed[18] = {0};
    for(int16_t i = 0; i < NR_POSITIONS; i++){
        numUsed[getStep(i)->getFittingPartTypeIdx()]++;
    }
    cout << "Really used:     ";
    for(int i = 0; i < 18; i++){
        cout << numUsed[i] << " ";
    }
    cout << endl;
}

void Path::resetPath() {
    for(int r = 0; r < NR_ROWS; r++){
        for(int c = 0; c < NR_COLS; c++){
            myConstr.remove_constraints(r, c);
        }
    }
    for(int i = 0; i < NR_POSITIONS; i++){
        getStep(i)->setOrientation(0);
        getStep(i)->resetPossiblePartTypes();
    }
    for(int i = 0; i < NR_PARTS; i++){
        Step::setNrUsedPartType(i, 0);
    }
}
