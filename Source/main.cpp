#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <bitset>

#define DISPLAY true
#define PATH "C:\\Users\\michi\\Downloads\\puzzleteile_bb_2017\\jpg_96_klein\\%04d.jpg"
#define IMG_SIZE 270
#define TOP 6
#define RIGHT 4
#define BOTTOM 2
#define LEFT 0
using namespace std;
using namespace cv;

Mat source;
int counter_save = 0;

class Part
{
public:
    Part(): connections(0){}
    ~Part() {}
    uint8_t getConnections() const
    {
        return connections;
    }
    void setConnections(uint8_t newconnections)
    {
        connections = newconnections;
    }

private:
    uint8_t connections;
};

class Details{
public:
    vector<vector<Point>> getContour(){return contour;}
    Mat getImage(){return image;}
    Point getCenter(){return center;}
    vector<Vec4i> getHierarchy(){return hierarchy;}
    unsigned char getTabs(){return tabs;}

    void setContour(vector<vector<Point>> cont){contour = cont;}
    void setImage(Mat im){image = im;}
    void setCenter(Point c){center = c;}
    void setHierarchy(vector<Vec4i> hier){hierarchy = hier;}
    void setCorners(vector<Point> cor){corners = cor;}
    void setTabs(unsigned char t){tabs = t;}

    vector<Point> getCorners(){return corners;}

private:
    Mat image;
    vector<Point> corners;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy;
    Point center;
    unsigned char tabs;
};

class analyseParts{
public:
    analyseParts(int s = 1008): nr_parts(s){getImages();}
    Mat getImage(int i){if(i>= nr_parts)return masks[nr_parts-1].getImage(); else return masks[i].getImage();}
    vector<vector<Point>> getContour(int i){if(i>= nr_parts)return masks[nr_parts-1].getContour(); else return masks[i].getContour();}
    Point getCenter(int i){if(i>= nr_parts)return masks[nr_parts-1].getCenter(); else return masks[i].getCenter();}
    vector<Vec4i> getHierarchy(int i){if(i>= nr_parts)return masks[nr_parts-1].getHierarchy(); else return masks[i].getHierarchy();}
    unsigned char getTabs(int i){if(i>= nr_parts)return masks[nr_parts-1].getTabs(); else return masks[i].getTabs();}
    Point findCenter(Mat);
    vector<Point> findCorners(vector<Point>,Point);
    unsigned char analyseContour(vector<Point>, vector<Point>);
    Mat makeBorder(Mat&);
    Mat readImages(int);
    Mat morphDilateErode(Mat&);
    vector<vector<Point>> findingContours(Mat&);
    Mat polyApprox(vector<vector<Point>> &);
private:
    void getImages();
    float lengthTwoPoints(Point, Point);
    float angle(Point, Point, Point);
    vector<Details> masks;
    int nr_parts;
};

Mat analyseParts::makeBorder(Mat& im_bw) {
    Mat im_part;

    Scalar value = (0,0,0);
    Point center = findCenter(im_bw);
    copyMakeBorder(im_bw, im_part, abs((IMG_SIZE/2)-center.y),abs((IMG_SIZE/2)-(im_bw.rows-center.y)), abs((IMG_SIZE/2)-center.x),abs( (IMG_SIZE/2)-(im_bw.cols-center.x )), BORDER_CONSTANT, value);
    copyMakeBorder(source, source, abs((IMG_SIZE/2)-center.y),abs((IMG_SIZE/2)-(source.rows-center.y)), abs((IMG_SIZE/2)-center.x),abs((IMG_SIZE/2)-(source.cols-center.x )), BORDER_CONSTANT, Scalar(255,255,255));

    return im_part;
}

Mat analyseParts::readImages(int count)
{
    char name[100];
    Mat corr;
    Mat ref_gray;

    sprintf(name, PATH, count);
    //cout << "path" << name << endl;

    source = imread(name, 1);
    if (!source.data)
        cerr << "Problem loading image!!!" << endl;
    if(DISPLAY)imshow("source",source);

    Mat im_gray, im_bw;
    cvtColor(source, im_gray, CV_RGB2GRAY);
    im_bw = (im_gray > 220);
    im_bw = 255 - im_bw;
    im_bw = makeBorder(im_bw);
    return im_bw;

}

