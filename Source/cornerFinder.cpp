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
#include "../Source/header/layer1.h"

using namespace cv;

// common std members:
using std::cout;
using std::cin;
using std::vector;
using std::endl;

#include <fstream>
#include <string>

int main() {
    for(int i = 0;i<1000;i++){
        // reading the image:
        char inputFile[100];
        
        // modify path and filename structure as needed:
        sprintf(inputFile,"../images/output/blackAndWhite/output_VZ/%d.jpg",i);
        Mat image = imread(inputFile,1); // 3 channel, but Black and white

        Mat segmentCheck = segment(image);
        int nonZeros =  countNonZero(segmentCheck);
        // skip to next iteration if the image is not processable (less then 4000 foreground pixels):
        if(nonZeros<4000){
            continue;
        }

        // Find the corners on the image:

        vector <PuzzlePoint> corners = findCorners(image);

        // display the corners on the image:
        for(int g = 0; g < corners.size();g++){
            cout<<g<<". corner: "<<corners[g].its_point<<endl;
            circle(image, corners[g].its_point, 10, Scalar(0, 200, 150), 4);
        }

        imshow("possible corners",  image);
        waitKey(0);

        //OPTIONAL: save images with marked corners:
        /*
        char outputFile [100];
        sprintf(outputFile,"../images/output/Abstract1/%d.jpg",i);
        imwrite(outputFile,image);
         */
    }
}



