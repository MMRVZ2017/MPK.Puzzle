//<editor-fold desc="All the includes and stuff">
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
//</editor-fold>
#include <fstream>
#include <string>

vector<double> differentiate(vector<double> input, int windowSize);
vector<double> movingAverage(vector<double> input, int windowSize);
double avg (vector<double> input);
int main() {
    for(int i = 12;i<30;i++){
        Mat image = readImage(i, "../images/output/blackAndWhite/output_BB/");
        //<editor-fold desc="Contour closing">
        Mat gray;
        cvtColor(image, gray, CV_BGR2GRAY);
        Mat canny;
        Canny(gray,canny,100,200,3);
       // imshow("Canny",canny);
        int morph_size = 4;
        Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
        //image = drawLargestContour(image,0,true);
        morphologyEx(canny,canny,MORPH_CLOSE,element);
        //imshow("Canny closed",canny);
        canny = drawLargestContour(canny,2,true);
        //imshow("Canny approx",canny);
        cvtColor(canny,image,CV_GRAY2BGR);
        Contour_t contours;
        Hierarchy_t hierarchy;
        //</editor-fold>
        //<editor-fold desc="Finding centroid of image">
        findContours(canny,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

        int index = getLongestContourIndex(contours);
        Moments m = moments(contours[index]);
        auto centerX = int(round(m.m10/m.m00));
        auto centerY = int(round(m.m01/m.m00));
        Point centroid = Point(centerX,centerY);
        circle(image, centroid, 4, Scalar(255,0,255), 2, 8);
        imshow("rect: ",image);
        //</editor-fold>
        //<editor-fold desc="Find edge points in polar coordinate system">
        vector<double> lx;
        for(size_t j = 0; j<contours[index].size();j++){
            Point contourPoint = contours[index][j];
            Point vec2Center = centroid-contourPoint;
            int d =  int(round(sqrt((vec2Center.x)*(vec2Center.x)+(vec2Center.y)*(vec2Center.y))));
            double distance =  d;
            lx.push_back(distance);

        }

        double maxL = findmax(lx);
        double l = maxL +5;
        vector<double> l_alpha;
        vector<Point> edge_alpha; // edge points as functions of alpha angle
        for(int j = 0;j<360;j++){
            Point endP;
            double angleRad = CV_PI*j/180; //  examining every 0.1 degree
            endP.x = int(round(centroid.x+l*cos(angleRad)));
            endP.y = int(round(centroid.y+l*sin(angleRad)));
            bool edgeFound = false;
            int t = 0;
            Point edgeP;
            while(!edgeFound){
                int pX = endP.x-int(round(t*cos(angleRad)));
                int pY = endP.y-int(round(t*sin(angleRad)));
                if(canny.at<uchar>(pY,pX) !=0){
                    edgeFound = true;
                    edgeP.x = pX;
                    edgeP.y = pY;
                }
                else{
                    t++;
                }
            }
            // Doesn'r have to be done for all 360
            // Find the contour point closest to the edge point
            // Only finds one point
            double min_diff = 3000.0;
            Point2f contour2Edge;
            cout<<"original edge P: " <<edgeP<<endl;

            //cout<<contours[index]<<endl;
            //cout<<contours[index].size();

            //cout<<edgeP<<endl;

            double edgeDist = sqrt((edgeP.x-centroid.x)*(edgeP.x-centroid.x)+(edgeP.y-centroid.y)*(edgeP.y-centroid.y));
            l_alpha.push_back(edgeDist);
            edge_alpha.push_back(edgeP);
            //line(image,centroid,endP,Scalar(0,255,255),1);
            //line(image,edgeP,endP,Scalar(255,200,100),1);
            circle(image,edgeP,2,Scalar(0,255,0),1);
            //TODO edgeP_(alpha) vector

        }
        //</editor-fold>
        //<editor-fold desc="Generating circular buffer">
        imshow("line",image);
        int startIndex = 100;
        vector<double> infiniteAlpha; // not really infinite, but has the last elements in the beginning as well for
        // moving average to work correctly
        for(size_t j = 0; j<startIndex;j++){
            // Putting the first 20 elements to the end
            l_alpha.push_back(l_alpha[j]);
        }
        cout<<"l_alpha.size: "<<l_alpha.size()<<endl;
        for(size_t j = 0; j < l_alpha.size()+startIndex;j++){
            if(j<startIndex){
                infiniteAlpha.push_back(l_alpha[l_alpha.size()-2*startIndex+j]);
            }
            else{
                infiniteAlpha.push_back(l_alpha[j-startIndex]);
            }
        }
        //</editor-fold>

        int windowSize = 20;
        vector<double>mAvg = movingAverage(infiniteAlpha, windowSize);
        startIndex-=windowSize;
        vector<double> diff_l = differentiate(mAvg,2);
        vector<double> diff2_l = differentiate(diff_l,2);
        // LETS FIND THOSE CORNERS:
        vector<Point> convexPoints;
        vector<Point> concavePoints;
        vector<Point> inPoints;
        vector<Point> outPoints;
        vector<Point> cornerPoints;
        double maxSlope = findmax(diff_l);
        double avgSlope = avg(diff_l);
        double avgDist = avg(mAvg);
        cout<<endl;
        for(int j = startIndex; j < diff_l.size()-startIndex; j++){
            int ind = j - startIndex-windowSize/2; // maybe shouldn't: -windowSize
            // if the sign changes from - to + or + to - (discrete nullposition)
            if(diff_l[j+1]*diff_l[j-1] < 0){ // if nullposition
                // Upward slope: concave
                if(diff2_l[j]>0) {
                   // must be inPoint or other concave -- purple:
                   circle(image, edge_alpha[ind], 3, Scalar(255, 0, 255), 3);
                    concavePoints.push_back(edge_alpha[ind]);

                }
                else // downward slope: convex
                {  // corner or outPoint, blue:
                    circle(image, edge_alpha[ind], 3, Scalar(255, 100, 0), 3);
                    convexPoints.push_back(edge_alpha[ind]);
                    /*
                    int wind=3;
                    double sum = 0;
                    for(int k = -wind; k<=wind;k++){
                        sum+=abs(diff_l[j+k]);
                    }
                    double avg = sum/(2*double(wind));


                    cout<<"angle: "<<ind<<endl;
                    double value = diff_l[j-6];
                    cout<<"avg: "<<avg<<endl;
                    if(abs(value) >0.15){ // than outPoint, blue
                       // circle(image, edge_alpha[ind], 3, Scalar(255, 100, 0), 3);
                    }
                    else // else corner, yellow
                       // circle(image, edge_alpha[ind], 3, Scalar(0, 255, 255), 3);
                       */
                }
                j++;

            }

        }// End of points sorting
        vector<int> indexes;
        for(int o = 0; o<convexPoints.size(); o++){
            Point cPoint = convexPoints[o];
            int cIndex = -1;
            for(size_t k = 0; k<contours[index].size();k++){

                // cout<<contours[index][k]<<endl;
               Point contour2Edge = Point2f(contours[index][k].x-cPoint.x, contours[index][k].y-cPoint.y);
                double distDiff =  sqrt((contour2Edge.x)*(contour2Edge.x)+(contour2Edge.y)*(contour2Edge.y));
                //cout<<"distDiff: "<<distDiff<<endl;
                // cout<<"min_diff: "<<min_diff<<endl;
                if(distDiff==0){
                    cPoint.x = contours[index][k].x;
                    cPoint.y = contours[index][k].y;
                    index = k;
                }

                // examine contour region and categorize


                indexes.push_back(cIndex);

            }
        }










        imshow("possible corners", image);



        //<editor-fold desc="Writing .csv file">
        // write for excel:
        std::ofstream fs;
        const char* filename = "../Source/exampleOutput.csv";
        fs.open(filename);
        for (size_t n=0; n<diff_l.size(); n++)
        {
            //cout <<l_alpha[n] <<endl;
            fs << infiniteAlpha[n]<<";"<<diff_l[n]<<";"<<diff2_l[n]<<endl;

        }
        fs.close();
        //</editor-fold>

        waitKey(0);
    }


}
double avg (vector<double> input){
    double sum = 0.0;
    for(int i = 0; i<input.size(); i++){
        sum+=input[i];
    }
    return sum/input.size();
}

vector<double> differentiate(vector<double> input, int windowSize){
    vector<double> diffV;
    diffV.resize(input.size());
    double diff;
    double h = 1;
    for(size_t i = 0; i<input.size();i++){
        if(i<h||i+int(h)>input.size()){
            diffV[i] = 0;
        }
        else
            diffV[i] = (input[i+h]-input[i-h])/(2*h);
    }


    return diffV;

}

vector<double> movingAverage(vector<double> input, int windowSize){
    double runningTotal = 0.0;
    int inputS = input.size();
    vector <double> movingAverage;
    for(size_t i = 0; i < input.size(); i++)
    {

        runningTotal += input[i];   // add
        if(i >= windowSize)
            runningTotal -= input[i - windowSize];   // subtract

        if(i >= (windowSize - 1)) {// output moving average
           // cout << "Your SMA: " << runningTotal / (double) windowSize;
            movingAverage.push_back((runningTotal / (double) windowSize));
        }
    }
    return movingAverage;
}
