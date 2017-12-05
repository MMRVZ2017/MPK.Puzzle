//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_2_Properties.h"

int32_t AbstractionLayer_2_Properties::GetColor() const
{
    return m_farbe;
}

void AbstractionLayer_2_Properties::SetColor(const int32_t farbe)
{
    m_farbe = farbe;
}
