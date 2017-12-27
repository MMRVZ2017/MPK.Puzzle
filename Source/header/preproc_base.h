//
// Created by Tamas on 2017-12-07.
//

#ifndef MPK_PUZZLE_PREPROC_BASE_H
#define MPK_PUZZLE_PREPROC_BASE_H
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <math.h>
#include <iostream>

using namespace cv;
// common std members:
using std::cout;
using std::cin;
using std::vector;
using std::endl;

typedef std::vector<std::vector<Point>> Contour_t;
typedef std::vector<Vec4i> Hierarchy_t; //hierarchy object is needed for drawContours apparently

class SolutionElement

{
public:
    SolutionElement(): index(-1), orientation(-1){}
    ~SolutionElement() {}
    uint16_t index;  //x.jpg
    uint8_t orientation;    //CW rotation = orientation*90 (deg): 0,1,2,3
};





Mat readImage(int fileIndex, const char* inputDir);
Mat segmentWS(Mat src);
Mat segmentThresh_bad(Mat src);
Mat drawLargestContour(Mat, int, bool);
Mat segmentThresh(Mat);
Mat segmentCanny(Mat);
Mat segment(Mat);
Mat correct(Mat, bool = false);
Mat processRawImage(int fileIndex = 0); // depracated by readimage
Mat processRawImageBB(int fileIndex = 0); // deprecated by readimage
Mat saveImage(int , Mat); // deprecated (wasn't working in the first place)
Mat resultImage( vector<vector< SolutionElement>> solutionMx);
int findOrientation(Mat);
Contour_t getLongestContour(Mat); //  depracated by getLongestContourIndex
int getLongestContourIndex(Contour_t);

int findmax(std::vector<int> vals);
int findmin(std::vector<int> vals);
float findmax(std::vector<float> vals);
float findmin(std::vector<float> vals);
double findmin(std::vector<double> vals);
double findmax(std::vector<double> vals);
std::vector<int> histogram(std::vector<int> data, int rangeWidth); // discrete histogram
std::vector<int> cont_histogram(std::vector<int> data);
void drawHistogram(vector<int> hist);


#endif //MPK_PUZZLE_PREPROC_BASE_H