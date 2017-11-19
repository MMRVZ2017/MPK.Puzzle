#define MAX_ABSTRAX 1
#define structdebug

#include "header.h"



int main()
{

	int cols=10, rows=10;
	//some basic part stuff
	vector<Part> myFirstPuzzle;
	Part myFirstPart;
	myFirstPart.setConnections(0b00101000);
	myFirstPuzzle.push_back(myFirstPart);

	//some basic random puzzle stuff
	randomBox myRandomBox(cols,rows);
	myRandomBox.createRandomPuzzle();
	vector<PuzzlePiece> myFirstBox = myRandomBox.shuffle();



	//some advanced solver stuff
	vector<LogEntry> log;
	vector<PuzzlePiece*> p_myFirstBox;

	cout << "original puzzle: " << endl;
	myRandomBox.printPuzzle();
	cout << endl;
	for(int i=0;i<myFirstBox.size();i++)
		p_myFirstBox.push_back(&myFirstBox[i]);
	Puzzle puzzleMat(cols, rows);

	while(next(log, p_myFirstBox,puzzleMat));

	puzzleMat.printPuzzle();
}
