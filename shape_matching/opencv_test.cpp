#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;




Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

Mat zentrieren (string name)
{
    Mat bild =imread(name);
    Mat src; Mat bild_gray;Mat bild_re;Mat bild_thr;
    float x=0;
    float y=0;
    int flaeche=0;

    /// Convert image to gray and blur it
    cvtColor(bild, bild_gray, COLOR_BGR2GRAY);
    blur(bild_gray, bild_gray, Size(2, 2));

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny

    threshold(bild_gray, bild_thr, 200, 255, THRESH_BINARY);
    /// Find contours
    findContours(bild_thr, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++) { mu[i] = moments(contours[i], false); }

    ///  Get the mass centers:
    vector<Point2f> mc(contours.size());

    for( int i = 0; i < contours.size(); i++ )
    {
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        if(mc[i].x>0 &&mc[i].y>0)
        {
            x = x+ mc[i].x*contourArea(contours[i]);
            y = y+ mc[i].y*contourArea(contours[i]);
            flaeche+=contourArea(contours[i]);
        }

    }
    int top, bottom, left, right,x_kor,y_kor;
    x_kor=x/flaeche;
    y_kor=y/flaeche;
    cout<<x_kor<<"  "<<y_kor<<endl;
    left = 260 / 2 - x_kor;
    right = 260 - left - bild.cols;
    top = 260 / 2 - y_kor;
    bottom = 260 - top - bild.rows;
    const cv::Scalar value(255, 255, 255);
    copyMakeBorder(bild_thr, bild_re, top, bottom, left, right, BORDER_CONSTANT,value);

    return bild_re;

}




int main( int argc, char** argv )
{

    String path("teile/*.jpg"); //select only jpg
    vector<cv::String> fn;
    vector<cv::Mat> data;
    glob(path,fn,true); // recurse
    for (size_t k=260; k<fn.size(); ++k)
    {
        cout<<k<<endl;
        Mat src = zentrieren(fn[k]);
       // Mat src = imread("1.jpg");
        int smallest=1;
        double value=0;
        double value_smallest=100;

        String path_match("Matches/*.jpg"); //select only jpg
        vector<cv::String> fn_match;
        vector<cv::Mat> data_match;
        glob(path_match,fn_match,true); // recurse
        for (size_t l=1; l<=18; l++)
        {
            Mat match =imread(fn_match[l]);
            cvtColor(match, match, COLOR_BGR2GRAY);
            threshold(match, match, 200, 255, THRESH_BINARY);
            //Mat match = imread("Match/1.jpg");
            int drehen=0;
            for (int m=0;m<4;m++)
            {
                value= norm(src,match,NORM_L1)/(match.rows*match.cols);
                cout<<value<<endl;
                if (value<value_smallest)
                {
                    value_smallest=value;
                    smallest=l;
                    cout<<value_smallest;
                }
                match = rotate(match,90);
                drehen++;
            }

        }
        cout<<smallest<<endl;
        namedWindow("anfang", WINDOW_AUTOSIZE);
        imshow("anfang",src);
        Mat best= imread(fn_match[smallest]);
        namedWindow("best", WINDOW_AUTOSIZE);
        imshow("best",best);
        waitKey(0);
   }

    return(0);

}




/*


#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;




Mat zentrieren (string name)
{
    Mat bild =imread(name);
    Mat src; Mat bild_gray;Mat bild_re;Mat bild_thr;
    float x=0;
    float y=0;
    int flaeche=0;

    /// Convert image to gray and blur it
    cvtColor(bild, bild_gray, COLOR_BGR2GRAY);
    blur(bild_gray, bild_gray, Size(2, 2));


    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny

    threshold(bild_gray, bild_thr, 200, 255, THRESH_BINARY);
    /// Find contours
    findContours(bild_thr, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++) { mu[i] = moments(contours[i], false); }

    ///  Get the mass centers:
    vector<Point2f> mc(contours.size());

    for( int i = 0; i < contours.size(); i++ )
    {
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        if(mc[i].x>0 &&mc[i].y>0)
        {
            x = x+ mc[i].x*contourArea(contours[i]);
            y = y+ mc[i].y*contourArea(contours[i]);
            flaeche+=contourArea(contours[i]);
        }

    }
    int top, bottom, left, right,x_kor,y_kor;
    x_kor=x/flaeche;
    y_kor=y/flaeche;
    cout<<x_kor<<"  "<<y_kor<<endl;
    left = 260 / 2 - x_kor;
    right = 260 - left - bild.cols;
    top = 260 / 2 - y_kor;
    bottom = 260 - top - bild.rows;
    const cv::Scalar value(255, 255, 255);
    copyMakeBorder(bild_thr, bild_re, top, bottom, left, right, BORDER_CONSTANT,value);



    return bild_re;

}




int main( int argc, char** argv )
{

        String name ="Matches/18.jpg";
        Mat src = zentrieren(name);

        namedWindow("ergebnis", WINDOW_AUTOSIZE);
        imshow("ergebnis", src);
        imwrite(name,src);
        waitKey(0);


    return(0);

}
*/