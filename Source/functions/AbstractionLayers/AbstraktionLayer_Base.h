//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRAKTIONLAYER_BASE_H
#define SOURCE_ABSTRAKTIONLAYER_BASE_H

#include <map>
#include "../../header/Part.h"

typedef vector<map<Part_v2*, int>> propabilityVector;

template<typename T>
class AbstraktionLayer_Base
{
public:
    virtual void PreProcessing(vector<Part_v2*>* partArray) = 0;
    virtual propabilityVector EvaluetePropability (propabilityVector inputVector) = 0;

    void InitialiseConstraintMatrixSize(int32_t collumns, int32_t rows)
    {
        m_constraintMatrix = vector<vector<T>>(collumns, vector<T>(rows));
    }

    vector<vector<T>> m_constraintMatrix;
};


#endif //SOURCE_ABSTRAKTIONLAYER_BASE_H
