#pragma once

#include "AbstractionLayer_KantenLaenge_Properties.h"
#include "../AbstraktionLayer_Base.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class AbstractionLayer_KantenLaenge : public AbstractionLayer_Base<AbstractionLayer_KantenLaenge_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) final ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)final;
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_KantenLaenge_Properties constraint)final;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)final;

    float PlaceOfPartGood(coor myCoor, vector<double> myPart);

private:
};