//
// Created by mpapa on 05.12.2017.
//
#pragma once
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

    explicit LogEntry(coor newCoor = coor(0,0)): myCoor(newCoor){
        abstractionLevel=-1;
        set=false;
    }
private:
    bool set;
    static int randomed;
};

class Puzzle
{
public:

    Puzzle(unsigned int newcols,unsigned int newrows):rows(newrows),cols(newcols)
    {
        a1.PreProcessing({rows,cols}, nullptr);
    }

    coor getSizeAsCoor()
    {return {cols,rows};}

    DestructionPower dp;
    AbstractionLayer_1 a1;

    void removeConstrains(coor removeCoordinates);
    void printPuzzle();
    void printBox();

    void createRandomBox(){createRandomPuzzle();putIntoBox();shuffle();createp_box();}
    void createRandomPuzzle();
    void putIntoBox();
    void shuffle();
    void createp_box();

    vector<Part> myBox;
    vector<Part*> p_myBox;

private:

    unsigned int rows;
    unsigned int cols;
};

bool next(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
coor calculateNextCoor(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void solve(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void setsolution(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
bool backtrack(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);

void createNextLogElement(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);

