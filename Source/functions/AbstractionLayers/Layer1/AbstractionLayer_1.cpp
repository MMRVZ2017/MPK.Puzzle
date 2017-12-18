//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include <iostream>

void AbstractionLayer_1::PreProcessing(const vector<Part_v2*>* partArray)
{
    InitialiseConstraintMatrixSize(32, 28);

    m_constraintMatrix[0][0].m_connections = 0b00110011;

    partArray->at(0)->SetPartID(5);
    std::cout << partArray->at(0)->GetPartID() << endl << bitset<8>(m_constraintMatrix[0][0].m_connections).to_string() << endl;
}

bool AbstractionLayer_1::EvalueteQuality (const coor constraintCoordinate, qualityVector& qVector)
{

}

bool AbstractionLayer_1::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{

}

bool AbstractionLayer_1::RemoveConstraintOnPosition(const coor constraintCoordinate)
{

}

