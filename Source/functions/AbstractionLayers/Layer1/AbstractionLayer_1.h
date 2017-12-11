//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_H
#define SOURCE_ABSTRACTIONLAYER_1_H

#include "../AbstraktionLayer_Base.h"
#include "AbstractionLayer_1_Properties.h"
#include <vector>
#include <iostream>
#include <bitset>

class AbstractionLayer_1 : public AbstraktionLayer_Base<AbstractionLayer_1_Properties>
{
public:
    void PreProcessing(vector<Part_v2*>* partArray);
    propabilityVector EvaluetePropability (coor constraintCoordinate, propabilityVector inputVector);
    bool SetConstraintOnPosition(coor constraintCoordinate, AbstractionLayer_1_Properties constraint);
    bool RemoveConstraintOnPosition(coor constraintCoordinate);

private:
};

#endif //SOURCE_ABSTRACTIONLAYER_1_H