Mat analyseParts::morphDilateErode(Mat &im_bw)
{
    Mat dst = im_bw.clone();
    int operation = 3;
    int morph_size = 2.4;
    int dilation_size = 2;
    int dilation_size1 = 1.5;
    int erosion_size = 2;
    int erosion_type = MORPH_RECT;
    int dilation_type = MORPH_RECT;
    Mat element = getStructuringElement(0, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

    Mat element1 = getStructuringElement(dilation_type, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
    Mat element3 = getStructuringElement(dilation_type, Size(2 * dilation_size1 + 1, 2 * dilation_size1 + 1), Point(dilation_size1, dilation_size1));
    Mat element2 = getStructuringElement(erosion_type, Size(2 * erosion_size, 2 * erosion_size), Point(erosion_size, erosion_size));

    morphologyEx(im_bw, dst, operation, element);
    dilate( dst, dst, element );
    erode( dst, dst, element2 );

    return dst;
}

vector<vector<Point>> analyseParts::findingContours(Mat& dst)
{
    vector<vector<Point> > contours1;
    vector<vector<Point> > contours;
    vector<Point> poly;
    vector<Vec4i> hierarchy1;
    vector<Vec4i> hierarchy;
    int dilation_size1 = 1.5;
    int erosion_size = 2;
    int erosion_type = MORPH_RECT;
    int dilation_type = MORPH_RECT;

    Mat element3 = getStructuringElement(dilation_type, Size(2 * dilation_size1 + 1, 2 * dilation_size1 + 1), Point(dilation_size1, dilation_size1));
    Mat element2 = getStructuringElement(erosion_type, Size(2 * erosion_size, 2 * erosion_size), Point(erosion_size, erosion_size));

    Mat dst1 =  Mat::zeros(dst .size(), CV_8UC1);
    findContours(dst, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0));
    for (int i = 0; i < contours1.size(); i++) {
        drawContours(dst1, contours1, i, Scalar(255, 255, 255), 2, 8, hierarchy1, 0);
    }
    dilate( dst1, dst1, element3 );
    erode( dst1, dst1, element2 );
    findContours(dst1, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    return contours;
}

Mat analyseParts::polyApprox(vector<vector<Point>>& contours)
{
    vector<vector<Point>> largestContour;
    largestContour.resize(contours.size());
    int erosion_size = 2;
    int erosion_type = MORPH_RECT;
    Mat element2 = getStructuringElement(erosion_type, Size(2 * erosion_size, 2 * erosion_size), Point(erosion_size, erosion_size));
    int index = 0;
    double area = 0.0;
    int strength = 5;
    vector<Vec4i> hierarchy;
    Mat mask_gray;


    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), largestContour[index], strength, true);
        index++;
    }

    Mat mask = Mat::zeros(Point(IMG_SIZE,IMG_SIZE), CV_8UC3);
    for (int i = 0; i < largestContour.size(); i++) {
        area = contourArea(largestContour[i]);
        if(area > 500  )
            drawContours(mask, largestContour, i, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0);
    }
    erode( mask, mask, element2 );

    cvtColor(mask, mask_gray, CV_RGB2GRAY);
    mask_gray = (mask_gray > 200);

    return mask_gray;
}

Mat createEmpty(Point size, int color) {
    if (color) {
        Mat empty = Mat::zeros(size, CV_8UC3);
        return empty;
    } else {
        Mat empty = Mat::zeros(size, CV_8UC1);
        return empty;
    }
}

float analyseParts::lengthTwoPoints (Point one, Point two) {
    float length = 0;
    length = sqrt((one.x - two.x) * (one.x - two.x) + (one.y - two.y)*(one.y - two.y));
    return length;
}

float analyseParts::angle(Point one, Point two, Point three) {
    float angle = 0;
    float disa, disb, disc;
    int det = 0;
    Point vecA, vecB;
    vecA.x = one.x-two.x;
    vecA.y = one.y-two.y;

    vecB.x = three.x-two.x;
    vecB.y = three.y-two.y;

    det = vecA.x*vecB.y-vecA.y*vecB.x;
   // cout << "vecA: " << vecA << endl;
    //cout << "vecB: " << vecB << endl;
    //cout << "det: " << det << endl;

    disa = lengthTwoPoints(two, three);
    disb = lengthTwoPoints(one, three);
    disc = lengthTwoPoints(one, two);

    angle = acos( ((disa*disa) + (disc *disc) - (disb * disb)) / (2 * disa * disc) );
    angle = angle * 180 / CV_PI;
    return angle;
}

