//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H
#define SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H

#include <stdint.h>
#include "AbstractionLayer_1.h"

class AbstractionLayer_1_Properties
{
public:
    AbstractionLayer_1_Properties() : m_connections(0b00000000) {}
    uint8_t GetConnections() const;
    void SetConnections(const uint8_t connections);
private:
    uint8_t m_connections;
    friend class AbstractionLayer_1;
};


#endif //SOURCE_ABSTRACTIONLAYER_1_PROPERTIES_H
