#include "path.h"

int debug = 0;

uint16_t Path::goBack(uint16_t actStepIdx){
    Step *myStep = getStep(actStepIdx);
    Step *lastStep = getStep(uint16_t(actStepIdx-1));
    int8_t actPartType = myStep->getFittingPartTypeIdx();

    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/

    if (actPartType != -1){
        decNrUsedPartType(myStep, actPartType);
    }
    myStep->resetPossiblePartTypes();
    myConstr.remove_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol());

    /*myStep->printNrUsedPartType();
    myBox.printTypeArr();
    cout << "Displaying Step " << actStepIdx << endl;
    myStep->printPossiblePartTypeArr();
    cout << "Displaying Step " << actStepIdx-1 << endl;
    lastStep->printPossiblePartTypeArr();*/

    return 0;
}

bool Path::placePart(Step* myStep, int8_t state) {
    int8_t partType = NR_PART_TYPES+1;              // invalid
    int8_t orientation;

    // TODO: Check for edges --> Wenn einmal gesetzt nicht umorientieren oder über Constraints am Rand abfangen

    if (state == 1){                                        // Wenn Back-Tracking
        partType = myStep->getFittingPartTypeIdx();         // -1: Es wurde noch keiner gesetzt
        if (partType != -1) myStep->setOrientation(int8_t(myStep->getOrientation()+1));     // Orientierung ändern für nächsten Versuch
        decNrUsedPartType(myStep, myStep->getFittingPartTypeIdx());             // Teil "wieder zurücklegen" (weil nachher wieder gesetzt)
    }
    if (state == 0 || partType == -1){                      // No Back-Tracking (Anderer Zweig weil bei Backtracking zuerst umorienterien) oder noch kein passender Teil gesetzt gewesen
        partType = getNextPossiblePartTypeIdx(myStep);      // Einen möglichen Part suchen
        if (partType == -1){                                // Keine Grundform mehr übrig wo 0 drinnen steht -> goBack
            cout << "Keine Grundform mehr übrig, GROB -> Go Back" << endl;
            return false;
        }
    }

    while(true) {
        //cout << bitset<8>(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol())) << endl;
        //cout << bitset<8>(getPuzzleBox()->getBaseTypeConnections(partType)) << endl;
        //cout << unsigned(myStep->getOrientation()) << endl;

        orientation = getConstrMatrix()->check_constraints(getConstrMatrix()->get_constraints(myStep->getPosition().getRow(),myStep->getPosition().getCol()),\
        getPuzzleBox()->getBaseTypeConnections(partType), myStep->getOrientation());
        //cout << signed(orientation) << endl;J
        if(orientation != -1) {
            cout << "Part fits in! " << "(Type: " << signed(partType) << ")" << endl;
            break;
        } else {
            cout << "Part doesn't fit in! " << "(Type: " <<  signed(partType) <<  ")" << endl;
            myStep->setPossiblePartType(partType, 2);
            myStep->setOrientation(0);
            partType = getNextPossiblePartTypeIdx(myStep);
            if (partType == -1){
                cout << "Keine Grundform mehr übrig, DETAIL -> Go Back" << endl;
                return false;
            }
        }
    }

    if(incNrUsedPartType(myStep, partType)){               // if a part of this type is available
        myStep->setPossiblePartType(partType, 1);          // Part of this type is set
        myStep->setOrientation(orientation);                // Set orientation of part
        uint8_t temp = getPuzzleBox()->getBaseTypeConnections(partType);
        myConstr.rotate_part(temp, orientation);
        myConstr.set_constraints(myStep->getPosition().getRow(), myStep->getPosition().getCol(), temp);
    } else {
        cout << endl << "Path::placePart: Something went wrong!" << signed(partType) << endl;
    }
    //myConstr.print_matrix();
    return true;
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
