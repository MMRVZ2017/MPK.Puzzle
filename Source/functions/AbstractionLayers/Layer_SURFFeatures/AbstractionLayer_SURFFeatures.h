#ifndef SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H
#define SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H

#include "../AbstraktionLayer_Base.h"

using namespace std;

class AbstractionLayer_SURFFeatures : public AbstractionLayer_Base<AbstractionLayer_SURFFeatures_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) ;
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_SURFFeatures_Properties constraint);
    bool RemoveConstraintOnPosition(const coor constraintCoordinate);

private:
};

#endif //SOURCE_ABSTRACTIONLAYER_SURFFEATURES_H
