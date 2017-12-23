#include "header.h"

int LogEntry::randomed(0);

int main()
{

    unsigned int cols=5, rows=6;

    vector<LogEntry> log;
    Puzzle puzzleMat(cols, rows);
    puzzleMat.createRandomBox();
    cout << "here" << endl;
    puzzleMat.a1.printConstraintMatrix();

    //vector<vector<PuzzlePiece*>> ab1class = abstractionLayer1classify(log, p_myFirstBox,puzzleMat);
    while(next(log, puzzleMat.p_myBox,puzzleMat));

    puzzleMat.printPuzzle();
    return 0;
}
