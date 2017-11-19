
bool next(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //case first log entry empty
    if(!(log.size()))
    {   
        log.push_back(LogEntry());
        solve(log, p_Box,puzzleMat); 
    }

	//case puzzle solved
    else if(!(p_Box.size()))
        return 0;

    //case last log multiple entries
    else if(log.back().PieceCollector.size() > 1)
    {
        //advance abstraction layer of last log by one and solve()
        //or pick first if highest level reached
        if(log.back().abstractionLevel < MAX_ABSTRAX)
        {
            log.back().advance();
            solve(log,p_Box,puzzleMat); 
        }
        else
        {
            setsolution(log,p_Box,puzzleMat);
        }
    }

    //case last log exactly one solution
    else if(log.back().PieceCollector.size() == 1)
    {   
        //create new log, put next coordinates in and go into solve. then git gud
        log.push_back(LogEntry());
        log.back().myCoor = calculateNextCoor(log, p_Box, puzzleMat);  
        solve(log, p_Box,puzzleMat); 
    }

    //case last log empty
        //backtrack
    else if(log.back().PieceCollector.size() == 0)
    {
        backtrack(log,p_Box,puzzleMat);
    }

	return 1;
}


coor calculateFirstCoor(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //returns coor of first piece
    coor firstCoor(0,0);
    return firstCoor;
}

coor calculateNextCoor(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //level 1:
        //go left to right, then increase current row
	int m= log.back().myCoor.m;
	int n= log.back().myCoor.n;
	if(m<puzzleMat.getCols()) m++;
	else if(n<puzzleMat.getRows()){ m=0; n++;}
	else return coor();

;
	return	coor(m,n);
    //return nextCoor;
}

void solve(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
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
    	log.back().PieceCollector[i]=p_Box[i];
}

void abstractionlayer1solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//remove all that do not fit according to abstraction layer 0
	for(int i=0;i<log.back().PieceCollector.size();i++)
	{

		if(!(puzzleMat.testRotationPiece(log.back().myCoor.m, log.back().myCoor.n, *(log.back().PieceCollector[i]))))
			log.back().PieceCollector.erase(log.back().PieceCollector.begin()+i);
	}
}

void setsolution(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
	//remove first element in last logelement from box
	for(int i=0;i<p_Box.size();i++)
		if(p_Box[i]==log.back().PieceCollector[0])
			p_Box.erase(p_Box.begin()+i);	
    
    //set to this element into matrix 
		puzzleMat.setPiece(log.back().myCoor.m, log.back().myCoor.n, *(log.back().PieceCollector[0]));
}  

bool backtrack(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //following possibilities:
        //last log entry empty
            //delete last log + backtrack

	if(!(log.back().PieceCollector.size()))
	{
		log.back().PieceCollector.pop_back();   	
		backtrack(log,p_Box,puzzleMat);
		return 1;
	}

	//last log entry only one solution
            //delete last logd put back into box + backtrack
	else if((log.back().PieceCollector.size())==1)
	{
		p_Box.push_back(log.back().PieceCollector[0]);
		log.back().PieceCollector.pop_back();   	
		//TODO remove from puzzle as well!!! 
		backtrack(log,p_Box,puzzleMat);
		return 1;
	}
        //last log entry multiple solutions (and current one was randomed)
            //delete randomed piece from PieceCollector and go to next (which might random again depending on function)
	else if((log.back().PieceCollector.size())>1)
	{
		p_Box.push_back(log.back().PieceCollector[0]);
		log.back().PieceCollector.erase(log.back().PieceCollector.begin()); 
		setsolution(log,p_Box,puzzleMat);
		return 1;
		//no need to remove from puzzle mat, as sersolution overwrites it anyway
	}
	else
		return 0;

}