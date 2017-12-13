#include "../../header.h"

void status(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);


bool next(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //log not yet started
    /*
    if(!(log.size()))
    {
    	log.push_back(LogEntry());
        log.back().myCoor = calculateNextCoor(log, p_Box, puzzleMat);
        solve(log, p_Box,puzzleMat);
    }
     */

    //last log element is set, create new log element
    if(!(log.size()) || log.back().isSet())
    {
    	if(!(p_Box.size()))
    	{
	    	cout << "box done" << endl;
	   		return 0;
    	}
	    log.push_back(LogEntry());
   	 	log.back().myCoor = calculateNextCoor(log, p_Box, puzzleMat);
    	solve(log, p_Box,puzzleMat);
    }

    //last log element is empty, backtrack
    else if(!(log.back().PieceCollector.size()))
        backtrack(log,p_Box,puzzleMat);

    //case last log element has multiple entries
    else if(log.back().PieceCollector.size() > 1)
    {
    	//is not yet max abstracted
        if(log.back().abstractionLevel < MAX_ABSTRAX)
        {
        	 log.back().advance();
            solve(log,p_Box,puzzleMat);
        }
        //no more layers, pick first
        else
        {
        	log.back().advanceRandomed();
            setsolution(log,p_Box,puzzleMat);
        }
    }

    //case last log exactly one solution
    else if(log.back().PieceCollector.size() == 1)
    {
        if(log.back().hasRandomed())
        {
	    	if(log.back().abstractionLevel < MAX_ABSTRAX)
	    	{
	    		log.back().advance();
	            solve(log,p_Box,puzzleMat);
	    	}
	    	else
	    		setsolution(log,p_Box,puzzleMat);
    	}
    	else
    		setsolution(log,p_Box,puzzleMat);
    }
    return 1;
}

coor calculateNextCoor(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //level 1:
        //go left to right, then increase current row

    if (log.size() == 1)
        return coor(0,0);


	int m= log.rbegin()[1].myCoor.m;
	int n= log.rbegin()[1].myCoor.n;


	if(m<puzzleMat.getCols()-1) m++;
	else if(n<puzzleMat.getRows()-1){ m=0; n++;}
	else return coor();
	return	coor(m,n);
    //return nextCoor;
}

void solve(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//status(log,p_Box,puzzleMat);
    switch(log.back().abstractionLevel)
    {
        //abstraction layer = 0
    		//go to abstraction layer 0 solver
        case 0:
        abstractionlayer0solver(log,p_Box,puzzleMat);
        break;

        //abstraction layer = 1
            //go to abstraction layer 1 solver
        case 1:
        abstractionlayer1solver(log,p_Box,puzzleMat);
        break;

        default:
        break;
    }
}

void abstractionlayer0solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //throw all remaining puzzle pieces into newest log
    for(int i=0;i<p_Box.size();i++)
    	log.back().PieceCollector.push_back(p_Box[i]);
}

void abstractionlayer1solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//remove all that do not fit according to abstraction layer 0
	for(int i=0;i<(log.back().PieceCollector.size());)
	{
        (*(log.back().PieceCollector[i])).resetShift();
		//TODO: change checker from checking every box piece to only checking the simplifyed version ob the box with abstraction layer one
		if(!(puzzleMat.testRotationPiece(log.back().myCoor, *(log.back().PieceCollector[i]))))
			log.back().PieceCollector.erase(log.back().PieceCollector.begin()+i);
		else
        {
            //set shift to 0 so that we have a defined starting position for all pieces
            while(log.back().PieceCollector[i]->getShift())
                log.back().PieceCollector[i]->shift(1);
            i++; //otherwise loop stops before end!
        }
	}
}

void setsolution(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//remove first element in last logelement from box
	for(int i=0;i<p_Box.size();)
		if(p_Box[i]==log.back().PieceCollector[0])
			p_Box.erase(p_Box.begin()+i);
		else
			i++;

    //turn piece until it fits and then set element into matrix
	if(puzzleMat.testRotationPiece(log.back().myCoor,*(log.back().PieceCollector[0])))
        //error if it turned
		//puzzleMat.setPiece(log.back().myCoor.m, log.back().myCoor.n, *(log.back().PieceCollector[0]));
		puzzleMat.setPiece(log.back().myCoor, *(log.back().PieceCollector[0]));
	else
	{
		cout << "fatal error, wrong piece saved" << endl;
		exit;
	}
	//tell log entry that it is set
	log.back().Set();

}

