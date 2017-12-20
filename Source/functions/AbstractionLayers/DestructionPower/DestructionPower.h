//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_H
#define SOURCE_ABSTRACTIONLAYER_1_H

#define DESTRUCTION_INIT 0.5
#define DESTRUCTION_COUNT 1

#include "../AbstraktionLayer_Base.h"
#include "DestructionPower_Properties.h"
#include <vector>
#include <iostream>
#include <bitset>
#include <random>

class DestructionPower : public AbstraktionLayer_Base<DestructionPower_Properties>
{
public:
    void PreProcessing(const vector<Part*>* partArray);//override
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint);
    bool RemoveConstraintOnPosition(const coor constraintCoordinate);

    void DestructionOfSurrounding(const coor constraintCoordinate);

private:
};

#endif //SOURCE_ABSTRACTIONLAYER_1_H
