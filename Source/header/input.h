//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_INPUT_H
#define SOURCE_INPUT_H


#include <stdint.h>

#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1_Properties.h"
#include "../functions/AbstractionLayers/DestructionPower/DestructionPower_Properties.h"

class LayerContainer;

class Part
{
public:
    Part() : m_partID(0) {}
    ~Part() {}

    int32_t GetPartID () const
    {
        return m_partID;
    }

    void SetPartID(const int32_t partID)
    {
        m_partID = partID;
    }

    uint8_t GetNumOfRotations () const
    {
        return m_numOfRotations;
    }

    void SetNumOfRotations(const uint8_t numOfRotations)
    {
        m_numOfRotations = numOfRotations;
    }

    LayerContainer* myLayers;
private:
    int32_t m_partID;
    uint8_t m_numOfRotations;
};


class coor
{
public:
    unsigned int col, row;
    coor(unsigned int newcol=-1,unsigned int newrow=-1): col(newcol), row(newrow)
    {}
};

#endif //SOURCE_INPUT_H
