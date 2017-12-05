//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_H
#define SOURCE_ABSTRACTIONLAYER_1_H

#include "../AbstraktionLayer_Base.h"
#include "../../../header/Part.h"
#include <vector>
#include <iostream>
#include <bitset>

class AbstractionLayer_1 : AbstraktionLayer_Base
{
public:
    AbstractionLayer_1 (vector<Part_v2>* partArray) : m_partArray(partArray) {}

    void Test()
    {
        m_partArray->at(0).SetPartID(5);
        m_partArray->at(0).m_test1.m_connections = 0b00110011;
        cout << m_partArray->at(0).GetPartID() << endl << bitset<8>(m_partArray->at(0).m_test1.m_connections).to_string() << endl;
        //Gibt einen Fehler, wenn man auf eine Eigenschaft einer anderen Layer zugreifen will
        //m_partArray->at(0).m_test2.GetColor();
    }

private:
    vector<Part_v2>* m_partArray;
};


#endif //SOURCE_ABSTRACTIONLAYER_1_H
