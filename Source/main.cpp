#include "header.h"

int LogEntry::randomed(0);

int main()
{

    unsigned int cols=5, rows=6;

    //some basic random puzzle stuff
    Puzzle myFirstPuzzle(cols,rows);
    myFirstPuzzle.createRandomBox();

    //some advanced solver stuff
    vector<LogEntry> log;
    vector<Part*> p_myFirstBox;

    Puzzle puzzleMat(cols, rows);
    puzzleMat.createRandomBox();

    //vector<vector<PuzzlePiece*>> ab1class = abstractionLayer1classify(log, p_myFirstBox,puzzleMat);
    while(next(log, puzzleMat.p_myBox,puzzleMat));

    puzzleMat.printPuzzle();
}
