// same as Abstract1.cpp as of 28-12-2017


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

#include <fstream>
#include <string>
//</editor-fold>



// typedefs:

class PuzzlePoint{
public:
    PuzzlePoint(const Point & p, const vector<Point> & c){

        its_cornerity = 0.0;
        its_point = p;
        its_index = -1;
        its_linR = {0.0,0.0,0.0,0.0};
        its_linL = {0.0,0.0,0.0,0.0};
        its_lin  = {0.0,0.0,0.0,0.0};
        its_contour = c;
        float minDiff = 1000.0;
        // Find the index of the contour points
        its_linFactor = 0.2;
        for(int k = 0; k<its_contour.size();k++){

            Point2f contour2Edge = Point2f(its_contour[k].x-float(its_point.x), its_contour[k].y-float(its_point.y));
            float distDiff =  sqrt((contour2Edge.x)*(contour2Edge.x)+(contour2Edge.y)*(contour2Edge.y));
            if(distDiff<minDiff){
                minDiff = distDiff;
                its_index = k;
            }
        }
    }
    Point its_point;
    void findCornerity(int noPoints); // fits lines to the right and left side of the point, using noPoints amount of points
    Vec4d its_lin;
    Vec4d its_linR;
    Vec4d its_linL;
    vector<Point> its_contour;
    double its_cornerity;
    int its_index;
    double its_linFactor;
    bool operator < (const  PuzzlePoint & other)
    {
        return its_cornerity < other.its_cornerity;
    }
};

double parallelity(Vec4d lin1, Vec4d lin2);

// function headers:

int closestPoint(vector<PuzzlePoint> points, Point p);

bool intersection(Vec4d l1, Vec4d l2, Point2d & r);

int findNeighbour(const vector<PuzzlePoint> & points, const PuzzlePoint & point, bool left);

int closest2Line(const vector<PuzzlePoint> & points, Vec4d lin);

double pointLength(Point2d point);

vector<double> differentiate(vector<double> input, int windowSize);

vector<double> movingAverage(vector<double> input, int windowSize);

double avg (vector<double> input);

RotatedRect getBaseRect(const Mat &);

Mat drawRect(const RotatedRect& rectangle, Mat src);

double lineFitNess(Vec4d lin, vector<Point> points, Point cPoint);

void displayLine_(Mat image, Vec4d lin, double length);

