// This cpp contains everything so far. Project isn't using header files yet




#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#include <vector>
#include <math.h>
#include <iostream>

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

//using namespace std;

//typedefs:

typedef std::vector<std::vector<Point>> Contour_t;
typedef std::vector<Vec4i> Hierarchy_t; //hierarchy object is needed for drawContours apparently

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


// Function headers:

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

Mat segmentThresh_bad(Mat src);
Mat drawLargestContour(Mat, int, bool);
Mat segmentThresh(Mat);
Mat segmentCanny(Mat);
Mat segment(Mat);
Mat correct(Mat);
Mat processRawImage(int fileIndex = 0);
Mat processRawImageBB(int fileIndex = 0);
Mat saveImage(int , Mat);
Mat resultImage();
Contour_t getLongestContour(Mat); //  depracated
int getLongestContourIndex(Contour_t);

int findmax(std::vector<int> vals);
int findmin(std::vector<int> vals);
float findmax(std::vector<float> vals);
float findmin(std::vector<float> vals);

std::vector<int> histogram(std::vector<int> data, int rangeWidth); // discrete histogram
std::vector<int> cont_histogram(std::vector<int> data);
void drawHistogram(vector<int> hist);

int main() {
    for(int i=0;i<1008;i++){
        //Mat res = processRawImage(i);
        Mat res = processRawImage(i);
        //imshow("res",res);
        // waitKey(0);
        //imshow("res",res);
        int nonZeros =  countNonZero(res);
        //cout<<"nonZero: "<<nonZeros<<endl;
        Mat straight;
        // if segmentation failed don't even try to straighten, because it can crash the program...
        if(nonZeros>1500){
            straight = correct(res);
        }
        else{
            straight = Mat::zeros(res.size(),CV_8UC3);
            putText(straight, "Segmentation failed", cvPoint(120,120),
                    FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
        }

        // imshow("straight", straight);
        // waitKey(0);
        char outputFile [100];
        sprintf(outputFile,"../outputVZ/corrected_%d.jpg",i);
        imwrite(outputFile,straight);
        // saveImage(i,res);

    }
    return 0;
}

Mat processRawImageBB(int fileIndex){

    char inputFile [100];
    if(fileIndex<10){
        sprintf(inputFile,"../input/000%d.jpg",fileIndex);
    }
    else if(fileIndex>=10 && fileIndex<100){
        sprintf(inputFile,"../input/00%d.jpg",fileIndex);
    }
    else if(fileIndex>=100 && fileIndex<1000){
        sprintf(inputFile,"../input/0%d.jpg",fileIndex);
    }
    else if(fileIndex>=1000){
        sprintf(inputFile,"../input/%d.jpg",fileIndex);
    }
    //cout<<inputFile<<endl;
    Mat source = imread(inputFile,1);
    //imshow("source",source);
    //waitKey(0);
    Mat mask = segment(source);
    return mask;

}

Mat processRawImage(int fileIndex){

    char inputFile [100];
    sprintf(inputFile,"../input/%d.jpg",fileIndex);

    Mat source = imread(inputFile,1);
    //imshow("source",source);
    //waitKey(0);
    Mat mask = segment(source);
    return mask;

}
Mat saveImage(int fileIndex,  const Mat img) {
    Mat saveMat = img;
    cvtColor(saveMat,saveMat, CV_GRAY2BGR,3);
    char outputFile [100];
    sprintf(outputFile,"../output/%d.jpg",fileIndex);
    imwrite(outputFile,saveMat);
}

//Mat resultImage( SolutionElement solutionMx[noCols][noRows]){

Mat segment(Mat src){
    Mat copySrc = src;
    Mat copy2Src = src;
    Mat threshMask = segmentThresh(src);
    Mat cannyMask = segmentCanny(copySrc);
    Mat threshBin;
    Mat cannyBin;

    threshold( threshMask, threshBin, 100,255,THRESH_BINARY );
    threshold( cannyMask, cannyBin, 100,255,THRESH_BINARY );

    int threshResult = countNonZero(threshBin);
    int cannyResult = countNonZero(cannyBin);

    //std::cout<<"Threshresult: "<< threshResult<<std::endl;
    //std::cout<<"cannyResult: "<< cannyResult<<std::endl;
    //imshow("Thresh",threshMask);
    //imshow("Canny", cannyMask);
    //waitKey(0);


    if(threshResult>cannyResult){
        return threshMask;
    }
    else{
        return cannyMask;
    }




}

Mat segmentThresh_bad(Mat src){
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
    threshold(gray,bw,180,255,CV_THRESH_BINARY_INV);
    // adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 5,2);
    //imshow("bw",bw);
    int padding = 100;
    copyMakeBorder(bw, bw, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    copyMakeBorder(src, src, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));

    ////////////////////////////////////////////////////////////
    // Contour closing algorithm should come here when ready:
    //*
    //*
    //*
    ///////////////////////////////////////////////////////////

    double maskPerimeter = 0;
    int approxStrength = 2;
    Mat mask = drawLargestContour(bw,approxStrength,true);
    int morph_size = 3;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    // Point morph size makes it omnidirectional
    morphologyEx(mask, mask, MORPH_CLOSE, element);

    //imshow("mask",mask);
    //waitKey(0);
    return mask;
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
    Mat element = getStructuringElement(MORPH_ELLIPSE,Size (morph_size,morph_size));
    // Mat kernel = Mat::ones(1,1, CV_32F);
    morphologyEx(bw, bw, MORPH_CLOSE, element, Point(-1,-1), 3);

    //imshow("closed",bw);
    // Find all the contours in the thresholded image
    std::vector<Vec4i> hierarchy;
    Mat mask = drawLargestContour(bw,1,true);

    return mask;
}

Mat segmentCanny(Mat src){

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // Convert image to binary
    Mat bw;

    adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 41,20); //(31, 20) (41,20)
    int padding = 100;
    copyMakeBorder(gray, gray, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    copyMakeBorder(src, src, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));


    int morph_size =1;
    Mat element = getStructuringElement( MORPH_RECT, Size(morph_size,morph_size) );
    morphologyEx(gray, bw, MORPH_OPEN, element);

    morph_size =3;
    element = getStructuringElement( MORPH_RECT, Size(morph_size,morph_size) );
    morphologyEx(bw, bw, MORPH_CLOSE, element);


    Canny(bw, bw, 100, 200, 3);
    morph_size =5;
    element = getStructuringElement( MORPH_ELLIPSE, Size(morph_size,morph_size) );
    morphologyEx(bw, bw, MORPH_CLOSE, element);


    // Find all the contours in the thresholded image
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
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

        //drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 3, 8, hierarchy, 0);
    }

    std::vector<std::vector<Point> >  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;

    int strength = 3; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    Mat mask =  Mat::zeros(src.size(), CV_8UC1);


    drawContours(mask, largestContour,index, 255, CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    //imshow("mask", mask);
    morph_size =5;
    element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(mask, mask, MORPH_CLOSE, element, Point(-1,-1), 1);
    return mask;
}

