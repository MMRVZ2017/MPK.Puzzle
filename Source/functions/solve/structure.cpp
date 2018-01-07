#include "../../header.h"
void status(vector<LogEntry>& log, vector<Part*>& p_Box);
bool SetBestOrMoreLayersArithmetical(vector<LogEntry>& log, qualityVector& cqVector);
void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth);
void sort(vector<LogEntry>& log);
void cut(vector<LogEntry>& log, int& cutID);
float capLogElements(vector<LogEntry>& log);
void CalculateNewCombinedQuality(vector<LogEntry>& log, qualityVector& qVector, qualityVector& cqVector);

bool next(vector<LogEntry>& log,Puzzle& puzzleMat)
{
    //last log element is set, create new log element or log not yet started
    if(!(log.size()) || log.back().isSet())
    {
    	if((puzzleMat.allSet())) return false; //puzzle solved
    	else createNextLogElement(log,puzzleMat);
    }
    //last log element is empty, backtrack
    else if(!(log.back().PieceCollector.size())) backtrack(log,puzzleMat);
    //case last log element has multiple entries
    else if(log.back().PieceCollector.size() > 1)
    {
        //moreLayers is 0, setbest is 1
        if (SetBestOrMoreLayersArithmetical(log, puzzleMat.combinedQualityVector)) setsolution(log, puzzleMat);
        else solve(log, puzzleMat);
    }
    //case last log exactly one solution
    else if(log.back().PieceCollector.size() == 1)
    {
        if(log.back().hasRandomed())
        {
	    	if(log.back().abstractionLevel < 2)//do 2 at least two best abstractions to check if part is okay
	    	{
	    		log.back().advance();
	            solve(log,puzzleMat);
	    	}
	    	else
	    		setsolution(log,puzzleMat);
    	}
        else
    		setsolution(log,puzzleMat);
    }
    return true;
}

void createNextLogElement(vector<LogEntry>& log, Puzzle& puzzleMat)
{
	log.emplace_back(LogEntry(coor(0, 0)));
   	log.back().myCoor = calculateNextCoor(log, puzzleMat);
    puzzleMat.dp.DestructionOfSurrounding(log.back().myCoor);//calculate dp from surrounding
    for(auto it:puzzleMat.p_myBox)
        log.back().PieceCollector.emplace_back(pair<float,Part*>(0,it));
    cout << puzzleMat.p_myBox.size() << endl;
    cout << log.back().PieceCollector.size() << endl;
   	solve(log,puzzleMat);

}

coor calculateNextCoor(vector<LogEntry>& log, Puzzle& puzzleMat)
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
	log.back().abstractionLevel = puzzleMat.dp.getNextAbstractionLayer(log.back().myCoor,log.back().abstractionLevel); //sets in abstractionLevel
	//status(log,p_Box,puzzleMat);
    switch(log.back().abstractionLevel)
    {
        case 0:
            puzzleMat.a1.EvaluateQuality(log.back().myCoor, log.back().PieceCollector);
        break;

        default:
        break;
    }

    float worth = capLogElements(log);
    calculateTrueDestructionPower(log,puzzleMat, worth);
    CalculateNewCombinedQuality(log, log.back().PieceCollector, puzzleMat.combinedQualityVector);

}

//removes from box and makes log "set"
void setsolution(vector<LogEntry>& log, Puzzle& puzzleMat)
{
	//advance number of randomed part count
	if(log.back().PieceCollector.size()>1) log.back().advanceRandomed();
	
	//remove first element in last logelement from box
	for(int i=0;i<puzzleMat.p_myBox.size();)
		if(puzzleMat.p_myBox[i]==log.back().PieceCollector.begin()->second)//mach ich das richtig so?!
            puzzleMat.p_myBox.erase(puzzleMat.p_myBox.begin()+i);
		else
			i++;

	//tell log entry that it is set
	log.back().Set();
}

bool backtrack(vector<LogEntry>& log, Puzzle& puzzleMat)
{
    //if more pieces possible, take next piece
    if((log.back().PieceCollector.size())>1)
    {
        puzzleMat.p_myBox.push_back(log.back().PieceCollector.begin()->second);
        log.back().PieceCollector.erase(log.back().PieceCollector.begin());

        if(log.back().PieceCollector.size()==1)
            log.back().decreaseRandomed();

        setsolution(log,puzzleMat);

        return true;
    }
    //else remove log element and backtrack once more
    else
    {
        puzzleMat.removeConstrains(log.back().myCoor); //this should remove constraints from all layers
        if((log.back().PieceCollector.size()))
            puzzleMat.p_myBox.emplace_back(log.back().PieceCollector.begin()->second);
        log.pop_back();
        backtrack(log,puzzleMat);
    }
}

