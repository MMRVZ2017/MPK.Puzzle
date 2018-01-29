#include "DestructionPower.h"

//TODO! Add more layers here!

//sets relations of speed for the different layers
map<int,float> DestructionPower_Properties::SpeedTable =
        {
                {0,0.99},
                {1,0.7},
                {2,0.7},
                {3,0.5}
        };

bool DestructionPower::PreProcessing(coor mySize,const vector<Part*>* partArray)
{
    cout << "DestructionPower Preprocessing...  ";

    InitialiseConstraintMatrixSize(mySize.col,mySize.row);

    for(auto& it:m_constraintMatrix)
        for(auto& ti:it)
            for(int i=0;i<DESTRUCTION_COUNT;i++)
                ti.DestructionArray.push_back(0);
    cout << "Done!" << endl;
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
    for(int i=0;i<m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray.size();i++)
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i]=0;
}

//gets destruction power from left and from top if possible and normalizes
void DestructionPower::DestructionOfSurrounding(const coor constraintCoordinate) {

   for (int i = 0; i < DESTRUCTION_COUNT; ++i) {
        int divisor=0;
       //check if not at edge and if information is valid
        if(constraintCoordinate.row > 0 && m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row-1].DestructionArray[i] > 0)
        {
            divisor++;
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row-1].DestructionArray[i];
        }
        if(constraintCoordinate.col > 0 && m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].DestructionArray[i] > 0)
        {
            divisor++;
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].DestructionArray[i];
        }
       if(constraintCoordinate.col < m_constraintMatrix.size()-1 && m_constraintMatrix[constraintCoordinate.col+1][constraintCoordinate.row].DestructionArray[i] > 0)
       {
           divisor++;
           m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col+1][constraintCoordinate.row].DestructionArray[i];
       }
       if(constraintCoordinate.row < m_constraintMatrix[i].size()-1 && m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row+1].DestructionArray[i] > 0)
       {
           divisor++;
           m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] += m_constraintMatrix[constraintCoordinate.col-1][constraintCoordinate.row].DestructionArray[i];
       }
        if(divisor)
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] /=divisor;
        else
            //create default destructionPower //TODO find some better solution for default
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i] = m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].SpeedTable[i];
        //aging
        if(m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i]<0.9)
            m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i]=m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].DestructionArray[i]*(float)1.001+(float)0.01;
    }
}

//gets next highest valued abstraction layer down from current one (if first, get highest)
int DestructionPower::getNextAbstractionLayer(coor newCoordinate, int currentAbstractionLayer)
{
    //hardcode advance
//    if(currentAbstractionLayer<DESTRUCTION_COUNT-1)
//        return ++currentAbstractionLayer;
//    return -1;

    if(currentAbstractionLayer==-1)
        return 0;//poempel in out
    float currentPower = 1;
    int nextLayer=-1;
    float nextLayerPower=0;
    if (currentAbstractionLayer>=0)
        currentPower = m_constraintMatrix[newCoordinate.col][newCoordinate.row].DestructionArray[currentAbstractionLayer];

    int i=0;

    //giff next most valuable layer
    for(auto it:m_constraintMatrix[newCoordinate.col][newCoordinate.row].DestructionArray)
    {
        if(it <= currentPower)
        {//if equal, then has to be the next one (activated from left to right)
            if(it == currentPower) {
                if (i > currentAbstractionLayer)
                    return i;
            }
                //if this one is bigger than previous biggest one, save
            else if(it>nextLayerPower)
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
        DestructionArray.back()<0.8 ? DestructionArray.back()=aging*DestructionArray.back()+(float)0.01:DestructionArray.back();
    }
}
