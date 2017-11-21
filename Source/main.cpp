#include <iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <algorithm>
#include "Creator/PuzzleCreator.h"
#include "Solver/PuzzleSolverGrp3.h"
//teständerung
using namespace std;

/*
const uint8_t SPALTEN = 35;
const uint8_t ZEILEN = 27;
*/
const uint8_t SPALTEN = 36;
const uint8_t ZEILEN = 28;

int main() {
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);

    PuzzleCreator pzlCreator(SPALTEN, ZEILEN);

    /*for (int i = 0; i < 10; i++)
    {
        uint8_t newPart = CreateRandomPart(1);
        cout << bitset<8>(newPart).to_string() << endl;
    }*/

    /*uint8_t test = 0;
    test |= 0b11111111;

    cout << test << " " << bitset<8>(test).to_string() << endl;*/

    //part_array = ErstellePuzzle(part_array, corners_array, edges_array, inners_array);
    part_array = pzlCreator.CreateRandomPuzzle2(part_array, corners_array, edges_array, inners_array);

    /*for (uint32_t index = 0; index < part_array.size(); index++)
    {
        if ((index % SPALTEN) == 0)
        {
            cout << endl;
        }
        cout << "||" << bitset<8>(part_array[index].getConnections()).to_string();
    }*/

    /*int anzEcken = 0;
    int anzKanten = 0;
    int anzInner = 0;
    cout << anzEcken << " " << anzKanten << " " << anzInner << endl;

    anzEcken = corners_array.size();
    anzKanten = edges_array.size();
    anzInner = inners_array.size();
    cout << anzEcken << " " << anzKanten << " " << anzInner << endl;

    int kanteV = 0;
    int kanteH = 0;
    for (kanteH = 1; kanteH <= (anzKanten / 2) - 1; kanteH++)
    {
        kanteV = (anzKanten / 2) - kanteH;

        if ((kanteH * kanteV) == anzInner) // 884
        {
            cout << "Zeilen: " << kanteV + 2 << endl;
            cout << "Spalten: " << kanteH + 2 << endl;
            cout << "Anzahl Kanten: " << kanteV * 2 + kanteH * 2 << endl; // 36*28 = 884
            cout << "Matrixgroesse = " << (kanteV + 2)*(kanteH + 2) << endl;
        }
    }*/

    PuzzleSolverGrp3 pzlSolverGrp3(part_array, corners_array, edges_array, inners_array);
    pzlSolverGrp3.SolvePuzzle();

    cin.get();

    return 0;
}