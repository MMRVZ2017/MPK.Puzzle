#include "path.h"

// VARIABLES FOR DEBUGGING
int numInc[18] = {0};
int numDec[18] = {0};
int failed1 = 0;
int failed2 = 0;
int failed3 = 0;

int32_t Path::solvePuzzle(int32_t limit) {

    // DEBUG
    int32_t numTries = 0;
    int tryOrder[10000] = {0};      // If used, make sure you don't write out of the array!
    int orderCounter = 0;

    // *** ESSENTIAL
    bool flag = false;
    uint16_t goBackIdx = 0;

    for (uint16_t i = 0; i < NR_POSITIONS; i++){
        // DEBUG
        numTries++;
        if(PRINT_MESSAGES) cout << "PLACING PART " << unsigned(i) <<endl;
        if(i > furthestStep) furthestStep = i;
        //tryOrder[orderCounter] = i; orderCounter ++;

        // Place a part
        flag = placePart(i, 0);
        // If part does not fit
        if (!flag) {
            // then go back as long as you don't find a fitting part
            for (goBackIdx = uint16_t(i); !flag; goBackIdx--) {

                // DEBUG
                numGoBack++;
                if(PRINT_MESSAGES) cout << "GO BACK FROM PART" << unsigned(goBackIdx) << endl;

                // Put away the part and the constraints
                goBack(goBackIdx);

                // DEBUG
                if(PRINT_MESSAGES) cout << "PLACING " << goBackIdx-1 << " Was Part: " << signed(getStep(int16_t(goBackIdx-1))->getFittingPartTypeIdx()) << endl;
                if(PRINT_MESSAGES) getStep(uint16_t(goBackIdx-1))->printPossiblePartTypeArr();
                //tryOrder[orderCounter] = goBackIdx-1; orderCounter ++;

                // And try another part
                flag = placePart(uint16_t(goBackIdx-1), 1);
            }
            // When a fitting part is found, go ahead with outer loop
            i = goBackIdx;
        }
        if (numTries >= limit){
            break;
        }
    }   // TODO: Catch wrong puzzles, Catch goBack to index 0

    // DEBUG
    //for (int i = 0; i < orderCounter; i++) cout << tryOrder[i] << endl;

    return numTries;
}

void Path::goBack(uint16_t actStepIdx){     // Go Back from Index actStepIdx
    Step *myStep = getStep(actStepIdx);
    int8_t actPartType = myStep->getFittingPartTypeIdx();

    // DEBUG
    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/
    if(PRINT_MESSAGES) cout << "Call reset for idx " << unsigned(actStepIdx) << endl;

    // Delete everything from this step
    if (actPartType != -1){                                                 // A part is already at this position -> put it away
        if (!decNrUsedPartType(actPartType)) failed1++;                     // Put it back into the box
        myConstr.remove_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol());     // TODO: That has to be shorter
    }
    myStep->resetPossiblePartTypes();                                       // Reset part types -> Every part is possible again
    myStep->setOrientation(0);                                              // Reset orientation

    // DEBUG
    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/
}

bool Path::placePart(int16_t idx, int8_t state) {           // Idx of part to place; state == 0: just place it; state == 1: we are going back
    Step* myStep = getStep(idx);
    int8_t partType = -1;
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

    // When backtracking and there was a part: delete it
    if (state == 1 || actPartType != -1) {
        if (!decNrUsedPartType(actPartType)) failed2++;         // Put it back into the box
        myStep->setPossiblePartType(actPartType, 2);            // This part is not good in this position TODO: Rotate first in this case?
        myStep->setOrientation(0);                              // Reset orientation
        myConstr.remove_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol());     // Reset constraints
    }

    // Try until one part fits:
    while(true) {
        // Get correct orientation for part
        orientation = myConstr.check_constraints(myStep->getPosition(), partType, myStep->getOrientation());

        // Check if part fits
        if (orientation == -1) { // If the part does not fit
            if(PRINT_MESSAGES) cout << "Part doesn't fit in! " << "(Type: " << signed(partType) << ")" << endl;
            myStep->setPossiblePartType(partType, 2);
            myStep->setOrientation(0);
            // get another part if and jump back to orientation-check
            partType = getNextPossiblePartTypeIdx(myStep);
            // If there is no other part left: go back
            if (partType == -1) {
                if(PRINT_MESSAGES) cout << "Keine Grundform mehr übrig, DETAIL -> Go Back" << endl;
                return false;
            }
        } else { // If the part fits
            if(PRINT_MESSAGES) cout << "Part fits in! " << "(Type: " << signed(partType) << "," << signed(orientation) << ")" << endl;
            break;
        }
    }

    // Set fitting part for the step
    if (incNrUsedPartType(partType)) {                      // if a part of this type is available (Should be true because checked before)
        myStep->setPossiblePartType(partType, 1);           // Set part of this type
        myStep->setOrientation(orientation);                // Set orientation of part
        myConstr.set_constraints(myStep->getPosition(), partType, orientation);     // Set constraints
    } else {
        // Something gröber went wrong -> Show message all the time
        failed3++;
        cout << "Path::placePart: Something went wrong!" << signed(partType) << endl << endl;
        return false;
    }
    return true;
}

