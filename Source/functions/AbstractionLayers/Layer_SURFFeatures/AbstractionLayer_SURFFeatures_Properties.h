#ifndef SOURCE_ABSTRACTIONLAYER_SURFFEATURES_PROPERTIES_H
#define SOURCE_ABSTRACTIONLAYER_SURFFEATURES_PROPERTIES_H

#include <stdint.h>

class AbstractionLayer_SURFFeatures_Properties
{
public:
    AbstractionLayer_SURFFeatures_Properties() {}

private:
    uint16_t m_numberOfFeaturesDetected;
    friend class AbstractionLayer_SURFFeatures;
};

#endif //SOURCE_ABSTRACTIONLAYER_SURFFEATURES_PROPERTIES_H
