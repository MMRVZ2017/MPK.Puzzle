
#ifndef SOURCE_ABSTRACTIONLAYER_ColorMatching_H
#define SOURCE_ABSTRACTIONLAYER_ColorMatching_H

#include "AbstractionLayer_ColorMatching.h"
#include "../AbstraktionLayer_Base.h"

//#include <utility>
#include <vector>
#include <iostream>
//#include <bitset>
//#include <random>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <cstdio>
//#include <cstdlib>
#include <cmath>



#ifdef _WIN32
#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif


using namespace std;
using namespace cv;

class AbstractionLayer_ColorMatching : public AbstractionLayer_Base<AbstractionLayer_ColorMatching_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) ;
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_ColorMatching_Properties constraint){}
    bool RemoveConstraintOnPosition(const coor constraintCoordinate){}

    qualityVector returnInBox(vector<Part>& PuzzleBox);

private:
};




#endif //SOURCE_ABSTRACTIONLAYER_ColorMatching_H
