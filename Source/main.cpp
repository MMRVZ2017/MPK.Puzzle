#include <iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <algorithm>
#include "Creator/PuzzleCreator.h"
#include "Solver/PuzzleSolverGrp3.h"
#include "Helper/HelperFunctions.h"

using namespace std;

/*
const uint8_t SPALTEN = 35;
const uint8_t ZEILEN = 27;
*/
const uint8_t SPALTEN = 4;
const uint8_t ZEILEN = 3;

int main() {
    // Darf nicht dynamisch gemacht werden, da dadurch die Speicheradressen für die anderen Pointer verschwinden
    vector <Part> part_array(SPALTEN * ZEILEN);

    PuzzleCreator pzlCreator(SPALTEN, ZEILEN, &part_array);

    bool puzzleCreated = false;
    while(!puzzleCreated)
    {
        puzzleCreated = pzlCreator.CreateRandomPuzzle3();
        //part_array = pzlCreator.CreateRandomPuzzle2();
        //puzzleCreated = true;
    }

    /*
    for (uint32_t index = 0; index < part_array.size(); index++)
    {
        if (((index % SPALTEN) == 0) && (index != 0))
        {
            cout << endl;
        }
        cout << bitset<8>(part_array[index].getConnections()).to_string() << "||";
    }
    cout << endl;
    */

    PuzzleSolverGrp3 pzlSolverGrp3(&part_array);

    bool puzzleSolved = false;
    int tries = 0;
    while(!puzzleSolved)
    {
        tries++;
        cout << tries << ". Versuch!" << endl;
        pzlSolverGrp3.ClearPuzzle();
        puzzleSolved = pzlSolverGrp3.SolvePuzzle();
    }

    /*if(!pzlSolverGrp3.SolvePuzzle2())
    {
        cout << "Puzzle ist nicht lösbar!" << endl;
    }*/

    vector<vector<SolutionElement>>* solutionMatrix = pzlSolverGrp3.GetSolutionMatrix();
    for (uint32_t zeile = 0; zeile < (*solutionMatrix)[0].size(); zeile++)
    {
        for (uint32_t spalte = 0; spalte < (*solutionMatrix).size(); spalte++)
        {
            cout << bitset<8>(HelperFunctions::ContinuousShift(part_array[(*solutionMatrix)[spalte][zeile].index].getConnections(), ((*solutionMatrix)[spalte][zeile].orientation * 2))).to_string() << "||";
        }
        cout << endl;
    }

    cin.get();

    return 0;
}