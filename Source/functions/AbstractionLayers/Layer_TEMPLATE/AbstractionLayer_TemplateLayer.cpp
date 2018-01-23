
#include "AbstractionLayer_TemplateLayer.h"     //TODO: Replace every occurrence of "TemplateLayer" in this file with Layer Name
#include "../../../header.h"                    //TODO: add AbstractionLayer_TemplateLayer.cpp to CMakeList
#include <iostream>



bool AbstractionLayer_TemplateLayer::PreProcessing(coor mySize, const vector<Part*>* partArray)
{
    //TODO: your implementation (see AbstractionLayer_Base.h for further descriptions)
}

bool AbstractionLayer_TemplateLayer::EvaluateQuality (coor constraintCoordinate, qualityVector& qVector)
{
    //TODO: your implementation (see AbstractionLayer_Base.h for further descriptions)
}

bool AbstractionLayer_TemplateLayer::SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_TemplateLayer_Properties constraint)
{
    //TODO: your implementation (see AbstractionLayer_Base.h for further descriptions)
}

bool AbstractionLayer_TemplateLayer::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    //TODO: your implementation (see AbstractionLayer_Base.h for further descriptions)
}