#ifndef SOURCE_PART_H
#define SOURCE_PART_H

#include <stdint.h>
#include "../functions/AbstractionLayers/Layer1/AbstractionLayer_1_Properties.h"
#include "../functions/AbstractionLayers/Layer2/AbstractionLayer_2_Properties.h"

class Part_v2
{
public:
    Part_v2() : m_partID(0) {}
    ~Part_v2() {}

    int32_t GetPartID () const
    {
        return m_partID;
    }

    void SetPartID(int partID)
    {
        m_partID = partID;
    }

    AbstractionLayer_1_Properties m_test1;
    AbstractionLayer_2_Properties m_test2;

private:
    int32_t m_partID;
};

#endif //SOURCE_PART_H