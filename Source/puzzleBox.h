#include <iostream>
#include <vector>
#include <bitset>
#include "defines.h"

#ifndef PUZZLEBOX_H
#define PUZZLEBOX_H

using namespace std;

/* -------------------------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------- PART --------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

class Part
{
public:
    Part(): connections(0){}
    uint8_t getConnections() const {return connections;}
    void setConnections(uint8_t newConnections) {connections = newConnections;}
    uint8_t getType() const {return type;}
    void setType(uint8_t newType) {type = newType;}
    uint16_t getID() const {return ID;}
    void setID(uint16_t newID) {ID = newID;}

private:
    uint8_t connections;
    uint8_t type;
    uint16_t ID;
};

/* -------------------------------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------ PUZZLEBOX --------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

class Puzzlebox
{
public:
    Puzzlebox(): numWrote{0}{}      //numType{0},
    ~Puzzlebox() {};
    static int8_t countType(int);                   // returns the counted parts for the asked type of a puzzle piece
    void sortParts(vector <Part> *);            // write lists
    void printAllVectors();
    void printVector(int);                      // print all elements of a vector
    void printPart(Part *);                     // print part connections
    static void printTypeArr();                        // prints array with number of each type of part available
    void printID(Part *);
    uint8_t getBaseTypeConnections(int8_t idx){return baseTypes[idx];}

private:
    vector < Part * > type[NR_PART_TYPES];
    static int8_t numType[NR_PART_TYPES];                    // contains all occurrences of the part categeories 0..18
    int numWrote[NR_PART_TYPES];                   // contains the sum of all set parts of part categoeries 0..18
    static uint8_t baseTypes[NR_PART_TYPES];
};

#endif //PUZZLEBOX_H

/*int main()
{
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);

    return 0;
}
*/
