//
// Created by Niko on 1/15/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
#define DISPLAY true
//#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#define PATH "C:\\Users\\Niko\\Desktop\\Technikum\\Semester 1\\MPK\\Projekt\\GitHubMMR\\MPK.Puzzle\\Source\\pieces\\%04d.jpg"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "AbstractionLayer_MeanDifference_Properties.h"
#include "../AbstraktionLayer_Base.h"

using namespace std;
using namespace cv;

class AbstractionLayer_MeanDifference : public AbstractionLayer_Base<AbstractionLayer_MeanDifference_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) override ;
    bool EvaluateQuality(coor constraintCoordinate, qualityVector& qVector) override;
    bool RemoveConstraintOnPosition( coor constraintCoordinate) override;
    bool PlaceOfPartGood(coor myCoor, Scalar& myPart);
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_MeanDifference_Properties constraint)override;
    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();
    bool CalculateMeanDifference(Scalar Part, Scalar RefPart);
private:
};

class cMeanDifference{
public:
    explicit cMeanDifference(int s = 1008): nr_parts(s){}
    Mat readImages(int);
    bool getImages();
private:
    int nr_parts;

};

#endif //MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
