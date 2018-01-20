#pragma once

#include "AbstractionLayer_PoempelPosition_Properties.h"
#include "../AbstraktionLayer_Base.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class AbstractionLayer_PoempelPosition : public AbstractionLayer_Base<AbstractionLayer_PoempelPosition_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) final ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)final;
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_PoempelPosition_Properties constraint)final;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)final;

    float PlaceOfPartGood(coor myCoor, vector<float> myPart);

private:
};