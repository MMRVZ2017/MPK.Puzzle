//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_H
#define SOURCE_ABSTRACTIONLAYER_1_H

#include "AbstractionLayer_1_Properties.h"
#include "../AbstraktionLayer_Base.h"

#include <vector>
#include <iostream>
#include <bitset>
#include <random>

class AbstractionLayer_1 : public AbstractionLayer_Base<AbstractionLayer_1_Properties>
{
public:
    void PreProcessing(coor mySize, const vector<Part*>* partArray) final;
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint);
    bool RemoveConstraintOnPosition(const coor constraintCoordinate);
    bool PlaceOfPartGood(coor myCoor, uint8_t& myPart);
    void shift(uint8_t& Part, int shifts);
    void setEdgeZero();


    void CreateRandomPuzzle();
    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();

private:
};

#endif //SOURCE_ABSTRACTIONLAYER_1_H
