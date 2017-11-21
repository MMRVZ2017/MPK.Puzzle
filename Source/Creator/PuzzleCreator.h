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
	PuzzleCreator(int spalten, int zeilen);
	~PuzzleCreator();

	uint8_t CreateNose();
	uint8_t CreateRandomPart(int wishedEdges);
	bool PuzzleLogicForNewPuzzle(vector <Part> part_array, uint8_t newPart, int partIndex);
	vector <Part> CreateRandomPuzzle(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array);
	vector <Part> CreateRandomPuzzle2(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array);
	vector <Part> CreatePuzzle(vector <Part> part_array, vector <Part *> corners_array, vector <Part *> edges_array, vector <Part *> inners_array);

	int m_spalten;
	int m_zeilen;
};

