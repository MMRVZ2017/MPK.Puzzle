#include "header.h"

int LogEntry::randomed(0);
unsigned int PuzzlePiece::idcount(0);


int main()
{

    unsigned int cols=5, rows=6;

    //some basic random puzzle stuff
    vector<PuzzlePiece> myFirstBox = createBox(coor(cols,rows));

    //some advanced solver stuff
    vector<LogEntry> log;
    vector<PuzzlePiece*> p_myFirstBox;

    //BoxClassify myFirstBox();
    cout << endl;
    for(auto &i:myFirstBox)
        p_myFirstBox.push_back(&i);

    Puzzle puzzleMat(cols, rows);

    //vector<vector<PuzzlePiece*>> ab1class = abstractionLayer1classify(log, p_myFirstBox,puzzleMat);
    //while(next(log, p_myFirstBox,puzzleMat));

    puzzleMat.printPuzzle();
}
