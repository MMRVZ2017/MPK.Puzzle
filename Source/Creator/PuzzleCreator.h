#include <stdint.h>
#include <vector>
#include <iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <algorithm>
#include "../Input/Part.h"

using namespace std;

class PuzzleCreator
{
public:
	PuzzleCreator(int spalten, int zeilen, vector <Part>* part_array);
	~PuzzleCreator();

	uint8_t CreateNose();
	uint8_t CreateRandomPart(int wishedEdges);
	bool PuzzleLogicForNewPuzzle(uint8_t newPart, int partIndex);
	vector <Part>* CreateRandomPuzzle();
    vector <Part>* CreateRandomPuzzle2();
    bool CreateRandomPuzzle3();
	vector <Part>* CreatePuzzle();

    void RotateWholePuzzle();
    void ShuffleWholePuzzle();

	vector <Part>* m_part_array;
    int m_spalten;
	int m_zeilen;
};

