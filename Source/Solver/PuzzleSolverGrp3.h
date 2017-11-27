#include "../Input/Part.h"
#include "../Output/SolutionElement.h"
#include <vector>

class PuzzleSolverGrp3
{
public:
	PuzzleSolverGrp3(vector <Part>* part_array);
	~PuzzleSolverGrp3();

    bool SolvePuzzle();
    bool SolvePuzzle2();
    bool SolvePuzzleNiko();
    bool SolvePuzzleDave();
    void ClearPuzzle();
	vector<vector<SolutionElement>>* GetSolutionMatrix();

private:
	vector <Part>*	m_part_array;
	vector <Part *> m_corners_array;
	vector <Part *> m_edges_array;
	vector <Part *> m_inners_array;
	vector < vector <SolutionElement> > m_solutionVector;
    bool PuzzleLogic(uint8_t currentPart, int collumn, int row);
    bool PuzzleLogic_v2(uint8_t currentPart, int collumn, int row);
    uint8_t GetOrientationForSolutionMatrix(uint16_t index, uint8_t side, uint8_t currentPartNoseOnSide);
    int GetIndexFromPart(Part* part);
    bool RecursiveFindAndPlace(vector<pair<int,int>> nextPuzzlePlaces, vector<pair<int,int>>& tempPuzzle);
    void InitializeVectors();
    void InitializeSolutionMatrix();

    // Niko
    void solveOuterEdge();
    void solveCorners();
};

