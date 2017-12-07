//
// Created by Tamas on 2017-12-07.
//


#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#include <vector>
#include <math.h>
#include <iostream>

#include "../Source/header/preproc_base.h"
//Constants:

using namespace cv;

// common std members:
using std::cout;
using std::cin;
using std::vector;
using std::endl;

int noRows = 5;
int noCols = 5;

//typedefs:


int main(){


    noRows = 15;
    noCols = 15;
    vector<vector< SolutionElement>> solvedMx;
    solvedMx.resize(noRows);
    int index = 35; // random

    //Making a random 5x5 solution matrix:
    for(int i = 0; i<noRows;i++){
        for(int j = 0; j<noCols;j++){
            SolutionElement element;
            element.index = index;
            element.orientation = 1;
            solvedMx[i].push_back(element);
            index++;
            // cout<<"index: "<<index<<endl;
        }
    }

    // showing the result:
    Mat solvedImage = resultImage(solvedMx);
    imshow("result",solvedImage);
    waitKey(0);


    //Making another solution matrix:
    noRows = 3;
    noCols = 3;
    vector<vector< SolutionElement>> solve3x3;
    solve3x3.resize(noRows);
    for(size_t i = 0; i < solve3x3.size(); i++){
        for(int j = 0; j < noCols; j++){
            SolutionElement element;
            element.index = 0;
            element.orientation = 0;
            solve3x3[i].push_back(element);
        }

    }
    // result written by hand:
    solve3x3[0][0].index = 11;
    solve3x3[0][1].index = 12;
    solve3x3[0][2].index = 13;
    solve3x3[1][0].index = 21;
    solve3x3[1][1].index = 22;
    solve3x3[1][2].index = 23;
    solve3x3[2][0].index = 31;
    solve3x3[2][1].index = 32;
    solve3x3[2][2].index = 33;


    solvedImage = Mat::zeros(solvedImage.size(), CV_8UC3);
    solvedImage = resultImage(solve3x3);
    imshow("result",solvedImage);
    waitKey(0);

    char outputFile [100];
    sprintf(outputFile,"../solution/solution.jpg");
    imwrite(outputFile,solvedImage);

}




