#include "header.h"

int main()
{
	vector<Part> myFirstPuzzle;
	Part myFirstPart;
	myFirstPart.setConnections(0b00101000);
	myFirstPuzzle.push_back(myFirstPart);
	cout << "Hello World" << endl;
	randomBox myFirstBox(2,3);
	myFirstBox.createRandomPuzzle();
	myFirstBox.shuffle();
	myFirstBox.printPuzzle();

}
