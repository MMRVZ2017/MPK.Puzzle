//
// Created by mpapa on 05.12.2017.
//
/*
#ifndef SOURCE_ABSTRACTIONLAYER_2_H
#define SOURCE_ABSTRACTIONLAYER_2_H

#include "../AbstraktionLayer_Base.h"
#include "../../../header/Part.h"
#include <vector>

class AbstractionLayer_2 : AbstraktionLayer_Base
{
public:
    AbstractionLayer_2 (vector<Part_v2>* partArray) : m_partArray(partArray) {}

    void Test()
    {
        m_partArray->at(0).SetPartID(5);
        m_partArray->at(0).m_test1;
        m_partArray->at(0).m_test2;
    }

private:
    vector<Part_v2>* m_partArray;
};


#endif //SOURCE_ABSTRACTIONLAYER_2_H
*/