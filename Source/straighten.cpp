//
// Created by Tamas on 2017-12-07.
//

//05-12-2017

// This cpp contains everything so far. Project isn't using header files yet

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

// int counter = 0;
const int noRows = 5;
const int noCols = 13;

int main() {
    for(int i=0;i<10;i++){
        Mat res = readImage(i,"../images/input/input_BB/");
        imshow("res",res);
         waitKey(0);
        //imshow("res",res);
        Mat segmentCheck = segment(res);
        int nonZeros =  countNonZero(segmentCheck);
        Mat straight;
        Mat alpha;
        if(nonZeros>2000){
            straight = correct(res, false); // without alpha channel
            alpha = correct(res, true); // with alpha channel
        }
        else{
            straight = Mat::zeros(res.size(),CV_8UC3);
            putText(straight, "Segmentation failed", cvPoint(120,120),
                    FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
            alpha = Mat::zeros(res.size(),CV_8UC4);
            putText(alpha, "Image segmentation failed", cvPoint(30,120),
                    FONT_HERSHEY_COMPLEX_SMALL,0.8, cvScalar(100,100,100),1,CV_AA);
        }



        char outputalpha [100];
        sprintf(outputalpha,"../images/output/transparent/output_BB/%d.png",i);
        imwrite(outputalpha, alpha);

        char outputFile [100];
        sprintf(outputFile,"../images/output/blackAndWhite/output_BB/%d.jpg",i);
        imwrite(outputFile,straight);
        // saveImage(i,res);

    }
    return 0;
}


