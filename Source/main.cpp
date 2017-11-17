#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

#include "header/input.h"

using namespace std;

int main()
{
	vector<Part> myFirstPuzzle;
	Part myFirstPart;
	myFirstPart.setConnections(0b00101000);
	myFirstPuzzle.push_back(myFirstPart);
	cout << "Hello World" << endl;
}
