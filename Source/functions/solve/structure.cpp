#include "../../header.h"
void status(vector<LogEntry>& log, vector<Part*>& p_Box);
bool setBestOrMoreLayers(vector<LogEntry>& log);
void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth);
void sort(vector<LogEntry>& log);
void cut(vector<LogEntry>& log, Part* cutID);
float capLogElements(vector<LogEntry>& log);
void calculateNewCombinedProbabilityForPuzzlePiecesArithmetic(vector<LogEntry>& log);

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
        if (setBestOrMoreLayers(log)) setsolution(log, p_Box, puzzleMat);
        else solve(log, p_Box, puzzleMat);
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
    puzzleMat.dp.DestructionOfSurrounding(log.back().myCoor);//calculate dp from surrounding
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
}

void solve(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
	puzzleMat.dp.getNextAbstractionLayer(log.back().myCoor,log.back().abstractionLevel); //sets in abstractionLevel
	//status(log,p_Box,puzzleMat);
    switch(log.back().abstractionLevel)
    {
        case 1:
            puzzleMat.a1.EvaluateQuality(log.back().myCoor, log.back().PieceCollector);
        break;

        default:
        break;
    }

    capLogElements(log);
    float worth = capLogElements(log);
    calculateTrueDestructionPower(log,puzzleMat, worth);
    calculateNewCombinedProbabilityForPuzzlePiecesArithmetic(log);

}

//removes from box and makes log "set"
void setsolution(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat)
{
	//advance number of randomed part count
	if(log.back().PieceCollector.size()>1) log.back().advanceRandomed();
	
	//remove first element in last logelement from box
	for(int i=0;i<p_Box.size();)
		if(p_Box[i]==log.back().PieceCollector.begin()->second)//mach ich das richtig so?!
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
        p_Box.push_back(log.back().PieceCollector.begin()->second);
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
            p_Box.emplace_back(log.back().PieceCollector.begin()->second);
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

//this is addon stuff that should later all be extracted into a sererate cpp as it is not core dispatcher functionality

void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth) {
    float destructionPower = sqrt(
            Layerworth * puzzleMat.dp.m_constraintMatrix[0][0].SpeedTable[log.back().abstractionLevel]);
    puzzleMat.dp.setDestructionPower(log.back().myCoor, log.back().abstractionLevel, destructionPower);
}

// PART RAUER_WEIDINGER
float capLogElements(vector<LogEntry>& log)
{
    // Till Now only ground structure -> incorrect variable ans vector names
    double limit = 0.6;
    double diff = 0;
    double maxdiff = 0;
    int vectorsizeBefore = 0;
    int vectorsizeAfter = 0;
    double destroyed = 0; // destroyed parts in %
    double worth = 0;

    vectorsizeBefore = log.back().PieceCollector.size();

    sort(log); // Sort the vector after probabilities
    qualityVector::const_iterator idxcut =log.back().PieceCollector.begin();
    for(;idxcut !=log.back().PieceCollector.end();idxcut++)
        if(idxcut->first < limit)
            break;


    auto newidxcut = idxcut;


    while(idxcut != log.back().PieceCollector.end())
    {
        diff = idxcut->second - (++idxcut)->second;
        if(diff > maxdiff)
        {
            maxdiff = diff;
            newidxcut = idxcut;
        }
    }
    cut(log,newidxcut->second);

    vectorsizeAfter = log.back().PieceCollector.size();

    destroyed = (vectorsizeBefore - vectorsizeAfter) / vectorsizeBefore;

    return (float)sqrt(destroyed*maxdiff);


}

void sort(vector<LogEntry>& log)
{
    //this does all the sorting that needs to happen
    //saddly this is a false statement
    //the monkey desperately tried to hold on to the flying dorm room.
}

qualityVector::iterator FindPartInLog(vector<LogEntry>& log, Part* wishedPartPointer)
{
    qualityVector::iterator partOnPositionIterator = log.back().PieceCollector.begin();

    while (partOnPositionIterator != log.back().PieceCollector.end())
    {
        if(partOnPositionIterator.base()->second == wishedPartPointer)
        {
            break;
        }
        else
        {
            partOnPositionIterator++;
        }
    }

    return partOnPositionIterator;
}

void cut(vector<LogEntry>& log, Part* cutID)
{
    auto it = FindPartInLog(log, cutID);
    while(it != log.back().PieceCollector.end())
        log.back().PieceCollector.erase(it++);
}

//partdavid
bool setBestOrMoreLayers(vector<LogEntry>& log)
{
    int countBest = 0;
    float tempBest = 0.0;

    // count how many Pieces are greater than the threshold value
    for(auto it:log.back().PieceCollector)
    {
        // check Probability of current Puzzle Piece in this vector
        if (it.first >= 0.90) // 0.90 as threshold
            countBest++;
        else
            if (it.first > tempBest)
                tempBest = it.first;
    }

    // return true if only one piece is left
    if (1 == countBest)
    {
        return true;
    }
        //else if (countBest > 1 && countBest < 10) // TODO: add possible constraints
    else
    {
        return false;
    }
}

void calculateNewCombinedProbabilityForPuzzlePiecesArithmetic(vector<LogEntry>& log)
{
    float totalValue = 0.0;
    int i;
    for(int i; i < log.back().PieceCollector.size(); i++);

        // sum Probability of current Puzzle Piece in PieceCollector vector
        //totalValue += *(log.back().PieceCollector.);

    //return totalValue / i;
}

/*
//PartDavid
void calculateNewCombinedProbabilityForPuzzlePiecesTopK(vector<LogEntry>& log, int executedLayers)
{
    float TopK[executedLayers][2] = {0.0};  // in Log speichern?
    float sumTopK[executedLayers] = {0.0};
    float HighestProbability = 0.0;

    // searching for Top2 probability values in PieceCollector for each layer
    for (int currentLayer = 0; currentLayer < executedLayers; currentLayer++)
    {
        // searching for Top2 probabilities in currentLayer
        for(int i = 0; i < log.back().PieceCollector.size() && log.back().abstractionLevel == currentLayer; i++)
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

*/