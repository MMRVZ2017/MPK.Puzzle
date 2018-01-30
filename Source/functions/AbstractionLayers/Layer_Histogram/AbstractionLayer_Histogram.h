//
// Created by Niko on 1/11/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_H
#define DISPLAY false
//#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "AbstractionLayer_Histogram_Properties.h"
#include "../AbstraktionLayer_Base.h"

using namespace std;
using namespace cv;

class AbstractionLayer_Histogram : public AbstractionLayer_Base<AbstractionLayer_Histogram_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) override ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)override;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)override;
    bool PlaceOfPartGood(coor myCoor, Mat& myPart);
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_Histogram_Properties constraint)override;
    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();
    bool CompareHistogram(Mat Part, Mat RefPart);

private:
};

class HistogramComparer{
public:
    explicit HistogramComparer(int s = 1008): nr_parts(s){}
    Mat readImages(int);
    bool getImages();
private:
    int nr_parts;

};

#endif //MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_H

