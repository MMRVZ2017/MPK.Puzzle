#include "../../header.h"
void status(vector<LogEntry>& log, vector<Part*>& p_Box);


bool next(vector<LogEntry>& log, vector<Part*>& p_Box,Puzzle& puzzleMat)
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
	    	if(log.back().abstractionLevel < 2)//do 2 at least two best abstractions to check if part is okay
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

void createNextLogElement(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
	log.emplace_back(LogEntry(coor(0, 0)));
   	log.back().myCoor = calculateNextCoor(log, p_Box,puzzleMat);
   	//getLayerDestructionPowerfromSurrounding();
    solve(log, p_Box,puzzleMat);

}

coor calculateNextCoor(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
    //level 1:
        //go left to right, then increase current row

    if (log.size() == 1)
        return {0,0};


	unsigned int m= log.rbegin()[1].myCoor.col;
	unsigned int n= log.rbegin()[1].myCoor.row;


	if(m<puzzleMat.getSizeAsCoor().col-1) m++;
	else if(n<puzzleMat.getSizeAsCoor().row-1){ m=0; n++;}
	return	{m,n};
    //return nextCoor;
}

void solve(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
	//getNextHighestLayerworth(puzzleMat); //sets in abstractionLevel
	//status(log,p_Box,puzzleMat);
    switch(log.back().abstractionLevel)
    {
        case 1:
            puzzleMat.a1->EvaluateQuality(log.back().myCoor, log.back().PieceCollector);
        break;

        default:
        break;
    }

    //capLogElements(log);
    //calculateWorth(log);
    //calculateTrueDestructionPower(log,puzzleMat);
    //calculateNewCombinedProbablility(log);

}

//removes from box and makes log "set"
void setsolution(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
	//advance number of randomed part count
	if(log.back().PieceCollector.size()>1) log.back().advanceRandomed();
	
	//remove first element in last logelement from box
	for(int i=0;i<p_Box.size();)
		if(p_Box[i]==log.back().PieceCollector.begin()->first)//mach ich das richtig so?!
			p_Box.erase(p_Box.begin()+i);
		else
			i++;

	//tell log entry that it is set
	log.back().Set();
}

bool backtrack(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
    //if more pieces possible, take next piece
    if((log.back().PieceCollector.size())>1)
    {
        p_Box.push_back(log.back().PieceCollector.begin()->first);
        log.back().PieceCollector.erase(log.back().PieceCollector.begin());

        if(log.back().PieceCollector.size()==1)
            log.back().decreaseRandomed();

        setsolution(log,p_Box,puzzleMat);

        return true;
    }
    //else remove log element and backtrack once more
    else
    {
        puzzleMat.removeConstrains(log.back().myCoor); //this should remove constraints from all layers
        if((log.back().PieceCollector.size()))
            p_Box.emplace_back(log.back().PieceCollector.begin()->first);
        log.pop_back();
        backtrack(log,p_Box,puzzleMat);
    }
}

void status(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
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