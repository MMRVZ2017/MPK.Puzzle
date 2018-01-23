//
// Created by Niko on 1/23/2018.
//
#include "AbstractionLayer_Cluster.h"
#include "../../../header.h"

bool AbstractionLayer_Cluster::PreProcessing(coor mySize,  const vector<Part*>* partArray){
    cv::Scalar mean, stddev;
    cMeanDifference analyse(mySize.row*mySize.col);
    cout << "Abstraction 4 Preprocessing...  " << flush;
    const vector<Part *> &ref_partArray = *partArray;
    Part buf;
    int iterator = 0;
    if (!analyse.getImages()) {
        cerr << "Error occured in getImages!" << endl;
        return false;
    } else // hier werden alle vier verschiedenen Rotationsarten 'gleichzeitig' abgespeichert
        //TODO rows and cols

        for (int i = 0; i < mySize.row * mySize.col; i++) {
            Mat src_img1 = analyse.readImages(i);
            meanStdDev(src_img1, mean, stddev, Mat());
            ref_partArray[iterator]->m_aCluster.meanValue = mean;
            iterator++;

        }
    InitialiseConstraintMatrixSize(mySize.col, mySize.row); //col row switched in this function

    cout << "Done!" << endl;
    return true;
}

bool AbstractionLayer_Cluster::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector){

    //evaluateQuality = evaluateProbabilaty
    for(int i = 0;i < qVector.size();i++)
    {
        if(PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_aCluster.meanValue))
        {
            qVector[i].first=1;

            continue;
        }
        qVector[i].first=0;
    }

}
bool AbstractionLayer_MeanDifference::PlaceOfPartGood(coor myCoor, Scalar &myPart) {

    //sets coordinates to correct position for layer
    myCoor.row++;
    myCoor.col++;

    if (myCoor.row == 1 && myCoor.col == 1) { return true; }
    else if (myCoor.col == 1 && myCoor.row > 1) {
        if (CalculateMeanDifference(m_constraintMatrix[myCoor.col][myCoor.row - 1].meanValue, myPart)) {
            return true;
        } else return false;
    } else if (myCoor.row == 1 && myCoor.col > 1) {
        if (CalculateMeanDifference(m_constraintMatrix[myCoor.col - 1][myCoor.row].meanValue, myPart)) {
            return true;
        } else return false;
    } else if (myCoor.col > 1 && myCoor.row > 1) {
        if (CalculateMeanDifference(m_constraintMatrix[myCoor.col][myCoor.row - 1].meanValue, myPart) &&
            CalculateMeanDifference(m_constraintMatrix[myCoor.col - 1][myCoor.row].meanValue, myPart)) {
            return true;
        } else return false;
    } else return false;

}

Mat cCluster::readImages(int count)
{
    char name[100];
    Mat corr;
    sprintf(name, PATH, count);
    Mat src = imread(name, 1);
    namedWindow("UP1", WINDOW_AUTOSIZE);
    imshow("UP1",src);
    if (!src.data)
    {
        cerr << "Problem loading image!!!" << endl;
        return src;
    }

    if(DISPLAY)imshow("src",src);

    Mat im_color;
    cvtColor(src, im_color, COLOR_BGR2HSV);
    return im_color;

}
bool cCluster::getImages() {

    Mat src;

    for (int i = 0; i < nr_parts; i++) {
        if (DISPLAY) cout << "Bild " << i << endl;
        Mat img = readImages(i);

        if (!img.data) {
            cerr << "Error: No pic found!!" << endl;
            return false;
        }
        else return true;
    }
}

bool AbstractionLayer_Cluster::CalculateMeanDifference(Scalar mean1, Scalar mean2) {
    float tmpMean[] = {0, 0, 0};

    for (int i = 0; i < 3; i++) {
        if (mean1[i] == 0) {
            mean1[i] = 0.1;
        }
        if (mean2[i] == 0) {
            mean2[i] = 0.1;
        }
    }
    if (mean1[0] <= mean2[0]) {
        tmpMean[0] = abs((1 - (mean2[0] / mean1[0])) * 100);
    }
    if (mean1[0] >= mean2[0]) {
        tmpMean[0] = abs((1 - (mean1[0] / mean2[0])) * 100);
    }
    if (mean1[1] <= mean2[1]) {
        tmpMean[1] = abs((1 - (mean2[1] / mean1[1])) * 100);
    }
    if (mean1[1] >= mean2[1]) {
        tmpMean[1] = abs((1 - (mean1[1] / mean2[1])) * 100);
    }
    if (mean1[2] <= mean2[2]) {
        tmpMean[2] = abs((1 - (mean2[2] / mean1[2])) * 100);
    }
    if (mean1[2] >= mean2[2]) {
        tmpMean[2] = abs((1 - (mean1[2] / mean2[2])) * 100);
    }
    if (((tmpMean[0] + tmpMean[1] + tmpMean[2]) / 3) < 5.0) {

        return true;
    } else return false;
}