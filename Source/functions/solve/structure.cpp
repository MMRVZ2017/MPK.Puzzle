#include "../../header.h"
bool SetBestOrMoreLayersArithmetical(vector<LogEntry>& log, qualityVector& cqVector);
void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth);
void cut(vector<LogEntry>& log, int& cutID);
float capLogElements(vector<LogEntry>& log);
void CalculateNewCombinedQuality(vector<LogEntry>& log, qualityVector& qVector, qualityVector& cqVector);

bool next(vector<LogEntry>& log,Puzzle& puzzleMat)
{
    //last log element is set, create new log element or log not yet started
    if(!(log.size()) || log.back().isSet())
    	if((puzzleMat.allSet()))
            return false; //puzzle solved
    	else createNextLogElement(log,puzzleMat);

    //last log element is empty, backtrack
    else if(!(log.back().PieceCollector.size()))
    {
        if(!(backtrack(log,puzzleMat)))
            return false;
    }

    //case last log element has multiple entries
    else if(log.back().PieceCollector.size() > 1)
        //moreLayers is 0, setbest is 1
        if (SetBestOrMoreLayersArithmetical(log, puzzleMat.combinedQualityVector)) setsolution(log, puzzleMat);
        else solve(log, puzzleMat);

    //case last log exactly one solution
    else if(log.back().PieceCollector.size() == 1)
        if(log.back().hasRandomed())
	    	if(log.back().abstractionLevel < 2)//do 2 at least two best abstractions to check if part is okay
	            solve(log,puzzleMat);
	    	else
	    		setsolution(log,puzzleMat);
        else
    		setsolution(log,puzzleMat);
    return true;

}

void createNextLogElement(vector<LogEntry>& log, Puzzle& puzzleMat)
{
	log.emplace_back(LogEntry(coor(0, 0)));
   	log.back().myCoor = calculateNextCoor(log, puzzleMat);
    puzzleMat.dp.DestructionOfSurrounding(log.back().myCoor);//calculate dp from surrounding
     //get all not set pieces
    for(auto it:puzzleMat.p_myBox)
        if(!it->set)
            log.back().PieceCollector.emplace_back(pair<float,Part*>(0,it));
    solve(log,puzzleMat);

}

coor calculateNextCoor(vector<LogEntry>& log, Puzzle& puzzleMat)
{

    if (log.size() == 1)
        return {0,0};


    unsigned int col= log.rbegin()[1].myCoor.col;
    unsigned int row= log.rbegin()[1].myCoor.row;
    //level 2 edges first

    if(col == 0 && row < 27)
        return {col,++row};
    if(row== 27 && col < 35)
        return {++col,row};
    if(col == 35 && row >0)
        return {col, --row};
    if(col >1&& row ==0)
        return{--col,row};
    if(col==1 && row==0)
        return {1,1};


    log.pop_back();//vis only!!!
    puzzleMat.resultImage(log);//vis only!!!


    if(row<puzzleMat.getSizeAsCoor().row-2) row++;
    else if(col<puzzleMat.getSizeAsCoor().col-2){ row=1; col++;}
    return	{col,row};

    //level 1:
        //go left to right, then increase current row



	if(row<puzzleMat.getSizeAsCoor().row-1) row++;
	else if(col<puzzleMat.getSizeAsCoor().col-1){ row=0; col++;}
	return	{col,row};
}

