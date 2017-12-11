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
double pointLength(Point2d point);
RotatedRect getBaseRect(const Mat &);
Mat drawRect(const RotatedRect& rectangle, Mat src);
double lineFitNess(Vec4d lin, vector<Point> points, Point cPoint);
int main() {
    for(int i = 0;i<1008;i++){
        cout<<"i: "<<i<<endl;
        Mat image = readImage(i, "../images/output/blackAndWhite/output_VZ/");
        imshow("image",image);
        Mat segmentCheck = segment(image);
        int nonZeros =  countNonZero(segmentCheck);
        cout<<nonZeros<<endl;
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
        imshow("Canny",canny);
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
        // for moving average:
        //TODO make windowSize adaptive:
        int windowSize = 7;

        vector<double>mAvg = movingAverage(infiniteAlpha, windowSize);
        cout<<"mAvg.size: "<<mAvg.size()<<endl;
        vector<double> diff_l = differentiate(mAvg,3);
        vector<double> diff2_l = differentiate(diff_l,3);
        cout<<"diff_l.size: "<<diff_l.size()<<endl;
        vector<Point> convexPoints;
        vector<Point> concavePoints;
        vector<Point> inPoints;
        vector<Point> outPoints;
        vector<Point> cornerPoints;
        double maxSlope = findmax(diff_l);
        double avgSlope = avg(diff_l);
        double avgDist = avg(mAvg);
        cout<<endl;
        //<editor-fold desc="Point sorting ( convex or concave">
        for(int j = startIndex; j < diff_l.size()-startIndex; j++){
            int ind = j - startIndex; // maybe shouldn't: -windowSize
           // cout<<"ind: "<<ind<<endl;
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
                    cout<<"edge_alpha(ind): "<<edge_alpha[ind]<<endl;
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
                j+=middleIndex;


            }// end of nullstelles
        }// End of points sorting
        //</editor-fold>

        // line fitting
        vector<double> lineFits;
        vector<int> indexes;
        for(int o = 0; o<convexPoints.size(); o++){ // Examining each convex Point
            Point cPoint = convexPoints[o];
           // cout<<"cPoint: "<<cPoint<<endl;
            int cIndex = -1;
            float minDiff = 1000.0;
            for(int k = 0; k<contours[index].size();k++){

                // cout<<contours[index][k]<<endl;
                Point2f contour2Edge = Point2f(contours[index][k].x-float(cPoint.x), contours[index][k].y-float(cPoint.y));
                float distDiff =  sqrt((contour2Edge.x)*(contour2Edge.x)+(contour2Edge.y)*(contour2Edge.y));
                //cout<<"distDiff: "<<distDiff<<endl;
                // cout<<"min_diff: "<<min_diff<<endl;
                if(distDiff<minDiff){
                    minDiff = distDiff;
                    cIndex = k;
                }

                indexes.push_back(cIndex);

            }
            //cout<<"cindex: "<< cIndex<<endl;
            vector<Point> neighbours;
            vector<Point> rightNeighbours;
            vector<Point> leftNeighbours;
            Vec4d lin;
            Vec4d linR, linL;
            double wSize = contours[index].size()/35;
            //<editor-fold desc="Find neighbouring points">
            auto wind = int(round(wSize)); // 30
            for(int u = -wind; u<wind; u++){
                int origInd = cIndex+u;
                int contInd = 0;
               // cout<<"origInd: "<<origInd<<endl;
                int det = contours[index].size()-1;
                if(origInd > det){ // go around
                    contInd = origInd-contours[index].size();
                }
                else if(origInd<0){
                    contInd = contours[index].size()+origInd;
                }
                else{
                    contInd = origInd;
                }
                if(u<0){
                    leftNeighbours.push_back(contours[index][contInd]);
                }
                else{
                    rightNeighbours.push_back(contours[index][contInd]);
                }
                neighbours.push_back(contours[index][contInd]);
            }
            //</editor-fold>
           // cout<<"neighbours: "<<neighbours<<endl;


            fitLine(neighbours,lin,CV_DIST_L2,0,0.01,0.01);
            fitLine(rightNeighbours,linR,CV_DIST_L2,0,0.01,0.01);
            fitLine(leftNeighbours,linL,CV_DIST_L2,0,0.01,0.01);

            Point2d PointR = Point2d(linR[2]-cPoint.x,linR[3]-cPoint.y);
            Point2d PointL = Point2d(linL[2]-cPoint.x,linL[3]-cPoint.y);

            double cos_alpha = (PointR.x)*(PointL.x)+(PointR.y)*(PointL.y)/(pointLength(PointL))*(pointLength(PointR));
            if(cos_alpha==0)
                cos_alpha = 0.001;
           // cout<<lin<<endl;
            //<editor-fold desc="display the line">
            Vec4i displayLine;
            displayLine[0] = int(round(lin[0]*wind+cPoint.x));
            displayLine[1] = int(round(lin[1]*wind+cPoint.y));
            displayLine[2] = int(round(cPoint.x)-lin[0]*wind);
            displayLine[3] = int(round(cPoint.y)-lin[1]*wind);
            line(image,Point(displayLine[2],displayLine[3]),Point(displayLine[0],displayLine[1]),Scalar(0,255,200),2);
            vector<double> dist2Lins;
            //</editor-fold>

            double lineFitness = lineFitNess(lin,neighbours,cPoint);
            lineFitness = lineFitNess(linL,leftNeighbours,cPoint)+lineFitNess(linR,rightNeighbours,cPoint);
            lineFitness = 1/cos_alpha;
            //double lineFitness = findmax(dist2Lins);
            lineFits.push_back(lineFitness);
            // examine contour region and categorize

        }// End of line fitting to convex points
        vector<Point> sortedConvex;
        if(convexPoints.size()>4){
            while(!lineFits.empty()){
                // float minLength =  findmin(lengths); // ascending order
                double maxLength =  findmin(lineFits); // descending order
                for(size_t a = 0;a<convexPoints.size(); a++){
                    if(lineFits[a]==maxLength){
                        lineFits.erase(lineFits.begin()+ a);
                        sortedConvex.push_back(convexPoints[a]);
                        convexPoints.erase(convexPoints.begin() + a);
                    }
                }
            }
            for(int j = 4; j<sortedConvex.size();j++){
                char str[200];
                sprintf(str," dist %d",j);
                cout<<str<<endl;
                putText(image, str, sortedConvex[j], FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255));
                circle(image,sortedConvex[j],20,Scalar(0,0,255),2);
            }

        }// end if (more than 4 convex points)


        imshow("possible corners", image);
        char outputFile [100];
        sprintf(outputFile,"../images/output/Abstract1/%d.jpg",i);
        imwrite(outputFile,image);






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
    imshow("Canny",canny);
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
        double x_p = double(points[a].x-x_0);
        double y_p = double( points[a].y-y_0);
        double v_x =  double(lin[0]);
        double v_y =  double(lin[1]);

        // cout<<"x0: "<<x_0<<" y0: "<< y_0<<"x_p: "<< x_p << " y_p" << y_p<<" vx: "<<v_x<<" vy: "<<v_y<<endl;
        double dist2Lin = -(x_p-x_0)*(y_0+v_y)+(y_p-y_0)*(x_0+v_x);
        dist2Lin = sqrt(pow(  x_p*(1-pow(v_x,2) -v_x*v_y*y_p) , 2 ) + pow(  y_p*(1-pow(v_y,2))-v_x*v_y*x_p  ,2));
        //  cout<<"dist2Lin: "<<dist2Lin<<endl;// értelmetlen
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