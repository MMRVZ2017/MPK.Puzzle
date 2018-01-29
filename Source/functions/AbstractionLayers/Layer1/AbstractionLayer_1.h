#ifndef SOURCE_ABSTRACTIONLAYER_1_H
#define SOURCE_ABSTRACTIONLAYER_1_H

#include "AbstractionLayer_1_Properties.h"
#include "../AbstraktionLayer_Base.h"

#include <utility>
#include <vector>
#include <iostream>
#include <bitset>
#include <random>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <bitset>

#define DISPLAY false

#ifdef _WIN32
#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif

#define IMG_SIZE 400 // 400
#define TOP 6
#define RIGHT 4
#define BOTTOM 2
#define LEFT 0

using namespace std;
using namespace cv;

class AbstractionLayer_1 : public AbstractionLayer_Base<AbstractionLayer_1_Properties>
{
public:
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) final ;
    bool EvaluateQuality ( coor constraintCoordinate, qualityVector& qVector)final;
    bool SetConstraintOnPosition( coor constraintCoordinate, AbstractionLayer_1_Properties constraint)final;
    bool RemoveConstraintOnPosition( coor constraintCoordinate)final;
    int RemoveSimilar(qualityVector&,uint8_t&);

    bool PlaceOfPartGood(coor myCoor, uint8_t& myPart);
    void shift(uint8_t& Part, int shifts);
    void setEdgeZero();

    int PoempelSum(uint8_t constraint);

    void CreateRandomPuzzle();
    qualityVector returnInBox(vector<Part>& PuzzleBox);
    void printConstraintMatrix();

private:
};

class Details{
public:
    vector<vector<Point>> getContour(){return contour;}
    Mat getImage(){return image;}
    Point getCenter(){return center;}
    vector<Vec4i> getHierarchy(){return hierarchy;}
    unsigned char getTabs(){return tabs;}

    void setContour(vector<vector<Point>> cont){contour = std::move(cont);}
    void setImage(Mat im){image = std::move(im);}
    void setCenter(const Point &c){center = c;}
    void setHierarchy(vector<Vec4i> hier){hierarchy = std::move(hier);}
    void setCorners(vector<Point> cor){corners = std::move(cor);}
    void setTabs(unsigned char t){tabs = t;}
    vector<double> getLen(){return len;}
    void setLens(vector<double> l ){len = l;}
    Point getMidpoint(){return midpoint;}
    void setMidpoint(Point m ){midpoint = m;}
    vector<double> getPoempelPosition(){return PoempelPosition;}
    void setPoempelPosition(vector<double>& newPP){PoempelPosition=newPP;}
    vector<Point> getCorners(){return corners;}

private:
    Mat image;
    vector<double> PoempelPosition;
    vector<Point> corners;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy;
    Point center;
    vector<double> len;
    unsigned char tabs;
    Point midpoint;
};

class analyseParts{
public:
    explicit analyseParts(int s = 1008): nr_parts(s){}
    Mat getImage(int i){if(i>= nr_parts)return masks[nr_parts-1].getImage(); else return masks[i].getImage();}
    vector<vector<Point>> getContour(int i){if(i>= nr_parts)return masks[nr_parts-1].getContour(); else return masks[i].getContour();}
    Point getCenter(int i){if(i>= nr_parts)return masks[nr_parts-1].getCenter(); else return masks[i].getCenter();}
    vector<Vec4i> getHierarchy(int i){if(i>= nr_parts)return masks[nr_parts-1].getHierarchy(); else return masks[i].getHierarchy();}
    unsigned char getTabs(int i){if(i>= nr_parts)return masks[nr_parts-1].getTabs(); else return masks[i].getTabs();}
    vector<double> getLen(int i ){return masks[i].getLen();}
    vector<double> getPoempelPosition(int i){return masks[i].getPoempelPosition();}
    vector<double> analyseLens(vector<double>, vector<Point>);
    Point calcMidpoint(vector<Point>);
    Point getMidpoint(int i){return masks[i].getMidpoint();}
    Point findCenter(Mat);
    vector<Point> findCorners(vector<Point>,Point);
    unsigned char analyseContour(vector<Point>, vector<Point>,vector<double>&);
    Mat makeBorder(Mat&);
    Mat readImages(int);
    Mat morphDilateErode(Mat&);
    vector<vector<Point>> findingContours(Mat&);
    Mat polyApprox(vector<vector<Point>> &);
    bool getImages();
private:
    float lengthTwoPoints(Point, Point);
    float angle(Point, Point, Point);
    vector<Details> masks;
    int nr_parts;
};


#endif //SOURCE_ABSTRACTIONLAYER_1_H
