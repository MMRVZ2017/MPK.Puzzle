
#ifndef SOURCE_ABSTRACTIONLAYER_ColorMatching_H
#define SOURCE_ABSTRACTIONLAYER_ColorMatching_H

#include "AbstractionLayer_ColorMatching.h"
#include "../AbstraktionLayer_Base.h"

//#include <utility>
#include <vector>
#include <iostream>
//#include <bitset>
//#include <random>
#include "opencv2/highgui/highgui.hpp" //TODO: Check if Path works for us
#include "opencv2/imgproc/imgproc.hpp"
//#include <cstdio>
//#include <cstdlib>
#include <cmath>


//#define DISPLAY false

#ifdef _WIN32
#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif

//#define IMG_SIZE 400
//#define TOP 6
//#define RIGHT 4
//#define BOTTOM 2
//#define LEFT 0

using namespace std;
using namespace cv;

class AbstractionLayer_ColorMatching : public AbstractionLayer_Base<AbstractionLayer_ColorMatching_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_1_Properties constraint){}
    bool RemoveConstraintOnPosition( coor constraintCoordinate){}

    qualityVector returnInBox(vector<Part>& PuzzleBox);

private:
};

class HSV
{
public:
    HSV(): h(0.0), s(0.0), v(0.0){}
    double h, s,v;
};



#endif //SOURCE_ABSTRACTIONLAYER_ColorMatching_H
