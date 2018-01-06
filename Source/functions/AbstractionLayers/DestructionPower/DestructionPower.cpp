//
// Created by mpapa on 05.12.2017.
//

#include "DestructionPower.h"

//TODO! Add more layers here!
map<int,float> DestructionPower_Properties::SpeedTable =
        {
                {1,0.001}
        };


bool DestructionPower::PreProcessing(coor mySize,const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(mySize.row,mySize.col);

    return true;
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool DestructionPower::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
}

bool DestructionPower::SetConstraintOnPosition(const coor constraintCoordinate, const DestructionPower_Properties constraint)
{
}

bool DestructionPower::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
}

//gets destruction power from left and from top if possible and normalizes
void DestructionPower::DestructionOfSurrounding(const coor constraintCoordinate) {
    float newDestructionArray[DESTRUCTION_COUNT];

    for (int i = 0; i < DESTRUCTION_COUNT; ++i) {
        m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray.push_back(0);
        int divisor=0;
        if(constraintCoordinate.row > 0)
        {
            divisor++;
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row-1].DestructionArray[i];
        }
        if(constraintCoordinate.col > 0)
        {
            divisor++;
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].DestructionArray[i];
        }
        if(divisor)
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] /=divisor;
        else
            //create default destructionPower //TODO find some better solution for default
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] =1-m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].SpeedTable[i+1];
    }
}

float DestructionPower::defaultDestructionPower(int i)
{

}

//gets next highest valued abstraction layer down from current one (if first, get highest)
int DestructionPower::getNextAbstractionLayer(coor newCoordinate, int currentAbstractionLayer)
{
    float currentPower = 1;
    int nextLayer=-1;
    float nextLayerPower=0;
    if (currentAbstractionLayer>=0)
        currentPower = m_constraintMatrix[newCoordinate.row][newCoordinate.col].DestructionArray[currentAbstractionLayer];

    int i=0;

    //giff next most valuable layer
    for(auto it:m_constraintMatrix[newCoordinate.row][newCoordinate.col].DestructionArray)
    {
        if(it <= currentPower)
        {//if equal, then has to be the next one (activated from left to right)
            if(it == currentPower)
                if(i>currentAbstractionLayer)
                    return i;
            //if this one is bigger than previous biggest one, save
            if(it>nextLayerPower)
            {
                nextLayerPower=it;
                nextLayer=i;
            }
        }
        i++;
    }
    return nextLayer;
}

DestructionPower_Properties::DestructionPower_Properties() {
    float aging=1.001;
    for(int i=0;i<DestructionArray.size();i++)

    {
        DestructionArray.emplace_back((DestructionPower_Properties::SpeedTable[i]*DESTRUCTION_INIT));
        DestructionArray.back()<0.99 ? DestructionArray.back()*=aging:DestructionArray.back();
    }
}
