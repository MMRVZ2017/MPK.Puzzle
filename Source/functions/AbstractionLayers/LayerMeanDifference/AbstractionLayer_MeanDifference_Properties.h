//
// Created by Niko on 1/15/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
class AbstractionLayer_MeanDifference_Properties
{
public:
    AbstractionLayer_MeanDifference_Properties(): m_MeanDifference(-1.0), meanValue(0.1){}
    double getmMeanDifference(){return m_MeanDifference;}
    Mat image;
    cv::Scalar meanValue;
private:

    double m_MeanDifference;

    friend class AbstractionLayer_MeanDifference;


};
#endif //MPK_PUZZLE_ABSTRACTIONLAYER_MEANDIFFERENCE_PROPERTIES_H
