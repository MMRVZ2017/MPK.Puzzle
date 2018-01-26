#ifndef SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H
#define SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H

#include "../AbstraktionLayer_Base.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#ifdef _WIN32
#define PATH_FULL_PUZZLE "..\\..\\..\\puzzle_img\\puzzle1.jpg"
#elif defined __unix__
#define PATH_FULL_PUZZLE "..//..//..//puzzle_img//puzzle1.jpg"
#elif defined __APPLE__
    #define PATH_FULL_PUZZLE "..//..//..//puzzle_img//puzzle1.jpg"
#endif

#ifdef _WIN32   //TODO: Code duplicate from AbstractionLayer_1.h
#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif


using namespace std;

class AbstractionLayer_SURFFeatures : public AbstractionLayer_Base<AbstractionLayer_SURFFeatures_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) ;
    bool PreProcessingFullImg(coor mySize) ;
    bool PreProcessingPieces(coor mySize, const vector<Part*>* partArray) ;
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_SURFFeatures_Properties constraint);
    bool RemoveConstraintOnPosition(const coor constraintCoordinate);

private:
};

#endif //SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H
