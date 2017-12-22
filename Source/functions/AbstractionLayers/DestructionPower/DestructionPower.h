//
// Created by mpapa on 05.12.2017.
//

#pragma once

#define DESTRUCTION_COUNT 1

#include "DestructionPower_Properties.h"
#include "../AbstraktionLayer_Base.h"

#include <vector>
#include <iostream>
#include <bitset>
#include <random>

class DestructionPower : public AbstractionLayer_Base<DestructionPower_Properties>
{
public:
    void PreProcessing(coor mySize,const vector<Part*>* partArray)override;
    bool EvaluateQuality (coor constraintCoordinate, qualityVector& qVector) override;
    bool SetConstraintOnPosition(coor constraintCoordinate, AbstractionLayer_1_Properties constraint);
    bool RemoveConstraintOnPosition(coor constraintCoordinate)override;

    void DestructionOfSurrounding(coor constraintCoordinate);
    int getNextAbstractionLayer(coor newCoordinate, int currentAbstractionLayer);


private:
};
