//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include <iostream>

/*
    void Test()
    {
        m_partArray->at(0).SetPartID(5);
        m_partArray->at(0).m_test1.m_connections = 0b00110011;
        cout << m_partArray->at(0).GetPartID() << endl << bitset<8>(m_partArray->at(0).m_test1.m_connections).to_string() << endl;
        //Gibt einen Fehler, wenn man auf eine Eigenschaft einer anderen Layer zugreifen will
        //m_partArray->at(0).m_test2.GetColor();
    }
    */

void AbstractionLayer_1::PreProcessing(vector<Part_v2*>* partArray)
{
    InitialiseConstraintMatrixSize(32, 28);

    m_constraintMatrix[0][0].m_connections = 0b00110011;

    partArray->at(0)->SetPartID(5);
    std::cout << partArray->at(0)->GetPartID() << endl << bitset<8>(m_constraintMatrix[0][0].m_connections).to_string() << endl;
}

propabilityVector AbstractionLayer_1::EvaluetePropability (propabilityVector inputVector)
{

}