int8_t Path::getNextPossiblePartTypeIdx(Step *myStep) const {
    // Do random stuff
    random_device rd;               // obtain a random number from hardware
    mt19937 eng(rd());              // seed the generator

    int8_t possibilities[NR_PART_TYPES] = {-1};        // To store the possibilities
    int8_t numPos = 0;                                 // To store the number of possibilities (for random-generator)

    for(int8_t i = 0; i < NR_PART_TYPES; i++){
        // Check if this part is possible
        if(myStep->getPossiblePartType(i) == 0 && Puzzlebox::countType(i) > 0 && Step::getNrUsedPartType(i) < Puzzlebox::countType(i)) {
            possibilities[numPos] = i;
            numPos++;
        }
    }
    // If there was no possible part type
    if(numPos == 0) return -1;

    // If there was only 1 possible part type
    if(numPos == 1) return possibilities[0];

    // If there were more than 1 possible parts
    uniform_int_distribution<> distr(1, numPos);            // Get a random part type
    return possibilities[distr(eng)-1];

    /*
    // Searching for next part without random
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

bool Path::incNrUsedPartType(int8_t partType) {
    // Debug
    numInc[partType]++;

    // Get the actual number of parts of this type
    int16_t nrUsedPartType = Step::getNrUsedPartType(partType);
    // If there is a part left
    if(Puzzlebox::countType(partType) > 0 && nrUsedPartType < Puzzlebox::countType(partType)) {
        nrUsedPartType = int16_t(nrUsedPartType + 1);
        Step::setNrUsedPartType(partType, nrUsedPartType);
        return true;
    } else {
        return false;     // if no parts left (TODO: already checked in getNextPossiblePartTypeIdx)
    }
}

bool Path::decNrUsedPartType(int8_t partType) {
    // Debug
    numDec[partType]++;

    // If there are no parts of this type
    if(Step::getNrUsedPartType(partType) == 0){
        return false;
    } else {
        int16_t nrUsedPartType = Step::getNrUsedPartType(partType);
        nrUsedPartType = int16_t(nrUsedPartType - 1);
        Step::setNrUsedPartType(partType, nrUsedPartType);
        return true;
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
    for(uint16_t i = (NR_COLS * 2 + (NR_ROWS-2)*2); i < NR_POSITIONS; i++) {
        for(uint8_t j = 12; j < NR_PART_TYPES; j++) {
            myPath[i].setPossiblePartType(j, 0);
        }
    }
}

void Path::count_printNumTypesUsed() {
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
    for(int16_t r = 0; r < NR_ROWS; r++){
        for(int16_t c = 0; c < NR_COLS; c++){
            myConstr.remove_constraints(r, c);
        }
    }
    for(int16_t i = 0; i < NR_POSITIONS; i++){
        getStep(i)->setOrientation(0);
        getStep(i)->resetPossiblePartTypes();
    }
    for(int8_t i = 0; i < NR_PARTS; i++){
        Step::setNrUsedPartType(i, 0);
    }
    numTries = 0;
    numGoBack = 0;
    furthestStep = 0;
}

void Path::printPath(uint16_t first2print, uint16_t last2print) {
    if (first2print <= last2print) {
        for (int16_t i = first2print; i <= last2print; i++){
            cout << i << ": " << "[" << unsigned(getStep(i)->getPosition().getRow()) << "]" << "[" << unsigned(getStep(i)->getPosition().getCol()) << "]: " \
                 << signed(getStep(i)->getFittingPartTypeIdx()) << "  Ori: " << signed(getStep(i)->getOrientation()) << " | ";
            getStep(i)->printPossiblePartTypeArr();
        }
    } else {
        cout << endl << "    Path::printPath: invalid indizes!" << endl;
    }
}

void Path::printPath() {
    for (int16_t i = 0; i < NR_POSITIONS; i++){
        cout << i << ": " << "[" << unsigned(getStep(i)->getPosition().getRow()) << "]" << "[" << unsigned(getStep(i)->getPosition().getCol()) << "]: " \
                 << signed(getStep(i)->getFittingPartTypeIdx()) << "  Ori: " << signed(getStep(i)->getOrientation()) << " | ";
        getStep(i)->printPossiblePartTypeArr();
    }
}

void Path::printDebug() {
    cout << "NUM INC: ";
    for(int i = 0; i < 18; i++){
        cout << numInc[i] << " ";
    }
    cout << endl;
    cout << "NUM DEC: ";
    for(int i = 0; i < 18; i++){
        cout << numDec[i] << " ";
    }
    cout << endl;
    cout << "Failed 1: " << failed1 << endl;
    cout << "Failed 2: " << failed2 << endl;
    cout << "Failed 3: " << failed3 << endl;
}
