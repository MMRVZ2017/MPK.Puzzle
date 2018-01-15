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

using namespace cv;

// common std members:
using std::cout;
using std::cin;
using std::vector;
using std::endl;


int main(){


    int noRows = 15;
    int noCols = 15;
    vector<vector< SolutionElement>> solvedMx;
    solvedMx.resize(noRows);
    int index = 35; // random
    //Making a random 15x15 solution matrix, later this will be provided by the solution groups:
    for(int i = 0; i<noRows;i++){
        for(int j = 0; j<noCols;j++){
            SolutionElement element;
            element.index = index;
            element.orientation = 1;
            solvedMx[i].push_back(element);
            index++;
        }
    }

    // showing the result:
    const char * displayDir = "../images/input/input_display/"; // this is where the displayed pieces are stored in BW jpg format.
    int MonitorY = 600; // make it smaller than your monitors vertical resolution so the image fits the screen
    Mat solvedImage = resultImage(solvedMx, displayDir,600);
    Mat fitSolvedImage = resultImageCropped(solvedMx, displayDir,600);
    Mat smartSolvedImage = resultImageSmart(solvedMx, displayDir,600);
    imshow("result",solvedImage);
    imshow("fitResult", fitSolvedImage);
    imshow("smartResult", smartSolvedImage);
    waitKey(0);

    char outputFile1 [100];
    sprintf(outputFile1,"../images/output/solution1.jpg");
    imwrite(outputFile1,solvedImage);
    //Making another solution matrix:
    noRows = 3;
    noCols = 3;
    vector<vector< SolutionElement>> solve3x3; // This is the solutionmatrix that the solution groups will provide
    // Initializing the solution matrix with zeros: going trough every row and filling every element with zero
    solve3x3.resize(noRows);
    for(size_t i = 0; i < solve3x3.size(); i++){
        for(int j = 0; j < noCols; j++){
            SolutionElement element;
            element.index = 0;
            element.orientation = 0;
            solve3x3[i].push_back(element);
        }

    }
    // result written by hand. This will be automatically generated by the solution groups:
    solve3x3[0][0].index = 11; // ../images/input_display/11.jpg
    solve3x3[0][0].index = 11;
    solve3x3[0][1].index = 12; // the puzzle piece in .../12.jpg goes to the first row second column in the puzzle
    solve3x3[0][2].index = 13;
    solve3x3[1][0].index = 21;
    solve3x3[1][1].index = 22;
    solve3x3[1][2].index = 23;
    solve3x3[2][0].index = 31;
    solve3x3[2][1].index = 32;
    solve3x3[2][2].index = 33;

    // Set piece orientations:
    solve3x3[0][0].orientation = 0;
    solve3x3[0][0].orientation = 0;
    solve3x3[0][1].orientation = 0;
    solve3x3[0][2].orientation = 0; // just for demonstration
    solve3x3[1][0].orientation = 0;
    solve3x3[1][1].orientation = 0;
    solve3x3[1][2].orientation = 0;
    solve3x3[2][0].orientation = 0;
    solve3x3[2][1].orientation = 0;
    solve3x3[2][2].orientation = 0;

    // Recycling the solvedImage object by setting every pixel to zero, this is probably unnecessary:
    solvedImage = Mat::zeros(solvedImage.size(), CV_8UC3);

    // Displaying the second solution described by our 3x3 solution matrix:
    solvedImage = resultImage(solve3x3, displayDir,600);
    imshow("result",solvedImage);
    waitKey(0);

    // saving the result:
    char outputFile2 [100];
    sprintf(outputFile2,"../images/output/solution2.jpg");
    imwrite(outputFile2,solvedImage);

}



