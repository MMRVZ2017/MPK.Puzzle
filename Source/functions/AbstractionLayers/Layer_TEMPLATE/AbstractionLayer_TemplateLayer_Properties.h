
#ifndef SOURCE_ABSTRACTIONLAYER_TEMPLATELAYER_H
#define SOURCE_ABSTRACTIONLAYER_TEMPLATELAYER_H          //%TODO: Change include guards (Replace TEMPLATELAYER)

#include <stdint.h>
                                                         //TODO: Replace every occurrence of "TemplateLayer" in this file with Layer Name


class AbstractionLayer_TemplateLayer_Properties
{
public:
    AbstractionLayer_TemplateLayer_Properties() {}


private:
    int m_template_feature;                              //TODO: add private member variables to store relevant information for your Layer for each position or part
                                                         //TODO: (used in ConstraintMatrix and Part Vector)
    friend class AbstractionLayer_TemplateLayer;
};



#endif //SOURCE_ABSTRACTIONLAYER_TEMPLATELAYER_H