int main() {
    for(int i = 0;i<1000;i++){
        cout<<"i: "<<i<<endl;
        Mat image = readImage(i, "../images/output/blackAndWhite/output_VZ/");
        //imshow("image",image);
        Mat segmentCheck = segment(image);
        int nonZeros =  countNonZero(segmentCheck);
        if(nonZeros>4000){
        }
        else{
            continue;
        }


        //<editor-fold desc="Contour closing">
        Mat gray;
        int morph_size;
        Mat element;
        cvtColor(image, gray, CV_BGR2GRAY);
        Contour_t c; Hierarchy_t h;
        findContours(gray,c,h,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        double mSize = c.size()/30;
//
//        int morph_size =int(round(mSize));
//        cout<<"msize: "<<morph_size<<endl;
//        Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
//        morphologyEx(gray,gray,MORPH_CLOSE,element);
//        morphologyEx(gray,gray,MORPH_CLOSE,element);
//        morph_size = int(round(mSize)/50);
//        element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ),Point(morph_size, morph_size ));
//        morphologyEx(gray,gray,MORPH_OPEN,element);
//        morphologyEx(gray,gray,MORPH_OPEN,element);
//        gray =  drawLargestContour(gray,0,true);


        Mat canny;
        Canny(gray,canny,100,200,3);
        // imshow("Canny",canny);
        morph_size = 4;
        element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
        //image = drawLargestContour(image,0,true);
        morphologyEx(canny,canny,MORPH_CLOSE,element);
        //imshow("Canny closed",canny);
        canny = drawLargestContour(canny,0,true);
        //imshow("Canny approx",canny);
        //further simplification

        cvtColor(canny,image,CV_GRAY2BGR);

        //</editor-fold>

        //<editor-fold desc="Finding centroid of image">
        Contour_t contours;
        Hierarchy_t hierarchy;

        findContours(canny,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

        int index = getLongestContourIndex(contours);
        Moments m = moments(contours[index]);
        auto centerX = int(round(m.m10/m.m00));
        auto centerY = int(round(m.m01/m.m00));
        Point centroid = Point(centerX,centerY);
        circle(image, centroid, 4, Scalar(255,0,255), 2, 8);
        //imshow("rect: ",image);
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

            double edgeDist = sqrt((edgeP.x-centroid.x)*(edgeP.x-centroid.x)+(edgeP.y-centroid.y)*(edgeP.y-centroid.y));
            l_alpha.push_back(edgeDist);
            edge_alpha.push_back(edgeP);
            //line(image,centroid,endP,Scalar(0,255,255),1);
            //line(image,edgeP,endP,Scalar(255,200,100),1);
            // circle(image,edgeP,2,Scalar(0,255,0),1);
            //TODO edgeP_(alpha) vector

        }
        //</editor-fold>

        //<editor-fold desc="Generating circular buffer">
        // imshow("line",image);
        int startIndex = 100;
        vector<double> infiniteAlpha; // not really infinite, but has the last elements in the beginning as well for
        // moving average to work correctly
        for(size_t j = 0; j<startIndex;j++){
            // Putting the first 20 elements to the end
            l_alpha.push_back(l_alpha[j]);
        }
        for(size_t j = 0; j < l_alpha.size()+startIndex;j++){
            if(j<startIndex){
                infiniteAlpha.push_back(l_alpha[l_alpha.size()-2*startIndex+j]);
            }
            else{
                infiniteAlpha.push_back(l_alpha[j-startIndex]);
            }
        }
        //</editor-fold>

        //<editor-fold desc="Applying moving average edge points">
        // for moving average:
        //TODO make windowSize adaptive:
        int windowSize = 7;

        vector<double>mAvg = movingAverage(infiniteAlpha, windowSize);
        vector<double> diff_l = differentiate(mAvg,3);
        vector<double> diff2_l = differentiate(diff_l,3);
        vector<Point> convexPoints;
        vector<Point> concavePoints;
        vector<Point> inPoints;
        vector<Point> outPoints;
        vector<Point> cornerPoints;
        double maxSlope = findmax(diff_l);
        double avgSlope = avg(diff_l);
        double avgDist = avg(mAvg);
        //</editor-fold>

        //<editor-fold desc="Point sorting (convex or concave)">
        for(int j = startIndex; j < diff_l.size()-startIndex; j++){
            int ind = j - startIndex; // maybe shouldn't: -windowSize
            // if the sign changes from - to + or + to - (discrete nullposition)
            if(diff_l[j+1]*diff_l[j-1] < 0) { // if nullposition
                double middleIndex = 0.0;
                while (diff_l[j + middleIndex+ 1] * diff_l[j + middleIndex - 1] < 0) {
                    middleIndex++;
                }
                ind += int(round(middleIndex / 2));

                // Upward slope: concave
                if (diff2_l[j] > 0) {
                    // must be inPoint or other concave -- purple:
                    circle(image, edge_alpha[ind], 3, Scalar(255, 0, 255), 3);
                    concavePoints.push_back(edge_alpha[ind]);

                } else // downward slope: convex
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



                    double value = diff_l[j-6];

                    if(abs(value) >0.15){ // than outPoint, blue
                       // circle(image, edge_alpha[ind], 3, Scalar(255, 100, 0), 3);
                    }
                    else // else corner, yellow
                       // circle(image, edge_alpha[ind], 3, Scalar(0, 255, 255), 3);
                       */
                }
                j+=middleIndex;


            }// end of nullstelles
        }// End of points sorting
        //</editor-fold> )

        // line fitting
        vector<double> lineFits;
        vector<int> indexes;
        // Examining each convex Point:
        //<editor-fold desc="Fitting lines to convex points-- finding the corner and male connector candidates">
        vector<PuzzlePoint> puzzlePoints;
        for(int o = 0; o<convexPoints.size(); o++){
            Point cPoint = convexPoints[o];
            PuzzlePoint puzzP(convexPoints[o],contours[index]);
            double wSize = contours[index].size()/35;
            puzzP.findCornerity(int(round(wSize)));
            // cout<<lin<<endl;
            //<editor-fold desc="display the line">
            Vec4i displayLine;
            double wind = wSize;
            displayLine[0] = int(round(puzzP.its_lin[0]*wind+cPoint.x));
            displayLine[1] = int(round(puzzP.its_lin[1]*wind+cPoint.y));
            displayLine[2] = int(round(cPoint.x)-puzzP.its_lin[0]*wind);
            displayLine[3] = int(round(cPoint.y)-puzzP.its_lin[1]*wind);
            //line(image,Point(displayLine[2],displayLine[3]),Point(displayLine[0],displayLine[1]),Scalar(0,255,200),2);
            //vector<double> dist2Lins;
            //</editor-fold>
            displayLine_(image, puzzP.its_linR,wSize);
            displayLine_(image, puzzP.its_linL,wSize);
            puzzlePoints.push_back(puzzP);
            // examine contour region and categorize

        }// End of line fitting to convex points
        //</editor-fold>
        std::sort(puzzlePoints.begin(), puzzlePoints.end());
        vector<Point> rectPoints;
        if(puzzlePoints.size() == 4){
            for(int k = 0; k<4;k++){
                circle(image, puzzlePoints[k].its_point, 10, Scalar(200, 100, 100), 4);
            }
        }
        else {

            for (int j = 0; j < puzzlePoints.size(); j++) {
                //cout<<"itscornerity: "<<puzzlePoints[j].its_cornerity<<endl;
                // clecer algorithm that finds all of the remaining corners:
                ///////////////////////////////////////////////////
                char str[200];
                sprintf(str, " no. %d", j);
                cout << str << endl;
                putText(image, str, puzzlePoints[j].its_point, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255, 255));
                //circle(image,sortedConvex[j],20,Scalar(0,0,255),2);
            }
            //int closestIndexR = closest2Line(puzzlePoints,puzzlePoints[0].its_linR);
            // circle(image,puzzlePoints[closestIndexR].its_point,10,Scalar(200,100,100),4);


            circle(image, puzzlePoints[0].its_point, 10, Scalar(200, 100, 100), 4);
            int closestIndexL = findNeighbour(puzzlePoints, puzzlePoints[0], true);
            circle(image, puzzlePoints[closestIndexL].its_point, 10, Scalar(200, 100, 100), 4);
            cout<<"left: "<< puzzlePoints[closestIndexL].its_point<<endl;
            int closestIndexR = findNeighbour(puzzlePoints, puzzlePoints[0], false);
            circle(image, puzzlePoints[closestIndexR].its_point, 10, Scalar(200, 100, 100), 4);
            cout<<"right: "<< puzzlePoints[closestIndexR].its_point<<endl;
            Point2d intersectPoint;
            bool success = intersection(puzzlePoints[closestIndexR].its_linR, puzzlePoints[closestIndexL].its_linL,
                                        intersectPoint);
            Point intP(int(round(intersectPoint.x)), int(round(intersectPoint.y)));
            int lastCorner = closestPoint(puzzlePoints, intP);
            circle(image, puzzlePoints[lastCorner].its_point, 10, Scalar(200, 100, 100), 4);
            cout<<"last: "<<puzzlePoints[lastCorner].its_point<<endl;
            //circle(image,intP,10,Scalar(200,100,100),4);

            // drawing box around first for corner candidates:
            for (int j = 0; j < 4; j++) {
                rectPoints.push_back(puzzlePoints[j].its_point);
            }
            RotatedRect minRect = minAreaRect(rectPoints);
            Point2f rect_points[4];
            minRect.points(rect_points);
            for (int j = 0; j < 4; j++){}
            // line( image , rect_points[j], rect_points[(j+1)%4], Scalar(255,255,0), 3, 8 );
        }

        //imshow("possible corners",  image);
        char outputFile [100];
        sprintf(outputFile,"../images/output/Abstract1/%d.jpg",i);
        imwrite(outputFile,image);
        //<editor-fold desc="Writing .csv file">
        /*
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
        */
        //waitKey(0);
    }


}

