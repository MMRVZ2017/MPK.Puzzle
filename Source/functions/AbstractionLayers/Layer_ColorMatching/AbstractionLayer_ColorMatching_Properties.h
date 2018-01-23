
#ifndef SOURCE_ABSTRACTIONLAYER_COLORMATCHING_H
#define SOURCE_ABSTRACTIONLAYER_COLORMATCHING_H

#include <stdint.h>

class HSV
{
public:
    HSV(): h(0.0), s(0.0), v(0.0){}
    double h, s,v;
};



class AbstractionLayer_ColorMatching_Properties
{
public:
    AbstractionLayer_ColorMatching_Properties() {}


private:
    HSV m_centerColor;
    HSV m_partColor;
    friend class AbstractionLayer_ColorMatching;
};



#endif //SOURCE_ABSTRACTIONLAYER_COLORMATCHING_H
