
bool next(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    //case first log entry empty
    if(!(log.size())
    {   
        log.push_back(LogEntry());
        solve(log, p_Box,puzzleMat); 
    }

	//case puzzle solved
    else if(!(p_Box.size())
        return 0;

    //case last log multiple entries
    else if(log.back().PieceCollector.size() > 1)
    {
        //advance abstraction layer of last log by one and solve()
        //or pick first if highest level reached
        if(log.back().abstractionLayer < MAX_ABSTRAX)
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
        log.back().myCoor = getNextCoor(log, p_Box, puzzleMat);  
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

        //if 

    //return nextCoor;
}

void solve(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat)
{
    switch(log.back().abstraction==0)
    {
        case 0:
        break;

        case 1:
        break;
    }
    if()
        //abstraction layer = 0
            //go to abstraction layer 0 solver
    
    //case
        //abstraction layer = 1
            //go to abstraction layer 1 solver
    //default
            //random one of the parts 
}

void abstractionlayer0solver()
{
    //throw all remaining puzzle pieces into newest log
}

void abstractionlayer1solver()
{
    //remove all impossible from newest log according to abstraction layer one
}

void setsolution()
{
    //remove first element in last logelement from box
    //set pointer to this element into matrix 
}  

void backtrack()
{
    //following possibilities:
        //last log entry empty
            //delete last log + backtrack
        //last log entry only one solution
            //delete last log + backtrack
        //last log entry multiple solutions (and current one was randomed)
            //delete randomed piece from puzzleCollector and go to next (which might random again depending on function)
}