void displayLine_(Mat image, Vec4d lin, double length){
    Vec4i displayLin;
    double wind = length;
    displayLin[0] = int(round(lin[0]*wind+lin[2]));
    displayLin[1] = int(round(lin[1]*wind+lin[3]));
    displayLin[2] = int(round(lin[2]));
    displayLin[3] = int(round(lin[3]));
    line(image,Point(displayLin[2],displayLin[3]),Point(displayLin[0],displayLin[1]),Scalar(0,255,200),2);
}

double parallelity(Vec4d lin1, Vec4d lin2){
    Point2d l1(lin1[0],lin1[1]);
    Point2d l2(lin2[0],lin2[1]);
    //double crossP = (l1.x)*(l2.y)-(l1.y)*(l2.y);
    double dotP = (l1.x)*(l2.x)+(l1.y)*(l2.y);
    double cos_alpha = abs(dotP/((pointLength(l1))*(pointLength(l2))));
    // double sin_alpha = abs(crossP/(pointLength(l1)*pointLength(l2)));
    return 1/cos_alpha;

}

double dist2Line(Vec4d lin, Point point){
    double x_0 = lin[2];
    double y_0 = lin[3];
    double x_p = point.x-x_0;
    double y_p = point.y-y_0;
    auto v_x =  double(lin[0]);
    auto v_y =  double(lin[1]);
    double dist2Lin =abs(x_p*v_y-y_p*v_x);
    return dist2Lin;

}

