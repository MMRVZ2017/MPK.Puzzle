#ifndef SOURCE_PART_H
#define SOURCE_PART_H

#include <stdint.h>
#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1_Properties.h"

class Part_v2
{
public:
    Part_v2() : m_partID(0) {}
    ~Part_v2() {}

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

    AbstractionLayer_1_Properties m_test1;

private:
    int32_t m_partID;
    uint8_t m_numOfRotations;
};

#endif //SOURCE_PART_H