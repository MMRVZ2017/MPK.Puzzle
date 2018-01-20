#include "AbstractionLayer_PoempelPosition.h"
#include "../../../header.h"

bool AbstractionLayer_PoempelPosition::PreProcessing(coor mySize,  const vector<Part*>* partArray)
{
    //get layer information here
    cout << "Abstraction Poempel Position Preprocessing...  " << flush;


    InitialiseConstraintMatrixSize(mySize.col, mySize.row);

    cout << "Done" << endl;
    return false;
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool AbstractionLayer_PoempelPosition::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i = 0;i<qVector.size();i++)
    {
        float value = PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_a3.SideLength));
        if(value > 0.8)//TODO find threshold
        {
            qVector[i].first=value;
            continue;
        }
        qVector[i].first=0;
    }
}

bool AbstractionLayer_PoempelPosition::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_PoempelPosition_Properties constraint)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].SideLength=constraint.SideLength;
}

bool AbstractionLayer_PoempelPosition::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].SideLength={0,0,0,0,0,0,0,0};
}

float AbstractionLayer_PoempelPosition::PlaceOfPartGood(coor myCoor, vector<float> myPart)
{
    //sets coordinates to correct position for layer

    //create negativePart, watch out for edges

    //check vector against negative part

    //return of well it fits within threshold
}

//shifts vector i steps to the right (8-2i to the left)
void AbstractionLayer_PoempelPosition_Properties::shift(int i)
{
    rotate(SideLength.begin(),SideLength.begin()+(8-2*i),SideLength.end());
}
