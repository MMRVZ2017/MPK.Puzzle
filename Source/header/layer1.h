//
// Created by Tamas on 2018-01-05.
//

#ifndef MPK_PUZZLE_LAYER1_H_H
#define MPK_PUZZLE_LAYER1_H_H
#include "../header/preproc_base.h"




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

vector < PuzzlePoint > findCorners(const Mat & src);
double parallelity(Vec4d lin1, Vec4d lin2);
int closestPoint(vector<PuzzlePoint> points, Point p);
bool intersection(Vec4d l1, Vec4d l2, Point2d & r);
int findNeighbour(const vector<PuzzlePoint> & points, const PuzzlePoint & point, bool left);
int closest2Line(const vector<PuzzlePoint> & points, Vec4d lin);
double pointLength(Point2d point);
vector<double> differentiate(vector<double> input, int windowSize);
vector<double> movingAverage(vector<double> input, int windowSize);
double avg (vector<double> input);
RotatedRect getBaseRect(const Mat &); // obsolete
Mat drawRect(const RotatedRect& rectangle, Mat src);
double lineFitNess(Vec4d lin, vector<Point> points, Point cPoint);
void displayLine_(Mat image, Vec4d lin, double length);





#endif //MPK_PUZZLE_LAYER1_H_H