int closest2Line(const vector<PuzzlePoint> & points, Vec4d lin){
    vector<double> dist2Lins;
    double minDiff = 10000.0;
    int minIndex = -1;
    for(int a = 0; a<points.size();a++){
        Point point = points[a].its_point;
        double distance = dist2Line(lin,point);
        cout<<"distance: "<<distance<<endl;
        dist2Lins.push_back(distance);
        if(distance<minDiff && point!=Point(lin[2], lin[3])){
            minIndex = a;
            minDiff = distance;
        }
    }
    return minIndex;

}

int findNeighbour(const vector<PuzzlePoint> & points, const PuzzlePoint & puzzP, bool left){
    double minDiff = 10000.0;
    int minIndex = -1;
    vector<double> distances;
    if(left){
        for(int a = 0; a<points.size();a++) {
            Point point = points[a].its_point;
            double distance = 0;
            distance = dist2Line(puzzP.its_linL, point);
            double paral = parallelity(puzzP.its_linL, points[a].its_linR);
            paral += parallelity(puzzP.its_linR, points[a].its_linL);
            distances.push_back(distance);

            if (distance < minDiff && point != Point(int(round(puzzP.its_linR[2])), int(round(puzzP.its_linR[3])))) {
                minIndex = a;
                minDiff = distance;
            }
        }//end for
        std::sort(distances.begin(),distances.end());
        int firstCandidate =  minIndex;
        int secondCandidate = 0;
        for(int i =0; i< points.size(); i++){
            double d = dist2Line(puzzP.its_linL, points[i].its_point);
            if(d == distances[2]){
                if(distances[2]>(2*distances[1]+9)){ // if the second candidate would be too far away
                    secondCandidate = firstCandidate;
                }
                else
                    secondCandidate = i;
            }
        }
        double paral1 = parallelity(puzzP.its_linL, points[firstCandidate].its_linR);
        paral1+=parallelity(puzzP.its_linR, points[firstCandidate].its_linL);
        double paral2 = parallelity(puzzP.its_linL, points[secondCandidate].its_linR);
        paral2+=parallelity(puzzP.its_linR, points[secondCandidate].its_linL);
        if(paral1<paral2)
            return (firstCandidate);
        else
            //return(secondCandidate);
            return (secondCandidate);


    }
    else{
        for(int a = 0; a<points.size();a++) {
            Point point = points[a].its_point;
            double distance = 0;
            distance = dist2Line(puzzP.its_linR, point);
            double paral = parallelity(puzzP.its_linR, points[a].its_linL);
            paral += parallelity(puzzP.its_linL, points[a].its_linR);
            distances.push_back(distance);

            if (distance < minDiff && point != Point(int(round(puzzP.its_linL[2])), int(round(puzzP.its_linL[3])))) {
                minIndex = a;
                minDiff = distance;
            }
        }//end for
        std::sort(distances.begin(),distances.end());
        int firstCandidate =  minIndex;
        int secondCandidate = 0;
        for(int i =0; i< points.size(); i++){
            double d = dist2Line(puzzP.its_linR, points[i].its_point);
            if(d == distances[2]){
                secondCandidate = i;
                if(distances[2]>(2*(distances[1])+9)){ // if the second candidate would be too far away
                    secondCandidate = firstCandidate;
                }
            }
        }
        double paral1 = parallelity(puzzP.its_linR, points[firstCandidate].its_linL)
                        + parallelity(puzzP.its_linL,points[firstCandidate].its_linR);
        double paral2 = parallelity(puzzP.its_linR, points[secondCandidate].its_linL)
                        + parallelity(puzzP.its_linL, points[secondCandidate].its_linR);
        if(paral1<paral2)
            return (firstCandidate);
        else
            return (secondCandidate);

    }
    return minIndex;

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
    //windowSize must be odd!
    double runningTotal = 0.0;
    int inputS = input.size();
    vector <double> movingAverage;
    for(size_t i = 0; i < input.size(); i++)
    {
        runningTotal = 0;   // add
        if(i >= windowSize && i+windowSize<input.size())
            for(int j = i-windowSize; j<=i+windowSize;j++){
                runningTotal+=input[j];
            }

//        if(i >= (windowSize - 1)) {// output moving average
//           // cout << "Your SMA: " << runningTotal / (double) windowSize;
//
//        }
        movingAverage.push_back((runningTotal /( (double)( 2*windowSize+1))));
    }
    return movingAverage;
}

