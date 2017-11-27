
//Older but stable version, commited at 27/11/2017

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

//#include "header/input.h"
#include <math.h>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


//using namespace std;
using namespace cv;


// Constants:

const int noRows = 5;
const int noCols = 13;

//typedefs:

class SolutionElement
{
public:
    SolutionElement(): index(-1), orientation(-1){}
    ~SolutionElement() {}
    void setIndex(uint16_t);
    void setOrientation(uint8_t);

    uint16_t getIndex();
    uint8_t getOrientation();

protected:
    uint16_t index;  //x.jpg
    uint8_t orientation;    //CW rotation = orientation*90 (deg): 0,1,2,3
};

void SolutionElement::setIndex(uint16_t ind) {
    index = ind;
}
void SolutionElement::setOrientation(uint8_t alpha) {
    orientation = alpha;
}
uint16_t SolutionElement::getIndex() {
    return index;
}
uint8_t SolutionElement::getOrientation() {
    return orientation;
}

// Function Headers:
Mat segmentThresh(Mat src);
Mat segmentCanny(Mat src);
Mat segment(Mat src);
Mat correct(Mat src);
Mat processRawImage(int fileIndex = 0);
Mat resultImage();

int main() {

	//TODO: arrange into function f.e.: processAllImages();
	// Uncomment to process all of the images in the input folder:
	/* 
     // Process all images:
    int noPics = 1008;
    for(int i = 0; i<noPics;i++){
        Mat processed  =  processRawImage(i);
        char outputFile [50];
        sprintf(outputFile,"../corrected/corrected_%d.jpg",i);
        imwrite(outputFile,processed);
    }
	*/
	
	
	// Show the result 
	//TODO: Mat resultImage() => Mat resultImage(solutionMatrix)
    Mat res = resultImage();
    imshow("result",res);
    waitKey(0);
    


}

Mat processRawImage(int fileIndex){
    char inputFile [50];
    sprintf(inputFile,"../input/%d.jpg",fileIndex);

    Mat source = imread(inputFile,1);
    Mat mask = segment(source);
    Mat corrected = correct(mask);
    //imshow("output", mask);

    return corrected;

}

//Mat resultImage( SolutionElement solutionMx[noCols][noRows]){
Mat resultImage(){

    Mat result(500,1500,CV_8UC3);


    SolutionElement solutionMx[noCols][noRows];
    int index = 0;
    //Making a random solution matrix:
    for(int i = 0; i<noCols;i++){
        for(int j = 0; j<noRows;j++){
            solutionMx[i][j].setIndex(index);
            solutionMx[i][j].setOrientation(1);
            index++;
        }
    }

    int previousRows = 0;
    int previousCols = 0;
    for(int i = 0; i<noCols;i++){

        for(int j = 0; j<noRows;j++){
            int imageNumber = solutionMx[i][j].getIndex();
            Mat img = processRawImage(imageNumber);
            //img.copyTo(result.rowRange(i*20, j*20).colRange(i*20+img.cols, j*20+img.cols));
            Rect ROI(i*90, j*90, 85, 85);
            Mat temp; resize(img,temp, Size(ROI.width, ROI.height));
            temp.copyTo(result(ROI));

            //imshow("result",result);
            //waitKey(0);


        }
    }

    return result;
}