Mat drawLargestContour(Mat bw, int approxStrength, bool filled){

    Mat mask =  Mat::zeros(bw.size(), CV_8UC1);
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int largestIndex = getLongestContourIndex(contours);
    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = approxStrength; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), contours[largestIndex], strength, true);
    int lineThickness = 2;
    if(filled){
        drawContours(mask, contours,largestIndex, 255,CV_FILLED , 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }
    else{
        drawContours(mask, contours,largestIndex, 255, lineThickness, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }

    return(mask);
}

Contour_t getLongestContour(Mat bw){

    Mat mask =  Mat::zeros(bw.size(), CV_8UC1);
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue; //
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
    }

    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = 2; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    return(largestContour);
}

int getLongestContourIndex(Contour_t contours){
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue; //
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
    }
    return(largestIndex);
}

Mat correct(Mat src) {
// input should be 1 channel black and white image can be filled or  contoured
    //counter++;
    //cout<<"counter: "<<counter<<endl;
    //imshow("input",src);
    // IMPORTANT CONSTANTS THAT CAN BE TWEAKED:

    float angleTolerance = 6; // range width of the histogram in degrees
    size_t maxIndex = 2; // The first three dominant segment groups will be examined

    // weighting factors:
    float oFactor = 2; // how important it is to have orthogonal matches
    float pFactor = 2; // how important it is to have parallel matches
    float lengthFactor =3; // how important is the length of the segment
    float matchLimit = 15; // which angle interval counts as a match
    int lineThresh = 20;
    double minLineLength = 5;
    double maxLineGap = 3;

    // input should be 1 channel black and white image

    //## Rotating the image using minimum area bounding rectangle:

    Mat bw;
    threshold(src, bw, 150, 255, CV_THRESH_BINARY); // INV: invert for findContours to work properly
    bw = drawLargestContour(bw,1,false);
    //imshow("bwww",bw);
    // Find all the contours in the thresholded image
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    int index = getLongestContourIndex(contours);
    Mat mask =  Mat::zeros(bw.size(), CV_8UC3);
    drawContours(mask, contours, static_cast<int>(index), Scalar(255,255,255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    RotatedRect box;
    box = minAreaRect(contours[index]);
    Point2f rect_points[4];
    box.points( rect_points );
    double alpha = 0;

    YouCanCommentLikeThisLOL:
    //Draw the rectangle:
    for( int j = 0; j < 4; j++ )
        line( mask, rect_points[j], rect_points[(j+1)%4], Scalar(0,255,0), 2, 8 );

    alpha =  atan2( (rect_points[1].x-rect_points[0].x) , (rect_points[1].y-rect_points[0].y)  );
    alpha = alpha*180/CV_PI;
    //std::cout<<alpha;

    //Rotate conter clockwise by alpha:
    Point2f center;
    center.x = mask.cols/2;
    center.y = mask.rows/2;
    Mat RotMatrix = getRotationMatrix2D(center,int(-alpha),1);
    warpAffine(mask,mask,RotMatrix, mask.size());

    //## Done rotating the image using minimum area rectangle

    // ##Find the angle of all line segments and create a histogram, then match parallel and orthogonal lines to approximate
    // the orientation of the shape


    std::vector<Vec4i> lines;



    Mat contourImage =  Mat::zeros(bw.size(), CV_8UC3);
    contourImage =  drawLargestContour(bw,1,false);
    //imshow("bw",bw);
    //waitKey(0);
    HoughLinesP(bw, lines, 1, CV_PI/180, lineThresh, minLineLength, maxLineGap); //needs binary image!!
    Mat lineImage = Mat::zeros(mask.size(),CV_8UC3);
    std::vector<Vec6f> segments; // Similar to vector<Vec4i> lines but with 2 additional data: Angle to x axis and length
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec6f lineObject;
        //calculate the length of the line:
        Point first = Point(lines[i][0], lines[i][1]);
        Point second = Point(lines[i][2], lines[i][3]);
        Point segment = second-first;
        float length = float(sqrt((segment.x)*(segment.x)+(segment.y)*(segment.y)));
        //cout<<"length calculated: "<<length<<endl;
        float rawAngle = float((180/CV_PI)*atan2(segment.x,segment.y)); // angle between 0 and 180 degs
        int theta = int(std::round(rawAngle)); // angle between 0 and 90°
        //cout<<"th: "<<theta<<endl;

        //cout<<"theta: "<<theta<<endl;
        for(int j = 0; j< 4; j++){
            lineObject[j] = (lines[i][j]);
        }
        lineObject[4] = theta;
        lineObject[5] = float(length);
        segments.push_back(lineObject);
        // At this point we have a lineObject that contains x0, y0, x1, y1, the length and angle of a line
        if(length < 30){
            // Blue:
            line( lineImage, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(244,100,0), 1, 1 );
        }
        else{
            // Blue:
            line( lineImage, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(244,100,0), 1, 1 );
            //line( lineImage, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0,255,255), 1, 1 );
        }
    }
    //imshow("lines", lineImage);
    //waitKey(0);
    vector<int>angles;
    for(size_t i = 0; i<segments.size();i++){
        angles.push_back(int(segments[i][4]));
    }


    vector<int> angleHist = histogram(angles,int(angleTolerance));
    // drawHistogram(angleHist);

    // Calculate the average frequency of any angle (histogram):

    double sum = 0;
    double noNonZero  = 0;
    for(size_t i = 0;i<angleHist.size(); i++){
        sum += angleHist[i];
        if(angleHist[i]!=0){
            noNonZero ++;
        }
    }
    double average = 0;
    if(noNonZero!=0){
        average = sum/noNonZero;
    }

    // Finding the segments that are statistifically significant (not noise):

    vector<Vec6f> significantSegments;
    for(size_t i = 0;i<angleHist.size(); i++) {
        if(angleHist[i]>int(average)){
            float angle =  angleTolerance*(i+1)-angleTolerance/2; // we get the angle that is in the middle of the range
            // cout<<"Possibly statistically significant angles: "<<angle<<endl;
            Vec6f segment ;


            segment[4] = angle;
            significantSegments.push_back(segment);
        }
    }

    // Arranging the significant segments into groups based on their angles (parallelity):

    vector<vector<Vec6f>>groups;
    groups.resize(significantSegments.size());
    for(size_t i = 0;i<segments.size(); i++){
        float ang = segments[i][4];
        //cout <<endl<<"angle to examine: "<< ang<<endl;
        for(size_t j = 0;j< significantSegments.size(); j++){
            float significantAngle =  significantSegments[j][4];
            // cout <<endl<<"significant angle to compare with: "<< significantAngle<<endl;
            if( significantAngle-angleTolerance/2 < ang && ang <=significantAngle+angleTolerance/2) {
                // cout<<"length "<< segments[i][5] <<endl;

                groups[j].push_back(segments[i]); // Sorting every statistically singnificant segment into a group
            }

        }
    }

    // Constructing a single average segment from every group of segments:

    vector<Vec6f> averageSegments;
    vector<float> lengths;
    for(size_t i = 0;i<groups.size(); i++){
        //cout<<endl<< "Group"<< i<<": "<<endl;
        float length_sum = 0;
        float angle_sum = 0;
        float angle_avg = 0;
        float angle_divider = 0;
        float x0_sum = 0;
        float y0_sum = 0;
        float x1_sum = 0;
        float y1_sum = 0;
        for(size_t j = 0;j<groups[i].size(); j++){
            //cout<< "angle = "<<groups[i][j][4]<<endl;
            length_sum+= groups[i][j][5];
            angle_sum += (groups[i][j][4])*(groups[i][j][5]); // need a weighted average!!!!!!
            angle_divider += groups[i][j][5];
            x0_sum += groups[i][j][0];
            y0_sum += groups[i][j][1];
            x1_sum += groups[i][j][2];
            y1_sum += groups[i][j][3];
        }
        angle_avg = angle_sum/angle_divider;
        // cout<<"Sum of lengths: "<<length_sum<<endl;
        // cout<<"Average angle "<<angle_avg<<endl;
        Vec6f averageSegment;
        averageSegment[0] = x0_sum/float(groups[i].size());
        averageSegment[1] = y0_sum/float(groups[i].size());
        averageSegment[2] = x1_sum/float(groups[i].size());
        averageSegment[3] = y1_sum/float(groups[i].size());

        averageSegment[4] = angle_avg;
        averageSegment[5] = length_sum;

        averageSegments.push_back(averageSegment);
        lengths.push_back(length_sum);

    }
    // super ugly sorting algorithm dont even try to understand :P
    vector<Vec6f> sortedSegments;
    while(!lengths.empty()){
        // float minLength =  findmin(lengths); // ascending order
        float maxLength =  findmax(lengths); // descending order
        for(size_t i = 0;i<averageSegments.size(); i++){
            if(averageSegments[i][5]==maxLength){
                lengths.erase(lengths.begin()+ i);
                sortedSegments.push_back(averageSegments[i]);
                averageSegments.erase(averageSegments.begin() +i);
            }
        }
    } //^^^^CANT BELIEVE THIS WORKS^^^^

    //Draw the sorted segments (yellow lines)
    for(size_t i =0;i<sortedSegments.size(); i++){
        //cout<<"sorted lengths in descending order: "<<sortedSegments[i][5]<<endl;
        line( lineImage, Point(int(sortedSegments[i][0]),int(sortedSegments[i][1])), Point(int(sortedSegments[i][2]),int( sortedSegments[i][3])), Scalar(0,255,255), 2, 2 );


    }

    // Match the segments with parallel and orthogonal ones:
    int depth = sortedSegments.size();
    int iteration = 0;

    vector<Vec6f> matchedSegments;
    //matchedSegments.resize(sortedSegments.size());


    while(iteration<depth){
        // First check the longest line, then the second longest and son on...
        Vec6f masterSegment = sortedSegments[iteration];
        Vec6f matchedSegment; // [0]: num of orthogonal matches, [1] num of parallel matches
        float masterAngle = masterSegment[4];

        matchedSegment[3] = masterSegment[5];
        for(int i = iteration + 1; i < sortedSegments.size();  i++ ){

            float examinedAngle = sortedSegments[i][4];

            /*
            if(masterAngle>90){
                masterAngle = masterAngle-180;
            }
            if(examinedAngle>90){
                examinedAngle = examinedAngle-180;
            }
            */


            // Check for parallellity:
            float epsilon = abs( masterAngle - examinedAngle ); // find the angle distance from master
            if( epsilon > 90){
                epsilon = 180-epsilon;
            }

            //cout<<"Parallelity factor between master: "<<masterAngle<<" and examined: "<<examinedAngle<<"is: "<<epsilon<<endl;
            // Check for parallellity:
            if(epsilon <=matchLimit){
                // We add the length of the line we matched with, and normalize it with the quality of the match (angle distance)
                matchedSegment[0] += sortedSegments[i][5]/epsilon;

            }
            // Check for orthogonality:
            epsilon = abs(90-(abs( masterAngle - examinedAngle ))); // find the angle distance from master
            //cout<<"Orthogonality factor between master: "<<masterAngle<<" and examined: "<<examinedAngle<<"is: "<<epsilon<<endl;
            // Check for parallellity:
            if(epsilon <=matchLimit){
                // We add the length of the line we matched with, and normalize it with the quality of the match (angle distance)
                matchedSegment[1] += sortedSegments[i][5]/epsilon;

            }


        }
        matchedSegment[2] = masterAngle;
        matchedSegments.push_back(matchedSegment);

        iteration++;

    }
    // cout<<endl<<endl;



    // Calculate weighted sum pMatches, oMatches + length:
    for(size_t i =0; i < matchedSegments.size();  i++ ) {
        float wSum =  pFactor * matchedSegments[i][0] + oFactor*matchedSegments[i][1] +lengthFactor*matchedSegments[i][3];
        lengths.push_back(wSum); // recycling lenghts vector that was emptied earlier
        matchedSegments[i][4] = wSum;

    }
    // Sorting, again this time the matches:
    vector<Vec6f> sortedMatches;
    while(!lengths.empty()){
        // float minLength =  findmin(lengths); // ascending order
        float maxLength =  findmax(lengths); // descending order
        for(size_t i = 0;i<matchedSegments.size(); i++){
            if(matchedSegments[i][4]==maxLength){
                lengths.erase(lengths.begin()+ i);
                sortedMatches.push_back(matchedSegments[i]);
                matchedSegments.erase(matchedSegments.begin() +i);
            }
        }
    }

    // just outputting:
    /*
    for(size_t i =0; i < sortedMatches.size();  i++ ){
        float pMatches = sortedMatches[i][0];
        float oMatches = sortedMatches[i][1];
        float angle = sortedMatches[i][2];
       // cout<<"segment "<< i<<endl<<"parallelmatches: "<<pMatches<<", orth. matches: "<<  oMatches << ",angle: "
            //<<angle;
        //cout<<"Length: "<<sortedMatches[i][3]<< " WSum: "<<sortedMatches[i][4]<<endl;


    }
     */

    // parallelize orthogonal angles:
    for(size_t i =0; i < sortedMatches.size()-1;  i++ ){
        float angle = sortedMatches[i][2];
        float nextAngle = 0;

        nextAngle = sortedMatches[i+1][2];
        //cout<<"anlge: "<<angle<<"nextAngle :"<<nextAngle<<endl<<endl;

        float epsilon =abs (nextAngle-angle);
        if(epsilon > 90){
            if(nextAngle>angle){
                nextAngle = nextAngle-180;
            }
            else{
                nextAngle = nextAngle+180;
            }

        }
        epsilon =abs (nextAngle-angle);
        if(epsilon>45){
            if(nextAngle>angle){
                nextAngle = nextAngle-90;
            }
            else{
                nextAngle = nextAngle+90;
            }
        }
        sortedMatches[i+1][2] = nextAngle;
    }

    float angle_sum = 0;
    float weight = 0;
    for(size_t i =0; i < maxIndex && i < sortedMatches.size();  i++ ){
        // cout<<"angle "<<sortedMatches[i][2]<<endl;
        angle_sum+=sortedMatches[i][4] * sortedMatches[i][2];
        //cout<<"anglesum "<<angle_sum<<endl;
        weight+=sortedMatches[i][4];
    }
    float orientation = angle_sum/weight;
/*
    if(orientation > 90 ) {
        orientation =180-orientation;
    }
    if(orientation > 45 ) {
        orientation =orientation-90;
    }
    */
    //cout<<endl<<"Orientation: "<< orientation<<endl;

    //imshow("lines",lineImage);
    Point2f cent;
    cent.x = lineImage.cols/2;
    cent.y = lineImage.rows/2;
    Mat RotMx = getRotationMatrix2D(cent,int(-orientation),1);
    warpAffine(lineImage,lineImage,RotMx, lineImage.size());

    warpAffine(src,src,RotMx, src.size());
    // find significant lines, then find right angles between these lines (corners)
    //imshow("rotated",src);
    //waitKey(0);


    return src;

}

int findmax(std::vector<int> vals){
    int max=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(max<vals[i]) max=vals[i];
    return max;
}

float findmax(std::vector<float> vals){
    float max=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(max<vals[i]) max=vals[i];
    return max;
}

int findmin(std::vector<int> vals){
    int min=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(min>vals[i]) min=vals[i];
    return min;
}

float findmin(std::vector<float> vals){
    float min=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(min>vals[i]) min=vals[i];
    return min;
}

std::vector<int> histogram(std::vector<int> data, int rangeWidth){

    int dataMax = findmax(data);
    int noRanges = (dataMax/rangeWidth+1);
    std::vector<int> discreteFreq;
    for(int i = 0; i<noRanges; i++){
        discreteFreq.push_back(0);
    }
    //cout<<"number of ranges:"<<noRanges<<std::endl;
    //cout<<"max val:"<<dataMax<<std::endl;
    for(size_t i=0; i<data.size();i++) {
        for(int j = 0; j< noRanges; j++){
            double min = j*rangeWidth;
            double max = (j+1)*rangeWidth;
            //std::cout<<"Data: "<<data[i]<<std::endl;
            //std::cout<<"min: "<<min<<" max: "<<max<<std::endl;
            if(min < data[i] && data[i] <= max){
                //std::cout<<data[i]<<" : added to range: "<<j<<std::endl;
                discreteFreq[j]++;
            }
        }

    }
    return(discreteFreq);

}

std::vector<int> cont_histogram(std::vector<int> data){

    int dataMax = findmax(data);
    std::vector<int> frequency;
    for(int i = 0; i<=dataMax; i++){
        frequency.push_back(0);
    }
    for(size_t i = 0; i<data.size(); i++){
        frequency[data[i]]++;
    }
    return(frequency);

}

void drawHistogram(vector<int> hist){
    cout<<"Histogram: "<<endl;
    for(size_t i=0; i<hist.size();i++){

        std::cout<<"|";
        for(int j = 0;j<hist[i];j++){
            std::cout<<"x";
        }
        std::cout<<std::endl;
    }
    std::cout<<"============"<<std::endl;
}