RotatedRect getBaseRect(const Mat& src){
    Mat gray = src.clone();
    Contour_t contours;
    Hierarchy_t hierarchy;
    double mSize = 0;
    findContours(gray,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    mSize = contours.size()/4;

    auto morph_size =int(round(mSize));
    cout<<"msize: "<<morph_size<<endl;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(gray,gray,MORPH_CLOSE,element);
    morphologyEx(gray,gray,MORPH_CLOSE,element);
    morph_size = int(round(mSize)*1);

    element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ),Point(morph_size, morph_size ));
    morphologyEx(gray,gray,MORPH_OPEN,element);
    morphologyEx(gray,gray,MORPH_OPEN,element);
    morph_size = int(round(mSize/3));
    element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ),Point(morph_size, morph_size ));
    morphologyEx(gray,gray,MORPH_ERODE,element);
    //imshow("gray",gray);
    gray  = drawLargestContour(gray,10,true);
    //imshow("polyapprox",gray);
    Mat canny;
    Canny(gray,canny,100,200,3);
    //imshow("Canny",canny);
    findContours(canny,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    int index = getLongestContourIndex(contours);
    RotatedRect rectangle = minAreaRect(contours[index]);
    return rectangle;

}

double lineFitNess(Vec4d lin, vector<Point> points, Point cPoint){
    vector<double> dist2Lins;
    double sumD = 0.0;

    for(int a = 0; a<points.size();a++){

        double x_0 = double( cPoint.x);
        double y_0 = double( cPoint.y);
        x_0 = lin[2];
        y_0 = lin[3];
        double x_p = points[a].x-x_0;
        double y_p = points[a].y-y_0;
        double v_x =  double(lin[0]);
        double v_y =  double(lin[1]);

        // cout<<"x0: "<<x_0<<" y0: "<< y_0<<"x_p: "<< x_p << " y_p" << y_p<<" vx: "<<v_x<<" vy: "<<v_y<<endl;
        double dist2Lin = (x_p-x_0)*(y_0+v_y)-(y_p-y_0)*(x_0+v_x);
        dist2Lin = x_p*v_y-y_p*v_x;
        // dist2Lin = sqrt(pow(  x_p*(1-pow(v_x,2) -v_x*v_y*y_p) , 2 ) + pow(  y_p*(1-pow(v_y,2))-v_x*v_y*x_p  ,2));
        dist2Lins.push_back(dist2Lin);
        sumD += abs(dist2Lin);

    }
    double lineFitness = sumD / (double)points.size();
    return lineFitness;
}

double pointLength(Point2d point){
    double length = sqrt((point.x)*(point.x)+(point.y)*(point.y));
    return length;
}

