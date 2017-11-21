

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    //! [load]

  String imageName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/Puzzle/4 copy.png";
   // String imageName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/Teile/4.jpg";
  //String imageName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/shape_sample/1.png";
     //String templateName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/Puzzle/1 copy.png";
    String templateName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/Templates/Bild2.png";
  //  String templateName = "/Users/nicolewaldhutter/Dropbox (BZH Taubinger)/MPK/shape_sample/2.png";

    //! [load]

    //! [mat]
    Mat image;
    Mat templ;
    //! [mat]

    //! [imread]
    image = imread( imageName, IMREAD_COLOR ); // Read the file
   templ = imread( templateName, IMREAD_COLOR ); // Read the file
    //! [imread]

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    if( templ.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the template" << std::endl ;
        return -1;
    }

    //! [window]
    namedWindow( "Display window1", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    imshow( "Display window1", image );                // Show our image inside it.
    //! [imshow]
    //! [window]
    namedWindow( "Display window2", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    imshow( "Display window2", templ);                // Show our image inside it.
    //! [imshow]
    //Grayscale matrix
    cv::Mat grayscaleMat1 (image.size(), CV_8U);
    cv::Mat grayscaleMat2 (templ.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor( image, grayscaleMat1, CV_BGR2GRAY );
    cv::cvtColor( templ, grayscaleMat2, CV_BGR2GRAY );

    //Binary image
    cv::Mat binaryMat1(grayscaleMat1.size(), grayscaleMat1.type());
    cv::Mat binaryMat2(grayscaleMat2.size(), grayscaleMat2.type());

    //Apply thresholding
    cv::threshold(grayscaleMat1, binaryMat1, 190, 255, cv::THRESH_BINARY_INV);
    cv::threshold(grayscaleMat2, binaryMat2, 190, 255, cv::THRESH_BINARY_INV);

    //Show the results
    cv::namedWindow("Output1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output1", binaryMat1);

    cv::namedWindow("Output2", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output2", binaryMat2);

    vector<vector<Point> > contours1,contours2;
    vector<Vec4i> hierarchy1,hierarchy2;

    /// Find contours
    findContours( binaryMat1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Find contours
    findContours( binaryMat2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    RNG rng(12345);
    Mat drawing1 = Mat::zeros( binaryMat1.size(), CV_8UC3 );
    for( int i = 0; i< contours1.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing1, contours1, i, color, 2, 8, hierarchy1, 0, Point() );
    }
    Mat drawing2 = Mat::zeros( binaryMat2.size(), CV_8UC3 );
    for( int i = 0; i< contours2.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing2, contours2, i, color, 2, 8, hierarchy2, 0, Point() );
    }

    /// Show in a window
    namedWindow( "Contours1", CV_WINDOW_AUTOSIZE );
    imshow( "Contours1", drawing1 );
    namedWindow( "Contours2", CV_WINDOW_AUTOSIZE );
    imshow( "Contours2", drawing2 );
    double ans1,ans2,ans3;



    cout<<a;
   // for(int i=0;i<contours1.size();i++)
    //{

        ans1=matchShapes(contours1[1], contours2[1], CV_CONTOURS_MATCH_I1,0.0);
        ans2=matchShapes(contours1[1], contours2[1], CV_CONTOURS_MATCH_I2,0.0);
        ans3=matchShapes(contours1[1], contours2[1], CV_CONTOURS_MATCH_I3,0.0);

  /*      double errorL2=norm(grayscaleMat1,grayscaleMat2,CV_L2);
        cout<<errorL2<<endl;
        cout<<ans1;
        cout<<ans2;
        cout<<ans3;*/
    //}
   /* for( int i = 0; i < contours1.size(); i++ ) {
        double area0 = contourArea(contours1[0]);
        double area1 = contourArea(contours2[0]);

        cout << "area0 =" << area0 << endl;
        cout << "area1 =" << area1 << endl;
    }
*/




    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]

    return 0;
}
