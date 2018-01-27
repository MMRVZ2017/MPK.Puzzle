//
// Created by mpapa on 05.12.2017.
//

#pragma once

#include <cstdint>
#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1_Properties.h"
#include "../functions/AbstractionLayers/DestructionPower/DestructionPower_Properties.h"
#include "../functions/AbstractionLayers/Layer3_PoempelPosition/AbstractionLayer_PoempelPosition_Properties.h"
#include "../functions/AbstractionLayers/Layer_ColorMatching/AbstractionLayer_ColorMatching_Properties.h"
#include "../functions/AbstractionLayers/Layer_SURFFeatures/AbstractionLayer_SURFFeatures_Properties.h"

class LayerContainer;

class Part
{
public:
    Part() : m_partID(0), m_numOfRotations(0), set(false)
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

    bool set;
    AbstractionLayer_1_Properties m_a1;
    AbstractionLayer_PoempelPosition_Properties m_a3;
    AbstractionLayer_SURFFeatures_Properties m_a4;
    AbstractionLayer_ColorMatching_Properties m_acm;
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
