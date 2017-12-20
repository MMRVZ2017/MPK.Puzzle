//
// Created by mpapa on 05.12.2017.
//

#ifndef SOURCE_DESTRUCTIONPOWER_PROPERTIES_H
#define SOURCE_DESTRUCTIONPOWER_PROPERTIES_H

#include <stdint.h>
#include "DestructionPower.h"

class DestructionPower_Properties
{
public:
    DestructionPower_Properties()
    {
        for(int i=0;i<sizeof(DestructionArray);i++)
            DestructionArray[i]=(SpeedTable[i]*DESTRUCTION_INIT);

        setSpeedTable();

    }
    map<int,float> getSpeedTable()
    {
        return SpeedTable;
    }
    void setSpeedTable()
    { SpeedTable=create_SpeedTable();}

private:

    static map<int,float> create_SpeedTable();
    static map<int,float> SpeedTable;

    float DestructionArray[DESTRUCTION_COUNT];

    friend class DestructionPower;

};

map<int,float> DestructionPower_Properties::create_SpeedTable()
{
    map<int, float> m;
    m[1] = 0.001;
    return m;
}

#endif //SOURCE_DESTRUCTIONPOWER_PROPERTIES_H