Mat drawRect(const RotatedRect& rectangle,  Mat src){
    //Mat image = Mat::zeros(src.size(), CV_8UC3);
    Point2f vtx[4];
    rectangle.points(vtx);
    for(int j = 0; j < 4; j++ ) {
        line(src, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
    }
    line(src,vtx[0], vtx[2],Scalar(0, 255, 0), 1, LINE_AA); // diagonal
    line(src,vtx[1], vtx[3],Scalar(0, 255, 0), 1, LINE_AA); // diagonal
    return src;
}

void PuzzlePoint::findCornerity(int noPoints) {
    // cout<<"its_point: "<<its_point<<endl;
    int cIndex = -1;
    float minDiff = 1000.0;
    // Find the index of the contour points
    for(int k = 0; k<its_contour.size();k++){

        // cout<<contours[index][k]<<endl;
        Point2f contour2Edge = Point2f(its_contour[k].x-float(its_point.x), its_contour[k].y-float(its_point.y));
        float distDiff =  sqrt((contour2Edge.x)*(contour2Edge.x)+(contour2Edge.y)*(contour2Edge.y));
        if(distDiff<minDiff){
            minDiff = distDiff;
            cIndex = k;
        }
    }

    vector<Point> neighbours;
    vector<Point> rightNeighbours;
    vector<Point> leftNeighbours;
    Vec4d lin;
    Vec4d linR, linL;
    double wSize = noPoints;
    //<editor-fold desc="Find neighbouring points">
    auto wind = int(round(wSize)); // 30
    // for circularization the contours array:
    for(int u = -wind; u<wind; u++){
        int origInd = cIndex+u;
        int contInd = 0;
        int det = its_contour.size()-1;
        if(origInd > det){ // go around
            contInd = origInd-its_contour.size();
        }
        else if(origInd<0){
            contInd = its_contour.size()+origInd;
        }
        else{
            contInd = origInd;
        }
        if(u<0){
            leftNeighbours.push_back(its_contour[contInd]);
        }
        else{
            rightNeighbours.push_back(its_contour[contInd]);
        }
        neighbours.push_back(its_contour[contInd]);
    }
    //</editor-fold>

    //TODO: try min area enclosing circle as well
    // or BB concept: hough circles for raw image
    fitLine(neighbours,its_lin,CV_DIST_L2,0,0.01,0.01);
    fitLine(rightNeighbours,its_linR,CV_DIST_L2,0,0.01,0.01);
    fitLine(leftNeighbours,its_linL,CV_DIST_L2,0,0.01,0.01);

    its_linR[2] = its_point.x;
    its_linR[3] = its_point.y;
    its_linL[2] = its_point.x;
    its_linL[3] = its_point.y;

    double linefitness = lineFitNess(its_linR,rightNeighbours, Point2d(its_linR[2],its_linR[3]))+
                         lineFitNess(its_linL,leftNeighbours, Point2d(its_linL[2],its_linL[3]));
    Point2d PointR = Point2d(its_linR[2]-its_point.x,its_linR[3]-its_point.y);
    Point2d PointL = Point2d(its_linL[2]-its_point.x,its_linL[3]-its_point.y);
    PointR = Point2d(its_linR[0],its_linR[1]);
    PointL = Point2d(its_linL[0],its_linL[1]);

    double cos_alpha = (PointR.x)*(PointL.x)+(PointR.y)*(PointL.y)/(pointLength(PointL))*(pointLength(PointR));
    if(cos_alpha==0)
        cos_alpha = 0.001;
    double flatness = abs(cos_alpha);
    double evenness = linefitness*its_linFactor; // how good can we fit a line on the two sides
    //cout<<"flatness: "<<flatness<<endl;
    //cout<<"evenness: "<<evenness<<endl;
    its_cornerity =  (flatness+evenness);


}

// Finds the intersection of two lines, or returns false.
// The lines are defined by (o1, p1) and (o2, p2).
bool intersection__old(Vec4d l1, Vec4d l2, Point2d & r) {
    Point2d o1(l1[2],l1[3]);
    Point2d p1(l1[0],l1[1]);
    Point2d o2(l2[2],l2[3]);
    Point2d p2(l2[0],l2[1]);


    Point2d x = o2 - o1;
    Point2d d1 = p1 - o1;
    Point2d d2 = p2 - o2;

    double cross = (d1.x)*(d2.y) - (d1.y)*(d2.x);
    cout<< "cross: "<<cross<<endl;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    cout<<"intersectP: "<< r <<endl;
    return true;
}

bool intersection(Vec4d l1, Vec4d l2, Point2d & r) {
    double a, c;
    a = (l1[1])/(l1[0]);
    c = l1[3]-l1[2]*a;
    double b, d;
    b = (l2[1])/(l2[0]);
    d = l2[3]-l2[2]*b;

    r = Point2d((d-c)/(a-b),(a*d-b*c)/(a-b));
    return true;
}

int closestPoint(vector<PuzzlePoint> points, Point p){
    double minDist = 10000;
    int minIndex = 0;
    for(int i = 0; i< points.size(); i++){
        Point2d segment = p-points[i].its_point;
        double distance = pointLength(segment);
        if(distance<minDist){
            minDist = distance;
            minIndex = i;
        }
    }
    return minIndex;
}//
// Created by Tamas on 2017-12-28.
//

