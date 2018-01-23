
#ifndef SOURCE_ABSTRACTIONLAYER_1_COLORMATCHING_H
#define SOURCE_ABSTRACTIONLAYER_1_COLORMATCHING_H

#include <stdint.h>
#include "AbstractionLayer_ColorMatching.h" //TODO: Compiler loop!?

class AbstractionLayer_ColorMatching_Properties
{
public:
    AbstractionLayer_ColorMatching_Properties() {}


private:
    HSV centerColor;
    HSV partColor
    friend class AbstractionLayer_ColorMatching;
};



#endif //SOURCE_ABSTRACTIONLAYER_1_COLORMATCHING_H
