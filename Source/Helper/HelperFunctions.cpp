//
// Created by mpapa on 19.11.2017.
//

#include "HelperFunctions.h"

/*
template <typename T>
static T ContinousShift(T bitStream, int shiftLength)
{
    if(shiftLength >= 0)
    {
        bitStream = bitStream << shiftLength | bitStream >> ((sizeof(T) * 4) - shiftLength);
    }
    else
    {
        bitStream = bitStream >> 5 | bitStream << ((sizeof(T) * 4) - shiftLength);
    }

    return bitStream;
}
*/

uint8_t HelperFunctions::ContinuousShift(uint8_t bitStream, int shiftLength)
{
    if(shiftLength >= 0)
    {
        bitStream = bitStream << shiftLength | bitStream >> (8 - shiftLength);
    }
    else
    {
        shiftLength = shiftLength * (-1);
        bitStream = bitStream >> shiftLength | bitStream << (8 - shiftLength);
    }

    return bitStream;
}
