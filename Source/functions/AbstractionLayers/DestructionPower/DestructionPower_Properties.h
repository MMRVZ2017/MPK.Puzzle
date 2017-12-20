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
        for(int i=0;i<DestructionArray.size();i++)
            DestructionArray[i]=(map[i]*DESTRUCTION_INIT);

    }
private:
    float DestructionArray[DESTRUCTION_COUNT];
    static map<int,float> SpeedTable = create_SpeedTable;
    friend class DestructionPower;

    static map<int,float> create_SpeedTable(){
        map<int, float> m;
        m[1] = 0.001;
        return m;
    }
};


#endif //SOURCE_DESTRUCTIONPOWER_PROPERTIES_H
