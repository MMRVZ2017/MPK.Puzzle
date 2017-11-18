//Raphael Maenle - 11.11.2017
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

#define debug

#include "puzzle.h"
#include "test_puzzle.h"

using namespace std;

vector<LogEntry> log;

int main()
{
	srand(time(0));

    vector<Part> part_array(NR_PARTS);
    vector<Part *> corners_array(NR_CORNERS);
    vector<Part *> edges_array(NR_EDGES);
    vector<Part *> inners_array(NR_INNERS);
    
    //randomBox myPuzzle(cols,rows);
    //myPuzzle.createRandomPuzzle();

    unsigned int rows=20, cols=20;
    vector<PuzzlePiece> myFirstBox = createBox(cols, rows);
    create4040hardBox(myFirstBox);

    printBox(myFirstBox);
    cout << endl;










    while(next());




    return 0;
}

next()
{

    //calculates next move according to log

		//case puzzle solved
			//return 0;

        //case last log empty
            //backtrack

        //case last log exactly one solution
			//put all remaining puzzle pieces into new log entry
            //try next piece/place with solve abstraction level 0

        //case last log multiple entries
            //advance abstraction layer of last log by one and solve()
            //or pick first if highest level reached

		//return 1;
}

solve()
{
    //case
        //abstraction layer = 0
            //go to abstraction layer 0 solver
    
    //case
        //abstraction layer = 1
            //go to abstraction layer 1 solver
    //default
            //random one of the parts 
}

abstractionlayer0solver()
{
    //throw all remaining puzzle pieces into log
        //remove all impossible
}

abstractionlayer1solver()
{
    //remove all impossible according to abstraction layer one
}

setsolution()
{
    //put 
    //set pointer to log into matrix 
}  

backtrack()
{
    //following possibilities:
        //last log entry empty
            //delete last log + backtrack
        //last log entry only one solution
            //delete last log + backtrack
        //last log entry multiple solutions (and current one was randomed)
            //delete randomed piece from puzzleCollector and go to next (which might random again depending on function)
}