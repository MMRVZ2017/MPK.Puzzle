#include "header.h"

int LogEntry::randomed(0);

int main()
{

    unsigned int cols=36,rows=28;

    vector<LogEntry> log;
    Puzzle puzzleMat(cols, rows);
    if(!puzzleMat.PreProcessing())
    {
        cerr << "Error occurred at PreProcessing!";
        return 0;
    }

    //puzzleMat.createRandomBox();

    cout << "Solving Puzzle now..." << flush;
    while(next(log, puzzleMat));

    cout << "Done!" << endl;
    cout << "Printing Puzzle now..." <<flush;
    puzzleMat.resultImage(log);


    //puzzleMat.printPuzzle();
    return 0;
}
