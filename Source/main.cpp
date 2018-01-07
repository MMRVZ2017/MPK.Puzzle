#include "header.h"

int LogEntry::randomed(0);

int main()
{

    unsigned int cols=6,rows=8;

    vector<LogEntry> log;
    Puzzle puzzleMat(cols, rows);
    if(!puzzleMat.PreProcessing())
    {
        cerr << "Error occurred at PreProcessing!";
        return 0;
    }

    puzzleMat.createRandomBox();

    cout << "Solving Puzzle now...";
    while(next(log, puzzleMat));

    cout << "Done!" << endl;
    puzzleMat.printPuzzle();
    return 0;
}