bool backtrack(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //following possibilities:
    //last log entry empty - delete last log + backtrack
	if(!(log.back().PieceCollector.size()))
	{
		puzzleMat.removePiece(log.back().myCoor);
		log.pop_back();
		backtrack(log,p_Box,puzzleMat);
		return 1;
	}

	//last log entry only one solution - delete last logd put back into box + backtrack
	else if((log.back().PieceCollector.size())==1)
	{
       (log.back().PieceCollector[0])->shift(1);

       //check rotion
        while((log.back().PieceCollector[0])->getShift() !=0 && (log.back().PieceCollector[0])->getShift() !=3)
        {
            log.back().PieceCollector[0]->shift(1);
            if(puzzleMat.testRotationPiece(log.back().myCoor, *(log.back().PieceCollector[0]), 1))
            {
                setsolution(log,p_Box,puzzleMat);
                return 1;
            }
        }

		p_Box.push_back(log.back().PieceCollector[0]);
        //shuffleup
        random_shuffle(p_Box.begin(),p_Box.end());
		puzzleMat.removePiece(log.back().myCoor);
		log.pop_back();
		//cout << "removed" << endl;
		//status(log,p_Box,puzzleMat);
		backtrack(log,p_Box,puzzleMat);

		return 1;
	}

    //last log entry multiple solutions (and current one was randomed) - delete randomed piece and go to next
	else if((log.back().PieceCollector.size())>1)
	{

        //check if piece has second rotation solution
       (*(log.back().PieceCollector[0])).shift(1);

         while((log.back().PieceCollector[0])->getShift() !=0 && (log.back().PieceCollector[0])->getShift() !=3)
        {
            log.back().PieceCollector[0]->shift(1);
            if(puzzleMat.testRotationPiece(log.back().myCoor, *(log.back().PieceCollector[0]), 1))
            {
                setsolution(log,p_Box,puzzleMat);
                return 1;
            }
        }

        p_Box.push_back(log.back().PieceCollector[0]);
        //shuffleup
        random_shuffle(p_Box.begin(),p_Box.end());
        log.back().PieceCollector.erase(log.back().PieceCollector.begin());

        if(log.back().PieceCollector.size()==1)
            log.back().decreaseRandomed();

        //for abstraction layer 1 so that first rotation solution is set.
        (*(log.back().PieceCollector[0])).resetShift();
        setsolution(log,p_Box,puzzleMat);

		return 1;
		//no need to remove from puzzle mat, as sersolution overwrites it anyway
	}
	else
		return 0;

}

void status(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	cout << "----------------------------" << endl;
	cout << "status:" << endl;
	cout << "hasrandomed: " << log[0].hasRandomed() << endl;
	for(int i=0;i<log.size();i++)
	{
		cout << "log    #" << i << ":" << endl;
		cout << "piecenr " << log[i].PieceCollector.size() << endl;
		if(log[i].isSet())
			cout << "isset: 1" << endl;
		else
			cout << "isset: 0" << endl;

		//cout << "Abstraction: " << log[i].abstractionLevel << endl;
		cout << "m: " << log[i].myCoor.m << " n: " << log[i].myCoor.n << endl;
		/*for(int j=0;j<log[i].PieceCollector.size();j++)
		{
			(*(log[i].PieceCollector[j])).printPiece();
			cout << endl;
		}*/
	}

	cout << endl;
	cout << "Box:" << endl;
	cout << "size:  " << p_Box.size() << endl;
	for(vector<PuzzlePiece*>::iterator i = p_Box.begin();i!=p_Box.end();i++)
	{
		(*(*i)).printPiece();
		cout << endl;
	}

	cout << "Puzzle:" << endl;
	puzzleMat.printPuzzle();
	cout << "----------------------------" << endl;
}

bool setBestOrMoreLayers(vector<LogEntry>& log)
{
	int countBest = 0;
	float tempBest = 0.0;

	// count how many Pieces are greater than the threshold value
	for(int i = 0; i < log.PieceCollector.size(); i++)
	{
		// check Probability of current Puzzle Piece in this vector
		if (*(log.back().PieceCollector[i]) >= 0.90) // 0.90 as threshold
		{
			countBest++;
		}
		else
		{
			if (*(log.back().PieceCollector[i]) > tempBest)
			{
				tempBest = *log.back().PieceCollector[i];
			}
		}
	}

	// return true if only one piece is left
	if (1 == countBest)
	{
		return true;
	}
	//else if (countBest > 1 && countBest < 10)	// TODO: add possible constraints
	else
	{
		return false;
	}
}

void calculateNewCombinedProbabilityForPuzzlePiecesArithmetic(vector<LogEntry>& log)
{
	float totalValue = 0.0;

	for(int i = 0; i < log.PieceCollector.size(); i++)
	{
		// sum Probability of current Puzzle Piece in PieceCollector vector
		totalValue += *(log.back().PieceCollector[i]);
	}

	return totalValue / i;
}

void calculateNewCombinedProbabilityForPuzzlePiecesTopK(vector<LogEntry>& log, int executedLayers)
{
	float TopK[executedLayers][2] = {0.0};	// in Log speichern?
	float sumTopK[executedLayers] = {0.0};
	float HighestProbability = 0.0;

	// searching for Top2 probability values in PieceCollector for each layer
	for (int currentLayer = 0; currentLayer < executedLayers; currentLayer++)
	{
		// searching for Top2 probabilities in currentLayer
		for(int i = 0; i < log.PieceCollector.size() && log.back().abstractionLevel == currentLayer; i++)
		{
			if (*(log.back().PieceCollector[i]) > TopK[currentLayer][0])
			{
				TopK[currentLayer][0] = *log.back().PieceCollector[i];
			}
			else if (*(log.back().PieceCollector[i]) > TopK[currentLayer][1])
			{
				TopK[currentLayer][1] = *log.back().PieceCollector[i];
			}
			else
			{
				// Spezialfall fuer 0 Ueberlegen
			}
		}
		sumTopK[currentLayer] = TopK[currentLayer][0] + TopK[currentLayer][1];
	}

	// searching for highest probability for designated Position
	for (int currentLayer = 0; currentLayer < executedLayers; currentLayer++)
	{
		if (sumTopK[currentLayer+1] > sumTopK[currentLayer])
		{
			HighestProbability = sumTopK[currentLayer+1];
		}
	}
}