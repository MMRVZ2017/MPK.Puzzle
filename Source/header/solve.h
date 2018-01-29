#pragma once

#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1.h"
#include "../functions/AbstractionLayers/Layer3_PoempelPosition/AbstractionLayer_PoempelPosition.h"
#include "../functions/AbstractionLayers/Layer_SURFFeatures/AbstractionLayer_SURFFeatures.h"
#include "../functions/AbstractionLayers/Layer_ColorMatching/AbstractionLayer_ColorMatching.h"
#include "../functions/AbstractionLayers/DestructionPower/DestructionPower.h"

using namespace std;

class LogEntry
{
public:
    qualityVector PieceCollector;
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

struct combinedQualityTop2 {
    Part* qualityID;
    float quality1 = 0.0;
    float quality2 = 0.0;
};

class Puzzle
{
public:

    Puzzle(unsigned int newcols,unsigned int newrows):cols(newcols),rows(newrows) {}

    bool PreProcessing()
    {
        createBox(); createp_box();
        if(!dp.PreProcessing({cols,rows}, nullptr))  return false;
        if(!a1.PreProcessing({cols,rows}, &p_myBox)) return false;
        if(!a3.PreProcessing({cols,rows}, &p_myBox)) return false;
        if(!a4.PreProcessing({cols,rows}, &p_myBox)) return false;
        if(!acm.PreProcessing({cols,rows}, &p_myBox)) return false;

        return true;
    }

    coor getSizeAsCoor() {return {cols,rows};}

    DestructionPower dp;
    AbstractionLayer_1 a1;
    AbstractionLayer_PoempelPosition a3;
    AbstractionLayer_SURFFeatures a4;
    AbstractionLayer_ColorMatching acm;

    void removeConstrains(coor removeCoordinates);
    void setConstraints(coor setConstraints, Part *constraintPiece);
    int removeSimilar(qualityVector&, Part&);

    void printPuzzle();
    void printBox();
    Mat resultImage(vector<LogEntry>&);

    void createRandomBox(){
        myBox.clear();p_myBox.clear();
        createRandomPuzzle();putIntoBox();
        printPuzzle();shuffle();createp_box(); clearMat();}
    void createRandomPuzzle();
    void putIntoBox();
    void shuffle();
    void createBox();
    void createp_box();
    void clearMat();

    bool allSet();

    vector<Part> myBox;
    vector<Part*> p_myBox;

    qualityVector combinedQualityVector;
    vector<combinedQualityTop2> combinedQualityTop2Vector;

    vector<float> tmp_destructionArray;

private:

    Mat readImage(int fileIndex, const char* inputDir);
    unsigned int cols;
    unsigned int rows;
};

bool next(vector<LogEntry>& log,Puzzle& puzzleMat);
coor calculateNextCoor(vector<LogEntry>& log, Puzzle& puzzleMat);
void solve(vector<LogEntry>& log, Puzzle& puzzleMat);
void setsolution(vector<LogEntry>& log, Puzzle& puzzleMat);
bool backtrack(vector<LogEntry>& log,Puzzle& puzzleMat);

void createNextLogElement(vector<LogEntry>& log,Puzzle& puzzleMat);