void status(vector<LogEntry>& log, Puzzle& puzzleMat)
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
	cout << "size:  " << puzzleMat.p_myBox.size() << endl;

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

    int id=0;

    double maxdiff = 0;
    int vectorsizeBefore = 0;
    int vectorsizeAfter = 0;
    double destroyed = 0; // destroyed parts in %

    vectorsizeBefore = log.back().PieceCollector.size();

    sort(log.back().PieceCollector.begin(),log.back().PieceCollector.end()); // Sort the vector after probabilities
    reverse(log.back().PieceCollector.begin(),log.back().PieceCollector.end());
    for(;id<log.back().PieceCollector.size();id++)
    {
        if(log.back().PieceCollector[id].first < limit)
            break;
    }


    int newid = --id; //set to the one just over limit


    while(id<log.back().PieceCollector.size()-1) //find maximum difference in function
    {

        diff = log.back().PieceCollector[id].first - log.back().PieceCollector[++id].first;
        if(diff > maxdiff)
        {
            maxdiff = diff;
            newid = id;
        }
    }
    cut(log,newid);

    vectorsizeAfter = log.back().PieceCollector.size();
    destroyed = ((double)vectorsizeBefore - (double)vectorsizeAfter) / (double)vectorsizeBefore;
    return (float)sqrt(destroyed*maxdiff);


}

qualityVector::iterator FindPartInLog(vector<LogEntry>& log, Part* wishedPartPointer)
{
    qualityVector::iterator partOnPositionIterator = log.back().PieceCollector.begin();

    while (partOnPositionIterator != log.back().PieceCollector.end())
    {
        if(partOnPositionIterator->second == wishedPartPointer)
            break;
        else
            partOnPositionIterator++;
    }

    return partOnPositionIterator;
}

void cut(vector<LogEntry>& log, int& cutID)
{
    while(cutID<log.back().PieceCollector.size())
        log.back().PieceCollector.erase(log.back().PieceCollector.begin()+cutID);
}

// --------------------  Part David: SetBest and CalculateCombinedQuality --------------------
// pruefen, ob mehr als X combinedQualities ueber dem Grenzwert sind. Wenn nur noch Y Pieces ueber bleiben, dann setBest!
// geeignete Threshold values muessen noch getestet werden
bool SetBestOrMoreLayersArithmetical(vector<LogEntry>& log, qualityVector& cqVector)
{
    float threshold = 1.0, tempBest = 0.0;
    unsigned int countHigherThreshold = 0;

    if(cqVector.empty())
    {
        cerr << "combinedQualityVector is empty." << endl;  // should not be empty => backtrack?
        return false; // Warning: can only return true or false. What return for error?
    }
    else
    {
        switch(log.back().abstractionLevel)
        {
            case 1: threshold = 0.90; break;
            case 2: threshold = 0.80; break;
            case 3: threshold = 0.75; break;
            case 4: threshold = 0.66; break;
            case 5: threshold = 0.60; break;
            default: threshold = 0.5; break;
        }

        // check Quality of current Puzzle Piece in combinedQualityVector with Threshold value
        for (qualityVector::iterator it = cqVector.begin(); it != cqVector.end(); it++)
        {
            if ((cqVector.back().first / log.back().abstractionLevel) >= threshold) // const threshold values
            {
                // count how many Pieces are greater than the threshold value
                countHigherThreshold++;
            }
            else
            {
                if ((cqVector.back().first / log.back().abstractionLevel) > tempBest)
                {
                    tempBest = cqVector.back().first;  // could be used, for additional constraints
                }
            }
        }

        // return true if only one piece is left
        if (1 == countHigherThreshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// jede Quality vom Piececollector zu einer combinedQuality aufsummieren (von jedem bereits verwendetem Layer)
// Achtung: Es muss noch der Mittelwert gebildet werden => SetBestOrMoreLayersArithmetical
void CalculateNewCombinedQuality(vector<LogEntry>& log, qualityVector& qVector, qualityVector& cqVector)
{
    bool summarizedVectors = false;
    int countSummarizedVectors = 0;

    // check if both qualityVectors are not empty
    if(qVector.empty())
    {
        cerr << "qualityVector is empty." << endl;  // should not be empty => backtrack?
        return;
    }
    else if(cqVector.empty())
    {
        cerr << "combinedQualityVector is empty." << endl;  // should not be empty => backtrack?
        return;
    }
    else
    {
        for (unsigned int i = 0; i < cqVector.size(); i++)
        {
            summarizedVectors = false;

            for (unsigned int j = 0; j < qVector.size(); j++)
            {
                // search same PuzzlePart of qualityVector and combinedQualityVector
                if (&cqVector.at(i).second == &qVector.at(j).second)
                {
                    // sum Quality of PieceCollector (qualityVector) to combinedQualityVector
                    cqVector.at(j).first += qVector.at(i).first;
                    countSummarizedVectors++;
                    summarizedVectors = true;
                    continue; // skip remaining for loop => save time!
                }
            }

            // remove element at poisition X in combinedQualityVector, because it was not summarized
            if (!summarizedVectors)
            {
                // inefficient way to delete element X
                //cqVector->erase(cqVector->begin()+i);

                // efficient way, but no sorted cqVector => wayne
                swap(cqVector.at(i), cqVector.back());
                cqVector.pop_back();
            }
        }

        // cqVector should have the same size now as newest qVector
        if (cqVector.size() != qVector.size())
        {
            cerr << "Size of combinedQualityVector doenst match with size of qualityVector!" << endl;
            cout << "Size of combinedQualityVector: " << cqVector.size() << endl;
            cout << "Size of qualityVector: " << qVector.size() << endl;
            cout << "Size of countSummarizedVectors: " << countSummarizedVectors << endl;
        }
    }
}