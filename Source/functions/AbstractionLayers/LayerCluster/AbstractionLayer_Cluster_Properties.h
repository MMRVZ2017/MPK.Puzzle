//
// Created by Niko on 1/23/2018.
//

#ifndef PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_PROPERTIES_H
#define PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_PROPERTIES_H

#include <stdint.h>

class AbstractionLayer_Cluster_Properties
{
public:
    AbstractionLayer_Cluster_Properties() : clusterType(-1) {}
    int getClusterType(){return clusterType;};
    void print();
    cv::Scalar meanValue;
private:
    int clusterType;
    friend class AbstractionLayer_Cluster;
    Mat image;
};

#endif //PUZZLESOLVER_ABSTRACTIONLAYER_CLUSTER_PROPERTIES_H
