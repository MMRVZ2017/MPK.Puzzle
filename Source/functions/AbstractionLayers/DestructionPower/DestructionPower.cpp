//
// Created by mpapa on 05.12.2017.
//

#include "DestructionPower.h"

//TODO! Add more layers here!
map<int,float> DestructionPower_Properties::SpeedTable =
        {
                {1,0.001}
        };


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
            newDestructionArray[i] += m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row-1].DestructionArray[i];
        }
        if(constraintCoordinate.col > 0)
        {
            divisor++;
            newDestructionArray[i] += m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].DestructionArray[i];
        }
        if(divisor)
            newDestructionArray[i] /=divisor;
    }
}

DestructionPower_Properties::DestructionPower_Properties() {
    float aging=1.001;
    for(int i=0;i<DestructionArray.size();i++)

    {
        DestructionArray.emplace_back((DestructionPower_Properties::SpeedTable[i]*DESTRUCTION_INIT));
        DestructionArray.back()<0.99 ? DestructionArray.back()*=aging:DestructionArray.back();
    }
}