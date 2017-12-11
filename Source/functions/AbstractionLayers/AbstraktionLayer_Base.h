//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRAKTIONLAYER_BASE_H
#define SOURCE_ABSTRAKTIONLAYER_BASE_H

#include <map>
#include "../../header/Part.h"
#include <vector>

using namespace std;

typedef vector<map<Part_v2*, float>> propabilityVector;

template<typename T>
class AbstraktionLayer_Base
{
public:
    virtual void PreProcessing(vector<Part_v2*>* partArray) = 0;
    virtual propabilityVector EvaluetePropability (coor constraintCoordinate, propabilityVector inputVector) = 0;
    virtual bool SetConstraintOnPosition(coor constraintCoordinate, T constraint) = 0;
    virtual bool RemoveConstraintOnPosition(coor constraintCoordinate) = 0;

    virtual void InitialiseConstraintMatrixSize(int32_t collumns, int32_t rows)
    {
        m_constraintMatrix = vector<vector<T>>(collumns, vector<T>(rows));
    }

    vector<vector<T>> m_constraintMatrix;
};


#endif //SOURCE_ABSTRAKTIONLAYER_BASE_H
