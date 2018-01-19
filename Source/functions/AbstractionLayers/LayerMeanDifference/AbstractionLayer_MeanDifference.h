//
// Created by Niko on 1/15/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
#define DISPLAY false
#define PATH "..\\..\\..\\pieces\\%04d.jpg"

using namespace std;
using namespace cv;

class AbstractionLayer_MeanDifference : public AbstractionLayer_Base<AbstractionLayer_MeanDifference_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) override ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)override;
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_1_Properties constraint)override;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)override;
    bool PlaceOfPartGood(coor myCoor, Mat& myPart);

    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();

private:
};

class cMeanDifference{
public:
    Mat readImages(int);
    bool calculateMeanDifference(Mat Part, Mat RefPart);
private:

};
#endif //MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_H
