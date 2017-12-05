#define MAX_ABSTRAX 1
#define structdebug


#include "header.h"
//#include "../Codicil/test_puzzle_long40x40.h"
#include "functions/AbstractionLayers/Layer1/AbstractionLayer_1.h"
#include "functions/AbstractionLayers/Layer2/AbstractionLayer_2.h"

int main()
{

    int cols=10, rows=20;
    //some basic part stuff
    vector<Part_v2> myFirstPuzzle;
    Part_v2 myFirstPart;
    //myFirstPart.setConnections(0b00101000);
    myFirstPuzzle.push_back(myFirstPart);

    AbstractionLayer_1 test1(&myFirstPuzzle);
    test1.Test();
    //AbstractionLayer_2 test2(&myFirstPuzzle);

    /*
    //some basic random puzzle stuff
    randomBox myRandomBox(cols,rows);
    myRandomBox.createRandomPuzzle();
    vector<PuzzlePiece> myFirstBox = myRandomBox.shuffle();

    //undo everything and make this puzzle fucking imba hard!!!
    //need 40x40 for this, so check your status

    //makehard4040puzzle(myFirstBox);

    //some advanced solver stuff
    vector<LogEntry> log;
    vector<PuzzlePiece*> p_myFirstBox;

    //BoxClassify myFirstBox();

    cout << "original puzzle: " << endl;
    myRandomBox.printPuzzle();
    cout << endl;
    for(int i=0;i<myFirstBox.size();i++)
        p_myFirstBox.push_back(&myFirstBox[i]);
    Puzzle puzzleMat(cols, rows);

    //vector<vector<PuzzlePiece*>> ab1class = abstractionLayer1classify(log, p_myFirstBox,puzzleMat);
    while(next(log, p_myFirstBox,puzzleMat));

    puzzleMat.printPuzzle();
    */
}
