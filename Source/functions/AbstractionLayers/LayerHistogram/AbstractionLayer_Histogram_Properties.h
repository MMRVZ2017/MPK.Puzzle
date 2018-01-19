//
// Created by Niko on 1/11/2018.
//

#ifndef MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_PROPERTIES_H
#define MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_PROPERTIES_H
#include <stdint.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
class AbstractionLayer_Histogram_Properties
{
public:
    AbstractionLayer_Histogram_Properties() : Correlation(-1){}
    double getCorrelation(){return Correlation;}
    Mat getmHistogram(){return m_Histogram;}

private:

    double Correlation;
    Mat m_Histogram;
    Mat image;
    friend class AbstractionLayer_Histogram;


};
#endif //MPK_PUZZLE_ABSTRACTIONLAYER_HISTOGRAM_PROPERTIES_H