Mat segmentThresh(Mat src){
    bool loadfailed = false;
    if (!src.data || src.empty())
    {
        std::cout << "Problem loading image!!!" << std::endl;
    }
    else{
        loadfailed = true;
    }
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    // Convert image to binary
    Mat bw;
    threshold(gray,bw,190,255,CV_THRESH_BINARY_INV);
    // adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 5,2);
    //imshow("bw",bw);
    int padding = 100;
    copyMakeBorder(bw, bw, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    copyMakeBorder(src, src, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    int morph_size =2;
    // Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    Mat element = getStructuringElement(MORPH_ELLIPSE,Size (2,2));
    // Mat kernel = Mat::ones(1,1, CV_32F);
    morphologyEx(bw, bw, MORPH_CLOSE, element, Point(-1,-1), 3);

    //imshow("closed",bw);
    // Find all the contours in the thresholded image
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Calculate the area of each contour
        area = contourArea(contours[i]);
        // Ignore contours that are too small or too large. If true, than the loop starts again
        if (area < 1e2 || 1e5 < area) continue; //
        //if false:
        // Find the contour with the largest area:
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
    }


    //Now we know the index of the largest contour in the image
    std::vector<std::vector<Point> >  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;

    int strength = 3; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    Mat mask =  Mat::zeros(src.size(), CV_8UC3);
    drawContours(mask, largestContour,index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED

    return mask;
}


Mat segmentCanny(Mat src){
    bool loadfailed = false;
    if (!src.data || src.empty())
    {
        std::cout << "Problem loading image!!!" << std::endl;
    }
    else{
        loadfailed = true;
    }
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // Convert image to binary
    Mat bw;
    // findcontours works like this: white:foreground, black: background. This is why we INVERT first

    //threshold(gray, bw, 240, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU); // INV: invert for findcontours to work properly

   // threshold(gray,gray,190,255,THRESH_BINARY);
    //adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 41,20); //(31, 20) (41,20)
     threshold(gray,gray,200,255,THRESH_TOZERO);
    //imshow("gray",gray);
   // waitKey(0);
    //medianBlur(gray,gray,3);
    Canny(gray, bw, 150, 200, 3);
    int padding = 100;
    copyMakeBorder(bw, bw, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    copyMakeBorder(src, src, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));

    int morph_size =1;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

    //Mat element = getStructuringElement(MORPH_ELLIPSE,Size (4,4));

    morphologyEx(bw, bw, MORPH_CLOSE, element, Point(-1,-1), 1);
   // imshow("canny",bw);

    // Find all the contours in the thresholded image
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Calculate the area of each contour
        area = contourArea(contours[i]);
        // Ignore contours that are too small or too large. If true, than the loop starts again
        if (area < 1e2 || 1e5 < area) continue; //
        //if false:
        // Find the contour with the largest area:
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }

        // Draw each contour only for visualisation purposes
        //drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 3, 8, hierarchy, 0);
    }
    //imshow("contours", src);

    //Now we know the index of the largest contour in the image
    std::vector<std::vector<Point> >  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;

    int strength = 6; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    Mat mask =  Mat::zeros(src.size(), CV_8UC3);

    //drawContours(mask, contours, largestIndex, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED

    drawContours(mask, largestContour,index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    //imshow("mask", mask);
   // waitKey(0);
    //closing the gaps:

    //int border = 40;
    //copyMakeBorder(mask, mask, border, border, border, border, BORDER_CONSTANT, (0,0,0));
    // Create a structuring element (SE) for morphology
    morph_size =3;
    element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(mask, mask, MORPH_CLOSE, element, Point(-1,-1), 1);
    return mask;
}


Mat segment(Mat src){
    Mat copySrc = src;
    Mat threshMask = segmentThresh(src);
    Mat cannyMask = segmentCanny(copySrc);

    Mat threshBin;
    Mat cannyBin;
    cvtColor(threshMask,threshBin,CV_BGR2GRAY);
    cvtColor(cannyMask,cannyBin,CV_BGR2GRAY);
    threshold( threshBin, threshBin, 100,255,THRESH_BINARY );
    threshold( cannyBin, cannyBin, 100,255,THRESH_BINARY );

    int threshResult = countNonZero(threshBin);
    int cannyResult = countNonZero(cannyBin);
    /*
    std::cout<<"Threshresult: "<< threshResult<<std::endl;
    std::cout<<"cannyResult: "<< cannyResult<<std::endl;
    imshow("Thresh",threshMask);
    imshow("Canny", cannyMask);
    waitKey(0);
    */

    if(threshResult>cannyResult){
        return threshMask;
    }
    else{
        return cannyMask;
    }

    return threshMask;

}

Mat correct(Mat src) {
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // Convert image to binary
    Mat bw;
    threshold(gray, bw, 150, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); // INV: invert for findcontours to work properly
    // Find all the contours in the thresholded image
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Calculate the area of each contour
        area = contourArea(contours[i]);
        // Ignore contours that are too small or too large. If true, than the loop starts again
        if (area < 1e2 || 1e5 < area) continue; //
        //else:
        // Find the contour with the largest area:
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
       drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 3, 8, hierarchy, 0);
    }
    //Now we know the index of the largest contour in the image
    std::vector<std::vector<Point> >  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = 6; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    Mat mask =  Mat::zeros(bw.size(), CV_8UC3);
    drawContours(mask, largestContour, static_cast<int>(index), Scalar(255,255,255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED

    RotatedRect box;
    box = minAreaRect(largestContour[index]);

    Point2f rect_points[4];
    box.points( rect_points );
    double alpha = 0;

    //Draw rectagles:
    /*

    for( int j = 0; j < 4; j++ )
       line( mask, rect_points[j], rect_points[(j+1)%4], Scalar(0,255,0), 2, 8 );

    alpha =  atan2( (rect_points[1].x-rect_points[0].x) , (rect_points[1].y-rect_points[0].y)  );

    alpha = alpha*180/CV_PI;
    //std::cout<<alpha;
    */


    //Rotate conter clockwise by alpha:
    Point2f center;
    center.x = mask.cols/2;
    center.y = mask.rows/2;
    Mat RotMatrix = getRotationMatrix2D(center,int(-alpha),1);
    warpAffine(mask,mask,RotMatrix, mask.size());
    return mask;

}

