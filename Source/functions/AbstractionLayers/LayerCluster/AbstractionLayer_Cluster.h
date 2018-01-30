//
// Created by Niko on 1/23/2018.
//

#ifndef PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_H
#define PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_H
#include "AbstractionLayer_Cluster_Properties.h"
#include "../AbstraktionLayer_Base.h"

#include <utility>
#include <vector>
#include <iostream>
#include <bitset>
#include <random>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <bitset>

#define DISPLAY true

#ifdef _WIN32
//#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#define PATH "C:\\Users\\Niko\\Desktop\\Technikum\\Semester 1\\MPK\\Projekt\\GitHubMMR\\MPK.Puzzle\\Source\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif

#define IMG_SIZE 400
#define TOP 6
#define RIGHT 4
#define BOTTOM 2
#define LEFT 0

using namespace std;
using namespace cv;

class AbstractionLayer_Cluster : public AbstractionLayer_Base<AbstractionLayer_Cluster_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) override ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)override;
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_Cluster_Properties constraint)override;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)override;
    bool PlaceOfPartGood(coor myCoor, Scalar& myPart);
    void shift(uint8_t& Part, int shifts);
    void setEdgeZero();


    void CreateRandomPuzzle();
    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();
    bool CalculateMeanDifference(Scalar Part, Scalar RefPart);

private:
};

class cCluster{
public:
    explicit cCluster(int s = 1008): nr_parts(s){}
    Mat readImages(int);
    bool assignClusterType(Mat Part, Mat RefPart);
    bool getImages();
private:
    int nr_parts;

};

#endif //PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_H
