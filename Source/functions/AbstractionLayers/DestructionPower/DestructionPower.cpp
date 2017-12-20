//
// Created by mpapa on 05.12.2017.
//

#include "DestructionPower.h"
#include <iostream>

void DestructionPower::PreProcessing(const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(32,28);
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool DestructionPower::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
}

bool DestructionPower::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{
}

bool DestructionPower::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
}

//gets destruction power from left and from top if possibe and normalizes
void DestructionPower::DestructionOfSurrounding(const coor constraintCoordinate) {
    float newDestructionArray[DESTRUCTION_COUNT];
    for (int i = 0; i < DESTRUCTION_COUNT; ++i) {
        int divisor=0;
        if(constraintCoordinate.row > 0)
        {
            divisor++;
            newDestructionArray[i] += m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row-1].m_destruction.DestructionArray[i];
        }
        if(constraintCoordinate.col > 0)
        {
            divisor++;
            newDestructionArray[i] += m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].m_destruction.DestructionArray[i];
        }
        if(divisor)
            newDestructionArray[i] /=divisor;
    }
}
