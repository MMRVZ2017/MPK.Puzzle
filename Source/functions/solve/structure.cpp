#include "../../header.h"

void status(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);


bool next(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //last log element is set, create new log element or log not yet started
    if(!(log.size()) || log.back().isSet())
    {
    	if(!(p_Box.size())) return false; //puzzle solved
    	else createNextLogElement(log,p_Box,puzzleMat);
    }
    //last log element is empty, backtrack
    else if(!(log.back().PieceCollector.size())) backtrack(log,p_Box,puzzleMat);
    //case last log element has multiple entries
    else if(log.back().PieceCollector.size() > 1)
    {
			//moreLayers is 0, setbest is 1
    	//if(SetBestorMoreLayers()) 	setsolution(log,p_Box,puzzleMat);
    	//else 						solve(log,p_Box,puzzleMat);
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
    return true;
}

void createNextLogElement(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	log.emplace_back(LogEntry());
   	log.back().myCoor = calculateNextCoor(log, p_Box, puzzleMat);
   	//getLayerDestructionPowerfromSurrounding();
    solve(log, p_Box,puzzleMat);

}

coor calculateNextCoor(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //level 1:
        //go left to right, then increase current row

    if (log.size() == 1)
        return {0,0};


	int m= log.rbegin()[1].myCoor.col;
	int n= log.rbegin()[1].myCoor.row;


	if(m<puzzleMat.getCols()-1) m++;
	else if(n<puzzleMat.getRows()-1){ m=0; n++;}
	else return {};
	return	{m,n};
    //return nextCoor;
}

void solve(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//getNextHighestLayerworth(puzzleMat); //sets in abstractionLevel
	//status(log,p_Box,puzzleMat);
    switch(log.back().abstractionLevel)
    {
        case 0:  abstractionlayer0solver(log,p_Box,puzzleMat);
        break;
        case 1:  abstractionlayer1solver(log,p_Box,puzzleMat);
        break;

        default:
        break;
    }

    //capLogElements(log);
    //calculateWorth(log);
    //calculateTrueDestructionPower(log,puzzleMat);
    //calculateNewCombinedProbablility(log);

}

void abstractionlayer0solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //throw all remaining puzzle pieces into newest log
    for(auto i:p_Box)
    	log.back().PieceCollector.push_back(i);
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
	//advance number of randomed part count
	if(log.back().PieceCollector.size()>1) log.back().advanceRandomed();
	
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
		return true;
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
                return true;
            }
        }

		p_Box.push_back(log.back().PieceCollector[0]);
        //shuffleup
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(p_Box.begin(),p_Box.end(),g);
		puzzleMat.removePiece(log.back().myCoor);
		log.pop_back();
		//cout << "removed" << endl;
		//status(log,p_Box,puzzleMat);
		backtrack(log,p_Box,puzzleMat);

		return true;
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
                return true;
            }
        }

        p_Box.push_back(log.back().PieceCollector[0]);
        //shuffleup
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(p_Box.begin(),p_Box.end(),g);
        log.back().PieceCollector.erase(log.back().PieceCollector.begin());

        if(log.back().PieceCollector.size()==1)
            log.back().decreaseRandomed();

        //for abstraction layer 1 so that first rotation solution is set.
        (*(log.back().PieceCollector[0])).resetShift();
        setsolution(log,p_Box,puzzleMat);

		return true;
		//no need to remove from puzzle mat, as setsolution overwrites it anyway
	}
	else
		return false;

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
		cout << "col: " << log[i].myCoor.col<< " row: " << log[i].myCoor.row << endl;
	}

	cout << endl;
	cout << "Box:" << endl;
	cout << "size:  " << p_Box.size() << endl;
	for(auto i:p_Box)
	{
		i->printPiece();
		cout << endl;
	}

	cout << "Puzzle:" << endl;
	puzzleMat.printPuzzle();
	cout << "----------------------------" << endl;
}

void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth)
{
	//hier muss noch rein, wo die zeit der Abstractionlevels gespeichter wird
	float destructionPower=sqrt(Layerworth * log.back().abstractionLevel);
	//puzzleMat.setdestructionPower(log.back().myCoor,log.back().abstractionLevel,destructionPower);
}