void solve(vector<LogEntry>& log,Puzzle& puzzleMat)
{
    log.back().abstractionLevel = puzzleMat.dp.getNextAbstractionLayer(log.back().myCoor,log.back().abstractionLevel); //sets in abstractionLevel
    cout << "a: " << log.back().abstractionLevel << endl;

    //status(log,p_Box,puzzleMat);
    //TODO!! Add more layers here
    switch(log.back().abstractionLevel)
    {
        case 0://pömpel
            puzzleMat.a1.EvaluateQuality(log.back().myCoor,log.back().PieceCollector);
        break;
        case 2://SURFFeature
//            return;
            puzzleMat.a4.EvaluateQuality(log.back().myCoor,log.back().PieceCollector);
            break;
        case 3://poempelposition
            puzzleMat.a3.EvaluateQuality(log.back().myCoor,log.back().PieceCollector);
            break;
        case 1://color
            puzzleMat.acm.EvaluateQuality(log.back().myCoor,log.back().PieceCollector);
            break;
        case -1://random
            setsolution(log,puzzleMat);
        return;
        default:
        break;
    }
    float worth = capLogElements(log);
    cout << "remaining: " << log.back().PieceCollector.size() << endl;
    calculateTrueDestructionPower(log,puzzleMat, worth);
    CalculateNewCombinedQuality(log, log.back().PieceCollector, puzzleMat.combinedQualityVector);

}

//removes from box and makes log "set"
void setsolution(vector<LogEntry>& log, Puzzle& puzzleMat)
{
	//advance number of randomed part count
	if(log.back().PieceCollector.size()>1) log.back().advanceRandomed();

	//'set=true' all 4 rotations of pieces in puzzleBox
	for(int i=0;i<puzzleMat.p_myBox.size();i++)
		if(puzzleMat.p_myBox[i]->GetPartID()==log.back().PieceCollector.begin()->second->GetPartID())
            puzzleMat.p_myBox[i]->set=true;

    puzzleMat.combinedQualityVector.clear(); //clear data from temp variable
	//tell log entry that it is set
	log.back().Set();
    puzzleMat.setConstraints(log.back().myCoor,log.back().PieceCollector.begin()->second);
    cout << "set:" << log.back().myCoor.col << "," << log.back().myCoor.row << endl;
    //cout << "ID: " << log.back().PieceCollector[0].second->GetPartID() << endl;
    puzzleMat.resultImage(log);

}

bool backtrack(vector<LogEntry>& log, Puzzle& puzzleMat)
{
   cout << "backtrack" << endl;
    if(log.empty())
    {
        cout << "Puzzle not solveable!" << endl;
        return false;
    }
    puzzleMat.combinedQualityVector.clear(); //remove all data from temp quality save
    //if more pieces possible, tset piece as not logged
    if((log.back().PieceCollector.size())>1)
    {
        for(int i=0;i<puzzleMat.p_myBox.size();i++)//unset puzzlepieces
            if(puzzleMat.p_myBox[i]->GetPartID()==log.back().PieceCollector.begin()->second->GetPartID())//sets all with partid
                puzzleMat.p_myBox[i]->set=false;


        //remove similar in log
        Part myPart = *log.back().PieceCollector[0].second;//tmpsaves bad part
        log.back().PieceCollector.erase(log.back().PieceCollector.begin());//removes bad part from log
         puzzleMat.removeSimilar(log.back().PieceCollector,myPart); //removes all pieces from log that are similar to bad part
        //TODO reprogram similar removal to allow multilayer tracking
        if(log.back().PieceCollector.size()) // this checks if 'removeSimilar' has cleared entire LogElement
        {
            if(log.back().PieceCollector.size()==1)
                log.back().decreaseRandomed();
            setsolution(log,puzzleMat);
            return true;
        }
    }
    //else remove log element and backtrack once more
    puzzleMat.removeConstrains(log.back().myCoor); //this should remove constraints from all layers
    if((log.back().PieceCollector.size())) //unset all
        for(int i=0;i<puzzleMat.p_myBox.size();i++)
            if(puzzleMat.p_myBox[i]->GetPartID()==log.back().PieceCollector.begin()->second->GetPartID())//sets all with partid
                puzzleMat.p_myBox[i]->set=false;

    log.pop_back();
     if(!backtrack(log,puzzleMat))
    {
        return false;

    }
    return true;
}


void calculateTrueDestructionPower(vector<LogEntry>& log, Puzzle& puzzleMat, float Layerworth) {
    float destructionPower = sqrt(
            Layerworth * puzzleMat.dp.m_constraintMatrix[0][0].SpeedTable[log.back().abstractionLevel]);

    //save destructionArray for when coor is done
    if(puzzleMat.tmp_destructionArray.empty())
        for(auto it:puzzleMat.dp.m_constraintMatrix[log.back().myCoor.col][log.back().myCoor.row].DestructionArray)
            puzzleMat.tmp_destructionArray.emplace_back(it);

    puzzleMat.tmp_destructionArray[log.back().abstractionLevel]=destructionPower;

}

