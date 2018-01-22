#include "AbstractionLayer_PoempelPosition.h"
#include "../../../header.h"

bool AbstractionLayer_PoempelPosition::PreProcessing(coor mySize,  const vector<Part*>* partArray)
{
    //get layer information here
    cout << "Abstraction Poempel Position Preprocessing...  " << flush;

    InitialiseConstraintMatrixSize(mySize.col, mySize.row);

    cout << "Done" << endl;
    return true;
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool AbstractionLayer_PoempelPosition::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i = 0;i<qVector.size();i++)
    {
        float value = PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_a3.SideLength);
        qVector[i].first=value;
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

float AbstractionLayer_PoempelPosition::PlaceOfPartGood(coor myCoor, vector<double> myPart)
{
    vector<double> comparePosition={0,0,0,0,0,0,0,0};
    float sum=0;
    //create negativePart, watch out for edges
    if(myCoor.col>0 && m_constraintMatrix[myCoor.col-1][myCoor.row].SideLength[3])//get right data from left
    {
        comparePosition[6]=m_constraintMatrix[myCoor.col-1][myCoor.row].SideLength[3];
        comparePosition[7]=m_constraintMatrix[myCoor.col-1][myCoor.row].SideLength[2];
        //sum absolute difference
        sum+=abs(comparePosition[6]-myPart[6]);
        sum+=abs(comparePosition[7]-myPart[7]);
    }
    if(myCoor.row>0 && m_constraintMatrix[myCoor.col][myCoor.row-1].SideLength[5])//get bot data from top
    {
        comparePosition[0]=m_constraintMatrix[myCoor.col][myCoor.row-1].SideLength[5];
        comparePosition[1]=m_constraintMatrix[myCoor.col][myCoor.row-1].SideLength[4];
        //sum absolute difference
        sum+=abs(comparePosition[0]-myPart[0]);
        sum+=abs(comparePosition[1]-myPart[1]);

    }
    if(myCoor.col<m_constraintMatrix.size()-1 && m_constraintMatrix[myCoor.col+1][myCoor.row].SideLength[7])// get left data from right
    {
        comparePosition[2]=m_constraintMatrix[myCoor.col+1][myCoor.row].SideLength[7];
        comparePosition[3]=m_constraintMatrix[myCoor.col+1][myCoor.row].SideLength[6];
        //sum absolute difference
        sum+=abs(comparePosition[2]-myPart[2]);
        sum+=abs(comparePosition[3]-myPart[3]);

    }

    if(myCoor.row<m_constraintMatrix[0].size()-1 && m_constraintMatrix[myCoor.col][myCoor.row+1].SideLength[1])//get top data from bot
    {
        comparePosition[4]=m_constraintMatrix[myCoor.col][myCoor.row+1].SideLength[1];
        comparePosition[5]=m_constraintMatrix[myCoor.col][myCoor.row+1].SideLength[0];
        //sum absolute difference
        sum+=abs(comparePosition[4]-myPart[4]);
        sum+=abs(comparePosition[5]-myPart[5]);

    }
    if(sum>100)
        return 0;
    if(sum==0)
        return 1;
    sum/=100;
    return 1-sum;
    //check vector against negative part, use sad
    //return of well it fits within threshold
}

//shifts vector i steps to the right (8-2i to the left)
void AbstractionLayer_PoempelPosition_Properties::shift(int i)
{
    rotate(SideLength.begin(),SideLength.begin()+(8-2*i),SideLength.end());
}
