//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H
#define SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H

#include <stdint.h>

class AbstractionLayer_1_Properties
{
public:
    AbstractionLayer_1_Properties() : m_connections(0b11111111) {}
    void shift(int shifts);
    void print();

private:
    uint8_t m_connections;
    friend class AbstractionLayer_1;
};





#endif //SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H
