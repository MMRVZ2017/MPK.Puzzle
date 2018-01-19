//
// Created by Niko on 1/15/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H

class AbstractionLayer_MeanDifference_Properties
{
public:
    AbstractionLayer_MeanDifference_Properties() : MeanDifference(-1){}
    double getMeanDifference(){return MeanDifference;};

private:

    double MeanDifference;
    friend class AbstractionLayer_MeanDifference;
    Mat image;
};
#endif //MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H
