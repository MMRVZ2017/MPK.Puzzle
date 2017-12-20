#ifndef SOURCE_SOLVE_H
#define SOURCE_SOLVE_H

#include <vector>
#include <iostream>
#include "input.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

class coor
{
public:
    unsigned int col, row;
    coor(int newcol=-1,int newrow=-1): col(newcol), row(newrow)
    {}
};

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
        myCoor = coor();
        abstractionLevel=0;
        set=0;
    }
private:
    bool set;
    static int randomed;
};

bool next(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
coor calculateNextCoor(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void solve(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
void setsolution(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);
bool backtrack(vector<LogEntry>& log, vector<Part*>& p_Box, puzzleMat);

void createNextLogElement(vector<LogEntry>& log, vector<Part*>& p_Box, Puzzle& puzzleMat);

#endif //SOURCE_SOLVE_H