void analyseParts::getImages(){
    Details mask;
    Mat src;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> corners;

    vector<Mat> puzzleimages;
    vector<vector<Point> > contours1;
    Mat mask_gray;

    for (int i = 0; i < nr_parts; i++) {
        if(DISPLAY)
            cout << "Bild " << i << endl;
        Mat im_bw = readImages(i);
        Mat dst = morphDilateErode(im_bw);
        contours1 = findingContours(dst);
        mask_gray = polyApprox(contours1);
        mask.setImage(mask_gray);

        int maxCorners = 20;
        vector<Point> corners_find;
        double qualityLevel = 0.01;
        double minDistance = 10;
        int blockSize = 3;
        bool useHarrisDetector = false;
        double k = 0.04;
        RNG rng(12345);

        Mat copy;
        copy = mask_gray.clone();

      /*  goodFeaturesToTrack( mask_gray,
                             corners_find,
                             maxCorners,
                             qualityLevel,
                             minDistance,
                             Mat(),
                             blockSize,
                             useHarrisDetector,
                             k );

        //cout<<"** Number of corners detected: "<<corners_find.size()<<endl;

        int r = 4;
        for( int i = 0; i < corners_find.size(); i++ )
        { circle( copy, corners_find[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),
                                                   rng.uniform(0,255)), -1, 8, 0 ); }


        if(DISPLAY) imshow( "Ecken gefunden", copy );*/


        mask.setCenter(findCenter(mask_gray));
        findContours(mask_gray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        mask.setContour(contours);
        mask.setHierarchy(hierarchy);
        corners = findCorners(contours[0],mask.getCenter());
        mask.setCorners(corners);
        mask.setTabs(analyseContour(corners,contours[0]));
        masks.push_back(mask);
        destroyAllWindows();
    }
}

Point analyseParts::findCenter(Mat img){
    Mat im  = img.clone();
    Canny(im, im, 800, 1000, 3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( im, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Moments m = moments(im, true);
    Point center(m.m10/m.m00, m.m01/m.m00);
    return center;
}

float pitch2Points(Point one, Point two)
{
    float pitch=0;
    float deltay=0;
    float deltax = 0;
    //cout << "**********\n" << "punkt1: " << one.x << "," << one.y << endl << "punkt2:" << two.x << "," << two.y << endl << "*********" << endl;
    deltay = abs(one.y - two.y);
    deltax = abs(one.x - two.x);

    if(deltax == 0)
        deltax = 0.1;

    if(deltay == 0)
        deltay = 0.1;

    pitch = deltay/deltax;

  //  cout << "deltax = " << deltax << endl;
  //  cout << "deltay = " << deltay << endl;
    return pitch;
}

vector<Point> analyseParts::findCorners(vector<Point> contour, Point center){


    int minContourPoint = 5;
    vector<vector<Point>> quad_contour;
    vector<Point> buf0;
    vector<Point> buf1;
    vector<Point> buf2;
    vector<Point> buf3;

    for(int i = 0; i < contour.size();i++){
        if(contour[i].x > center.x && contour[i].y > center.y) {
            buf0.push_back(contour[i]);
        } else if(contour[i].x < center.x && contour[i].y > center.y) {
            buf1.push_back(contour[i]);
        } else if(contour[i].x > center.x && contour[i].y < center.y) {
           buf2.push_back(contour[i]);
        } else if(contour[i].x < center.x && contour[i].y < center.y) {
            buf3.push_back(contour[i]);
        }
    }
    quad_contour.push_back(buf0);
    quad_contour.push_back(buf1);
    quad_contour.push_back(buf2);
    quad_contour.push_back(buf3);

    Mat drawing = createEmpty(Point(IMG_SIZE,IMG_SIZE),1);
    for(int j = 0; j < 4;j++){
        for(int i = 0; i < quad_contour[j].size(); i++) {
           circle(drawing,quad_contour[j][i],2,Scalar(abs(j+2)*50,abs(j+2)*20,0),3,8);
        }
    }
    circle(drawing,center,2,Scalar(0,255,255),3,8,0);
    /*finde ecke rechts unten*/
    vector<Point> corners;
    float dist = 0;
    float max_dist = 0;
    int max_idx = 0;
    int num = 0;
    float distToCenter = 0;
    float min_dist_line = 1000;
    float dist_buf = 0;
    for(int i = 0; i < (IMG_SIZE/2); i++){
        for(int j = 0; j < quad_contour[0].size(); j++){
            if(quad_contour[0][j].x > center.x+i  && quad_contour[0][j].y > center.y+i)
                num++;
        }
        if(num < minContourPoint) {
            dist = i;
            i = (IMG_SIZE/2);
        }
        num = 0;
    }
    for(int j = 0; j < quad_contour[0].size(); j++){
        if(quad_contour[0][j].x > center.x+dist  && quad_contour[0][j].y > center.y+dist) {
            distToCenter = lengthTwoPoints(center,quad_contour[0][j]);

            if(abs(quad_contour[0][j].x-center.x+dist) < min_dist_line){
                min_dist_line = j;
               // cout << "min dist line: " << min_dist_line << endl;
            }

            if(distToCenter > max_dist) {
                    max_dist = distToCenter;
                    max_idx = j;
            }
        }
    }

    corners.push_back(quad_contour[0][max_idx]);
    line(drawing,Point(center.x+dist,(IMG_SIZE/2)),Point(center.x+dist,IMG_SIZE),Scalar(255,0,255),3,8);
    line(drawing,Point((IMG_SIZE/2),center.y+dist),Point(IMG_SIZE,center.y+dist),Scalar(255,0,255),3,8);
    circle(drawing,quad_contour[0][max_idx],5,Scalar(50,100,255),5,8);

    circle(source,quad_contour[0][max_idx],8,Scalar(50,100,255),2,8);

    // Hier Code um bei Ecke rechts unten in beide Richtungen weiter zu gehen und 90° zu finden. Das ist echte Ecke


   /* float k = 0;
    float k1 = 0;
    circle(drawing,quad_contour[0][max_idx-7],8,Scalar(255,255,255),1,8);
   /* for(int i = max_idx; i > 2; i--){
        k = pitch2Points(quad_contour[0][i],quad_contour[0][i-2]);
        k1= pitch2Points(quad_contour[0][i-2],quad_contour[0][i-4]);
        cout << "delta zurueck: " << abs(k1-k) << "i: " << i <<   endl;
        if(abs(k1-k) >= 15){
            circle(drawing,quad_contour[0][i-2],8,Scalar(255,255,0),1,8);
            max_idx = i;
            i = 0;
        }
    }*/

   /* for (int i = max_idx-7;  i < quad_contour[0].size()-2; i++)
    {
        /*cout << "punkt1: " << quad_contour[0][max_idx].x << "," << quad_contour[0][max_idx].y<< endl;
        cout << "punkt2: " << quad_contour[0][max_idx+i].x << "," << quad_contour[0][max_idx+i].y<< endl;
        cout << "punkt3: " << quad_contour[0][max_idx+i*2].x << "," << quad_contour[0][max_idx+i*2].y<< endl;*/
       /* k = pitch2Points(quad_contour[0][i],quad_contour[0][i+1]);
        k1= pitch2Points(quad_contour[0][i+1],quad_contour[0][i+2]);
        cout << "delta: " << k-k1 << endl;
        if(k-k1 < -2 && quad_contour[0][i+1].y > quad_contour[0][i+2].y){
            circle(drawing,quad_contour[0][i+1],10,Scalar(0,255,255),1,8);
            i = quad_contour[0].size();
        }


        //contour von ecke nach links und rechts
        //cout << angle(contour[max_idx], contour[max_idx + 1], contour[max_idx + 2]);

    }

/*finde ecke links unten*/
    dist = 0;
    max_dist = 0;
    max_idx = 0;
    num = 0;
    distToCenter = 0;
    for(int i = 0; i < (IMG_SIZE/2); i++){
        for(int j = 0; j < quad_contour[1].size(); j++){
            if(quad_contour[1][j].x < center.x-i && quad_contour[1][j].y > center.y+i)
                num++;
        }
        if(num < minContourPoint) {
            dist = i;
            i = (IMG_SIZE/2);
        }
        num = 0;
    }
    for(int j = 0; j < quad_contour[1].size(); j++){
        if(quad_contour[1][j].x < center.x-dist  && quad_contour[1][j].y > center.y+dist) {
            distToCenter = lengthTwoPoints(center,quad_contour[1][j]);
            if(distToCenter > max_dist) {
                max_dist = distToCenter;
                max_idx = j;
            }
        }
    }
    corners.push_back(quad_contour[1][max_idx]);
    line(drawing,Point(center.x-dist,(IMG_SIZE/2)),Point(center.x-dist,IMG_SIZE),Scalar(255,0,255),3,8);
    line(drawing,Point((IMG_SIZE/2),center.y+dist),Point(0,center.y+dist),Scalar(255,0,255),3,8);
    circle(drawing,quad_contour[1][max_idx],5,Scalar(50,100,255),5,8);
    circle(source,quad_contour[1][max_idx],8,Scalar(50,100,255),2,8);

/*finde ecke rechts oben*/
    dist = 0;
    max_dist = 0;
    max_idx = 0;
    num = 0;
    distToCenter = 0;
    for(int i = 0; i < (IMG_SIZE/2); i++){
        for(int j = 0; j < quad_contour[2].size(); j++){
            if(quad_contour[2][j].x > center.x+i && quad_contour[2][j].y < center.y-i)
                num++;
        }
        if(num < minContourPoint) {
            dist = i;
            i = (IMG_SIZE/2);
        }
        num = 0;
    }
    for(int j = 0; j < quad_contour[2].size(); j++){
        if(quad_contour[2][j].x > center.x+dist  && quad_contour[2][j].y < center.y-dist) {
            distToCenter = lengthTwoPoints(center,quad_contour[2][j]);
            if(distToCenter > max_dist) {
                max_dist = distToCenter;
                max_idx = j;
            }
        }
    }
    corners.push_back(quad_contour[2][max_idx]);
    line(drawing,Point(center.x+dist,(IMG_SIZE/2)),Point(center.x+dist,0),Scalar(255,0,255),3,8);
    line(drawing,Point((IMG_SIZE/2),center.y-dist),Point(IMG_SIZE,center.y-dist),Scalar(255,0,255),3,8);
    circle(drawing,quad_contour[2][max_idx],5,Scalar(50,100,255),5,8);
    circle(source,quad_contour[2][max_idx],8,Scalar(50,100,255),2,8);

/*finde ecke links oben*/
    dist = 0;
    max_dist = 0;
    max_idx = 0;
    num = 0;
    distToCenter = 0;
    for(int i = 0; i < (IMG_SIZE/2); i++){
        for(int j = 0; j < quad_contour[3].size(); j++){
            if(quad_contour[3][j].x < center.x-i && quad_contour[3][j].y < center.y-i)
                num++;
        }
        if(num < minContourPoint) {
            dist = i;
            i = (IMG_SIZE/2);
        }
        num = 0;
    }
    for(int j = 0; j < quad_contour[3].size(); j++){
        if(quad_contour[3][j].x < center.x-dist  && quad_contour[3][j].y < center.y-dist) {
            distToCenter = lengthTwoPoints(center,quad_contour[3][j]);
            if(distToCenter > max_dist) {
                max_dist = distToCenter;
                max_idx = j;
            }
        }
    }
    corners.push_back(quad_contour[3][max_idx]);
    line(drawing,Point(center.x-dist,(IMG_SIZE/2)),Point(center.x-dist,0),Scalar(255,0,255),3,8);
    line(drawing,Point(0,center.y-dist),Point((IMG_SIZE/2),center.y-dist),Scalar(255,0,255),3,8);
    circle(drawing,quad_contour[3][max_idx],5,Scalar(50,100,255),5,8);
    circle(source,quad_contour[3][max_idx],8,Scalar(50,100,255),2,8);
    if(DISPLAY) imshow("draw",drawing);
    if(DISPLAY) imshow("Corners",source);
    //char save_name[100];
   // sprintf(save_name,"C:\\Users\\michi\\Desktop\\test_teile\\%d.jpg",counter_save);
   // imwrite(save_name,source);
  //  counter_save++;
    return corners;
}

unsigned char analyseParts::analyseContour(vector<Point> corners, vector<Point> contour) {
    vector<Point> contour_right;
    vector<Point> contour_top;
    vector<Point> contour_left;
    vector<Point> contour_bottom;

    Mat drawing = createEmpty(Point(IMG_SIZE,IMG_SIZE),1);
    int count = 0;
    int corner0 = 0, corner1 = 0, corner2 = 0, corner3 = 0;
    for(int i = 0; i < contour.size(); i++){
        //cout << "contour " << contour[i] << endl;
        if(contour[i] == corners[0])
            corner0 = i;
        else if(contour[i] == corners[1])
            corner1 = i;
        else if(contour[i] == corners[2])
            corner2 = i;
        else if(contour[i] == corners[3])
            corner3 = i;
    }
    //cout << "corner0: " << corners[0] << endl;
    //cout << "corner1: " << corners[1] << endl;
    //cout << "corner2: " << corners[2] << endl;
    //cout << "corner3: " << corners[3] << endl;


    count = corner0;
    while(contour[count] != contour[corner2]){
        count++;
        count %= contour.size();
        contour_right.push_back(contour[count]);
        circle(drawing,contour[count],3,Scalar(255,0,0),2,8);
    }
    count = corner2;
    while(contour[count] != contour[corner3]){
        count++;
        count %= contour.size();
        contour_top.push_back(contour[count]);
        circle(drawing,contour[count],3,Scalar(0,255,0),2,8);
    }
    count = corner3;
    while(contour[count] != contour[corner1]){
        count++;
        count %= contour.size();
        contour_left.push_back(contour[count]);
        circle(drawing,contour[count],3,Scalar(0,0,255),2,8);
    }
    count = corner1;
    while(contour[count] != contour[corner0]){
        count++;
        count %= contour.size();
        contour_bottom.push_back(contour[count]);
        circle(drawing,contour[count],3,Scalar(255,255,255),2,8);
    }
    float ref_right = (contour[corner0].x+contour[corner2].x)/2;
    float ref_top = (contour[corner2].y+contour[corner3].y)/2;
    float ref_left = (contour[corner3].x+contour[corner1].x)/2;
    float ref_bottom = (contour[corner1].y+contour[corner0].y)/2;

    float max_dist = 0;
    float dist = 0;
    int max_idx = 0;
    for(int i = 0; i < contour_right.size(); i++){
        dist = abs(ref_right-contour_right[i].x);
        if(dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }

    contour_right.insert(contour_right.begin(),corners[0]);
    contour_right.push_back(corners[2]);

    contour_top.insert(contour_top.begin(),corners[2]);
    contour_top.push_back(corners[3]);

    contour_left.insert(contour_left.begin(),corners[3]);
    contour_left.push_back(corners[1]);

    contour_bottom.insert(contour_bottom.begin(),corners[1]);
    contour_bottom.push_back(corners[0]);

    /*-------------------------------------*/
    //ecken Korrektur rechte Kontur
    /*-------------------------------------*/

    vector<Point> contour_r;
    double laenge = 0;
    int idx = 0, counter = 0, c = 0, d = 0;

    while (counter < (contour_right.size()-1)) {
        counter++;
        c++;
        laenge = sqrt((contour_right[idx].x - contour_right[idx + c].x) * (contour_right[idx].x - contour_right[idx + c].x) +
                      (contour_right[idx].y - contour_right[idx + c].y) * (contour_right[idx].y - contour_right[idx + c].y));
        if (laenge > 4) {
            d++;
            contour_r.push_back(contour_right[idx]);
            idx = counter;
            c = 0;
        }
    }

    float k = 0;
    int correct_count = 0;
    int correct_idx = 0;
    for(correct_count = 0; correct_count < contour_r.size()-2; correct_count++){

        k = pitch2Points(contour_r[correct_count],contour_r[correct_count+1]);
        //cout << "unten: " << k << endl;
        if(k >= 2) {
            correct_idx = correct_count;
            correct_count = contour_r.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
    if(correct_idx > 0){
        circle(drawing, contour_r[correct_idx], 10, Scalar(0, 255, 0), 2, 8);
        corners[0] = contour_r[correct_idx];
    }
    /*-----------------------------------*/
    vector<Point> contour_r1;
    laenge = 0;
    idx = contour_right.size()-1, counter = contour_right.size()-1, c = 0, d = 0;

    while (counter > 1) {
        counter--;
        c--;
        laenge = sqrt((contour_right[idx].x - contour_right[idx + c].x) * (contour_right[idx].x - contour_right[idx + c].x) +
                      (contour_right[idx].y - contour_right[idx + c].y) * (contour_right[idx].y - contour_right[idx + c].y));
        if (laenge > 4) {
            contour_r1.push_back(contour_right[idx]);
            idx = counter;
            c = 0;
        }
    }


    k = 0;
    correct_count = 0;
    correct_idx = 0;
    for(correct_count = 0; correct_count < contour_r1.size()-2; correct_count++){

        k = pitch2Points(contour_r1[correct_count],contour_r1[correct_count+1]);
       // cout << "oben: " << k << endl;
        if(k >= 2) {
            correct_idx = correct_count;
            correct_count = contour_r1.size();
        }
    }
    /*-----------------------------------*/
    if(correct_idx > 0){
        circle(drawing, contour_r1[correct_idx], 10, Scalar(0, 0, 255), 2, 8);
        corners[2] = contour_r1[correct_idx];
    }
    /*-----------------------------------*/

    /*-------------------------------------*/
    //ecken Korrektur links Kontur
    /*-------------------------------------*/

    vector<Point> contour_l;
    laenge = 0;
    idx = 0, counter = 0, c = 0, d = 0;

    while (counter < (contour_left.size()-1)) {
        counter++;
        c++;
        laenge = sqrt((contour_left[idx].x - contour_left[idx + c].x) * (contour_left[idx].x - contour_left[idx + c].x) +
                      (contour_left[idx].y - contour_left[idx + c].y) * (contour_left[idx].y - contour_left[idx + c].y));
        if (laenge > 4) {
            d++;
            contour_l.push_back(contour_left[idx]);
            idx = counter;
            c = 0;
        }
    }

    k = 0;
    correct_count = 0;
    correct_idx = 0;
    for(correct_count = 0; correct_count < contour_l.size()-2; correct_count++){

        k = pitch2Points(contour_l[correct_count],contour_l[correct_count+1]);
       // cout << "oben _links: " << k << endl;
        if(k >= 2) {
            correct_idx = correct_count;
            correct_count = contour_l.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
    if(correct_idx > 0){
        circle(drawing, contour_l[correct_idx], 8 , Scalar(0, 255, 0), 2, 8);
        corners[3] = contour_l[correct_idx];
    }
    /*-----------------------------------*/
    vector<Point> contour_l1;
    laenge = 0;
    idx = contour_left.size()-1, counter = contour_left.size()-1, c = 0, d = 0;

    while (counter > 1) {
        counter--;
        c--;
        laenge = sqrt((contour_left[idx].x - contour_left[idx + c].x) * (contour_left[idx].x - contour_left[idx + c].x) +
                      (contour_left[idx].y - contour_left[idx + c].y) * (contour_left[idx].y - contour_left[idx + c].y));
        if (laenge > 4) {
            contour_l1.push_back(contour_left[idx]);
            idx = counter;
            c = 0;
        }
        if(correct_count > 500)
            break;
    }
    k = 0;
    correct_count = 0;
    correct_idx = 0;
   // cout << "unten _links_länge" << contour_l1.size() << endl;
    for(correct_count = 0; correct_count < contour_l1.size(); correct_count++){

        k = pitch2Points(contour_l1[correct_count],contour_l1[correct_count+1]);
        //circle(drawing, contour_l1[correct_count], 8, Scalar(0, 0, 255), 2, 8);
       // cout << "unten _links: " << k << endl;
        if(k >= 2) {
          //  cout << "end _links: " << endl;
            correct_idx = correct_count;
            correct_count = contour_l1.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
  //  cout << "unten _links_correct: " << correct_idx << endl;
    if(correct_idx > 0){
        circle(drawing, contour_l1[correct_idx], 8, Scalar(0, 0, 255), 2, 8);
        corners[1] = contour_l1[correct_idx];
    }
    /*-----------------------------------*/

    /*-------------------------------------*/
    //ecken Korrektur oben Kontur
    /*-------------------------------------*/

    vector<Point> contour_t;
    laenge = 0;
    idx = 0, counter = 0, c = 0, d = 0;

    while (counter < (contour_top.size()-1)) {
        counter++;
        c++;
        laenge = sqrt((contour_top[idx].x - contour_top[idx + c].x) * (contour_top[idx].x - contour_top[idx + c].x) +
                      (contour_top[idx].y - contour_top[idx + c].y) * (contour_top[idx].y - contour_top[idx + c].y));
        if (laenge > 4) {
            d++;
            contour_t.push_back(contour_top[idx]);
            idx = counter;
            c = 0;
        }
        if(correct_count > 500)
            break;
    }

    k = 0;
    correct_count = 0;
    correct_idx = 0;
    for(correct_count = 0; correct_count < contour_t.size()-2; correct_count++){

        k = pitch2Points(contour_t[correct_count],contour_t[correct_count+1]);
       // cout << "top _rechts: " << k << endl;
        if(k <= 2) {
            correct_idx = correct_count;
            correct_count = contour_t.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
    if(correct_idx > 0){
        circle(drawing, contour_t[correct_idx], 8 , Scalar(255, 255, 255), 2, 8);
        corners[2] = contour_t[correct_idx];
    }
    /*-----------------------------------*/
    vector<Point> contour_t1;
    laenge = 0;
    idx = contour_top.size()-1, counter = contour_top.size()-1, c = 0, d = 0;

    while (counter > 1) {
        counter--;
        c--;
        laenge = sqrt((contour_top[idx].x - contour_top[idx + c].x) * (contour_top[idx].x - contour_top[idx + c].x) +
                      (contour_top[idx].y - contour_top[idx + c].y) * (contour_top[idx].y - contour_top[idx + c].y));
        if (laenge > 4) {
            contour_t1.push_back(contour_top[idx]);
            idx = counter;
            c = 0;
        }
        if(correct_count > 500)
            break;
    }
    k = 0;
    correct_idx = 0;
   // cout << "oben _links_lange" << contour_t1.size() << endl;
    for(correct_count = 0; correct_count < contour_t1.size(); correct_count++){
        k = pitch2Points(contour_t1[correct_count],contour_t1[correct_count+1]);
        //circle(drawing, contour_l1[correct_count], 8, Scalar(0, 0, 255), 2, 8);
       // cout << "oben _links: " << k << endl;
        if(k < 2) {
        //    cout << "end_oben _links " << endl;
            correct_idx = correct_count;
            correct_count = contour_t1.size();
        }
    }
    /*-----------------------------------*/
   // cout << "unten _top_correct: " << correct_idx << endl;
    if(correct_idx > 0){
        circle(drawing, contour_t1[correct_idx], 8, Scalar(0, 255, 255), 2, 8);
        corners[3] = contour_t1[correct_idx];
    }
    /*-------------------------------------*/

    /*-------------------------------------*/
    //ecken Korrektur unten Kontur
    /*-------------------------------------*/

    vector<Point> contour_b;
    laenge = 0;
    idx = 0, counter = 0, c = 0, d = 0;

    while (counter < (contour_bottom.size()-1)) {
        counter++;
        c++;
        laenge = sqrt((contour_bottom[idx].x - contour_bottom[idx + c].x) * (contour_bottom[idx].x - contour_bottom[idx + c].x) +
                      (contour_bottom[idx].y - contour_bottom[idx + c].y) * (contour_bottom[idx].y - contour_bottom[idx + c].y));
        if (laenge > 4) {
            d++;
            contour_b.push_back(contour_bottom[idx]);
            idx = counter;
            c = 0;
        }
        if(correct_count > 500)
            break;
    }

    k = 0;
    correct_count = 0;
    correct_idx = 0;
    for(correct_count = 0; correct_count < contour_b.size()-2; correct_count++){

        k = pitch2Points(contour_b[correct_count],contour_b[correct_count+1]);
       // cout << "bottom _rechts: " << k << endl;
        if(k <= 2) {
            correct_idx = correct_count;
            correct_count = contour_b.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
    if(correct_idx > 0){
        circle(drawing, contour_b[correct_idx], 8 , Scalar(255, 255, 255), 2, 8);
        corners[1] = contour_b[correct_idx];
    }
    /*-----------------------------------*/
    vector<Point> contour_b1;
    laenge = 0;
    idx = contour_bottom.size()-1, counter = contour_bottom.size()-1, c = 0, d = 0;

    while (counter > 1) {
        counter--;
        c--;
        laenge = sqrt((contour_bottom[idx].x - contour_bottom[idx + c].x) * (contour_bottom[idx].x - contour_bottom[idx + c].x) +
                      (contour_bottom[idx].y - contour_bottom[idx + c].y) * (contour_bottom[idx].y - contour_bottom[idx + c].y));
        if (laenge > 4) {
            contour_b1.push_back(contour_bottom[idx]);
            idx = counter;
            c = 0;
        }
        if(correct_count > 500)
            break;
    }
    k = 0;
    correct_idx = 0;
  //  cout << "unten _links_lange" << contour_b1.size() << endl;
    for(correct_count = 0; correct_count < contour_b1.size(); correct_count++){
        k = pitch2Points(contour_b1[correct_count],contour_b1[correct_count+1]);
        //circle(drawing, contour_l1[correct_count], 8, Scalar(0, 0, 255), 2, 8);
     //   cout << "oben _links: " << k << endl;
        if(k < 2) {
       //     cout << "end_bottom _links " << endl;
            correct_idx = correct_count;
            correct_count = contour_b1.size();
        }
        if(correct_count > 500)
            break;
    }
    /*-----------------------------------*/
   // cout << "unten _bottom_correct: " << correct_idx << endl;
    if(correct_idx > 0){
        circle(drawing, contour_b1[correct_idx], 8, Scalar(0, 255, 255), 2, 8);
        corners[0] = contour_b1[correct_idx];
    }
    /*-----------------------------------*/

    unsigned char tabs = 0;
    circle(drawing,contour_right[max_idx],10,Scalar(255,0,255),2,8);
    if (ref_right - contour_right[max_idx].x <= -20)
        tabs |= (2 << RIGHT);
    if (ref_right - contour_right[max_idx].x >= 20)
        tabs |= (1 << RIGHT);
    if (abs(ref_right - contour_right[max_idx].x) < 20)
        tabs |= (0 << RIGHT);

    max_dist = 0;
    dist = 0;
    max_idx = 0;
    for(int i = 0; i < contour_top.size(); i++){
        dist = abs(ref_top-contour_top[i].y);
        if(dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }
    circle(drawing,contour_top[max_idx],10,Scalar(255,0,255),2,8);
    if (ref_top - contour_top[max_idx].y <= -20)
        tabs |= (1 << TOP);
    if (ref_top - contour_top[max_idx].y >= 20)
        tabs |= (2 << TOP);
    if (abs(ref_top - contour_top[max_idx].y) < 20)
        tabs |= (0 << TOP);

    max_dist = 0;
    dist = 0;
    max_idx = 0;
    for(int i = 0; i < contour_left.size(); i++){
        dist = abs(ref_left-contour_left[i].x);
        if(dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }
    circle(drawing,contour_left[max_idx],10,Scalar(255,0,255),2,8);
    if (ref_left - contour_left[max_idx].x <= -20)
        tabs |= (1 << LEFT);
    if (ref_left - contour_left[max_idx].x >= 20)
        tabs |= (2 << LEFT);
    if (abs(ref_left - contour_left[max_idx].x) < 20)
        tabs |= (0 << LEFT);

    max_dist = 0;
    dist = 0;
    max_idx = 0;
    for(int i = 0; i < contour_bottom.size(); i++){
        dist = abs(ref_bottom-contour_bottom[i].y);
        if(dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }
    circle(drawing,contour_bottom[max_idx],10,Scalar(255,0,255),2,8);
    if (ref_bottom - contour_bottom[max_idx].y <= -20)
        tabs |= (2 << BOTTOM);
    if (ref_bottom - contour_bottom[max_idx].y >= 20)
        tabs |= (1 << BOTTOM);
    if (abs(ref_bottom - contour_bottom[max_idx].y) < 20)
        tabs |= (0 << BOTTOM);

    //cout << bitset<sizeof(char) * CHAR_BIT> (tabs) <<  "b\n";

    for(int i = 0; i < 4; i++){
        circle(source, corners[i], 8, Scalar(0, 255, 0), 2, 8);
    }

    if(DISPLAY)imshow("corners",drawing);
    if(DISPLAY)imshow("corrected_corners",source);
    if(DISPLAY)waitKey(0);
    return tabs;
}

int main() {
    analyseParts analyse(1008);
    vector<Part> parts;
    Part buf;
    unsigned char tabs = 0;
    for(int i = 0; i < 1008; i++){
        tabs = analyse.getTabs(i);
        buf.setConnections(tabs);
        parts.push_back(buf);
    }

    //Zugriff auf den vector mit den einzelnen teilen: part[0].getConnenctions() entspricht pömpel von bild 0.jpg und liefert ein unsigned char, poempl Belegung wie ausgemacht

    return 0;
}