//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_2_PROPERTIES_H
#define SOURCE_ABSTRACTIONLAYER_2_PROPERTIES_H

#include <stdint.h>
#include "AbstractionLayer_2.h"

class AbstractionLayer_2_Properties
{
public:
    AbstractionLayer_2_Properties() : m_farbe(0) {}
    int32_t GetColor() const;
    void SetColor(const int32_t farbe);
private:
    int32_t m_farbe;
    friend class AbstractionLayer_2;
};


#endif //SOURCE_ABSTRACTIONLAYER_2_PROPERTIES_H
