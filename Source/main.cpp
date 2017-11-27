#include <iostream>
#include <bitset>
#include <vector>
#include "Creator/PuzzleCreator.h"
#include "Solver/PuzzleSolverGrp3.h"
#include "Helper/HelperFunctions.h"

using namespace std;

const uint8_t SPALTEN = 3;
const uint8_t ZEILEN = 3;

void MatrixAusgeben(vector <Part> part_array)
{
    for (uint32_t index = 0; index < part_array.size(); index++)
    {
        if (((index % SPALTEN) == 0) && (index != 0))
        {
            cout << endl;
        }
        cout << bitset<8>(part_array[index].getConnections()).to_string() << "||";
    }
    cout << endl;
}

int main() {
    vector <Part> part_array(SPALTEN * ZEILEN);

    PuzzleCreator pzlCreator(SPALTEN, ZEILEN, &part_array);

    bool puzzleCreated = false;
    while(!puzzleCreated)
    {
        puzzleCreated = pzlCreator.CreateRandomPuzzle3();
    }

    //MatrixAusgeben(part_array);

    PuzzleSolverGrp3 pzlSolverGrp3(&part_array);

    // Solver - Maximilian - Random
    /*bool puzzleSolved = false;
    int tries = 0;
    while(!puzzleSolved)
    {
        tries++;
        cout << tries << ". Versuch!" << endl;
        pzlSolverGrp3.ClearPuzzle();
        puzzleSolved = pzlSolverGrp3.SolvePuzzle();
    }*/

    // Solver - Maximilian - Depth First
    if(!pzlSolverGrp3.SolvePuzzle2())
    {
        cout << "Puzzle ist nicht lösbar!" << endl;
    }

    // Solver - Niko - Border
    //bool puzzleSolved = pzlSolverGrp3.SolvePuzzleNiko();

    // Solver - Dave - Border
    //bool puzzleSolved = pzlSolverGrp3.SolvePuzzleDave();

    // Ausgabe der fertigen Matrix
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