// PART RAUER_WEIDINGER
float capLogElements(vector<LogEntry>& log)
{

    // Till Now only ground structure -> incorrect variable ans vector names
    double limit = 0.1;
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

    int newid=0;
    //check if all over
    if(id==log.back().PieceCollector.size())
        return 0;
    if(id>0)
        newid = --id; //set to the one just over limit

    while(id<(log.back().PieceCollector.size()-1)) //find maximum difference in function
    {
        if(!log.back().PieceCollector[id].first)
            break;

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
    float threshold, tempBest = 0.0;
    unsigned int countHigherThreshold = 0;

    if(cqVector.empty())
    {
        //cerr << "combinedQualityVector is empty." << endl;  // should not be empty => backtrack?
        return false; // Warning: can only return true or false. What return for error?
    }
    else
    {
        switch(log.back().abstractionLevel)
        {
            case 0: threshold = 0.90; break;
            case 1: threshold = 0.80; break;
            case 2: threshold = 0.75; break;
            case 3: threshold = 0.66; break;
            case 4: threshold = 0.60; break;
            default: threshold = 0.5; break;
        }
        //TODO!! add more layers here!

        // check Quality of current Puzzle Piece in  combinedQualityVector with Threshold value
        for (qualityVector::iterator it = cqVector.begin(); it != cqVector.end(); it++)
            if ((cqVector.back().first / log.back().abstractionLevel) >= threshold) // const threshold values
                // count how many Pieces are greater than the threshold value
                countHigherThreshold++;
            else
                if ((cqVector.back().first / log.back().abstractionLevel) > tempBest)
                    tempBest = cqVector.back().first;  // could be used, for additional constraints

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
    bool removePart=true;

    // check if both qualityVectors are not empty
    if(qVector.empty())
    {
        //cerr << "qualityVector is empty." << endl;  // should not be empty => backtrack?
        return;
    }
     if(cqVector.empty())
    {
        //cout << "combinedQualityVector was initialized." << endl;  //first layer stuff eh
        for(auto it:qVector)
            cqVector.emplace_back(it);
    }
     else
     {
         for (unsigned int i = 0; i < cqVector.size();) {
             for (unsigned int j = 0; j < qVector.size(); j++) {
                 // search same PuzzlePart of qualityVector and combinedQualityVector
                 removePart=true;
                 if (cqVector.at(i).second->GetPartID() == qVector.at(j).second->GetPartID() && cqVector.at(i).second->GetNumOfRotations() == qVector.at(j).second->GetNumOfRotations()) {
                     // sum Quality of PieceCollector (qualityVector) to combinedQualityVector
                     cqVector.at(i).first += qVector.at(j).first;
                     qVector.at(j).first = cqVector.at(i).first/DESTRUCTION_COUNT;
                     countSummarizedVectors++;
                     removePart=false;
                     break; // skip remaining for loop => save time!
                 }
             // remove element at poisition X in combinedQualityVector, because it was not summarized
             // inefficient way to delete element X
             //cqVector->erase(cqVector->begin()+i);
             // efficient way, but no sorted cqVector => wayne //echt? lol

             }
             if(removePart)
             {
                 swap(cqVector.at(i), cqVector.back());
                 cqVector.pop_back();
             } else i++;

         }

         // cqVector should have the same size now as newest qVector
         if (cqVector.size() != qVector.size()) {
             cerr << "Size of combinedQualityVector doenst match with size of qualityVector!" << endl;
             cout << "Size of combinedQualityVector: " << cqVector.size() << endl;
             cout << "Size of qualityVector: " << qVector.size() << endl;
             cout << "Size of countSummarizedVectors: " << countSummarizedVectors << endl;
         }
     }
}