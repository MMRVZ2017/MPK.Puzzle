//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_SOLVE_H
#define SOURCE_SOLVE_H

#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1.h"
#include "../functions/AbstractionLayers/DestructionPower/DestructionPower.h"


using namespace std;

class LogEntry
{
public:
    map<Part*, float> PieceCollector;
    int abstractionLevel;
    coor myCoor;

    void advance(){abstractionLevel++;}
    void Set(){set=1;}
    bool isSet(){return set;}
    void advanceRandomed() { randomed++;}
    void decreaseRandomed() { randomed--;}
    int hasRandomed(){return randomed;}

    LogEntry()
    {
        myCoor = coor(0,0);
        abstractionLevel=0;
        set=0;
    }
private:
    bool set;
    static int randomed;
};

class Puzzle
{
public:
    Puzzle(unsigned int newcols,unsigned int newrows):rows(newrows),cols(newcols){}

    coor getSizeAsCoor()
    {return {cols,rows};}

    DestructionPower* dp;
    AbstractionLayer_1* a1;

    void removeConstrains(coor removeCoordinates);
    void printPuzzle();

    void createRandomBox(){createRandomPuzzle();putIntoBox();shuffle();}
    void createRandomPuzzle(){a1->CreateRandomPuzzle();}
    void putIntoBox();
    void shuffle();

    vector<Part> myBox;
    vector<Part*> p_myBox;

private:

    unsigned int rows;
    unsigned int cols;
};

class LayerContainer
{
public:
    AbstractionLayer_1_Properties m_test1;
    DestructionPower_Properties m_destruction;
};


bool next(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
coor calculateNextCoor(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void solve(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void setsolution(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
bool backtrack(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);

void createNextLogElement(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);

#endif //SOURCE_SOLVE_H
