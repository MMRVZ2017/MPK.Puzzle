//
// Created by mpapa on 05.12.2017.
//

#pragma once

#include <cstdint>

#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1_Properties.h"
#include "../functions/AbstractionLayers/DestructionPower/DestructionPower_Properties.h"

class LayerContainer;

class Part
{
public:
    Part() : m_partID(0), m_numOfRotations(0)
    {}
    ~Part() = default;

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

    void print(){m_a1.print();}

    AbstractionLayer_1_Properties m_a1;
    DestructionPower_Properties m_destruction;
private:
    int32_t m_partID;
    uint8_t m_numOfRotations;
};


class coor
{
public:
    unsigned int col, row;
    coor (unsigned int newcol,unsigned int newrow): col(newcol), row(newrow)
    {}
};
