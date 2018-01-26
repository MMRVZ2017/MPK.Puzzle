#include "AbstractionLayer_SURFFeatures.h"

// Parameters for algorithm:
// maxCorners – The maximum number of corners to return. If there are more corners than that will be found, the strongest of them will be returned
// qualityLevel – Characterizes the minimal accepted quality of image corners;
// minDistance – The minimum possible Euclidean distance between the returned corners
// mask – The optional region of interest. It will specify the region in which the corners are detected
// blockSize – Size of the averaging block for computing derivative covariation
// useHarrisDetector – Indicates, whether to use operator or cornerMinEigenVal()
// k – Free parameter of Harris detector

#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#ifdef _WIN32
#define PATH_FULL_PUZZLE "..\\..\\..\\puzzle_img\\puzzle1.jpg"
#elif defined __unix__
#define PATH_FULL_PUZZLE "..//..//..//puzzle_img//puzzle1.jpg"
#elif defined __APPLE__
    #define PATH_FULL_PUZZLE "..//..//..//puzzle_img//puzzle1.jpg"
#endif

using namespace cv;
using namespace std;

bool AbstractionLayer_SURFFeatures::PreProcessing(coor mySize, const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(mySize.col, mySize.row);
    std::vector< cv::Point2f > corners;                         // Variable to store corner-positions at

    // -- Complete puzzle image processing --
    // Load and resize image, so that number of parts in row and col fit in
    cv::Mat image = cv::imread(PATH_FULL_PUZZLE, IMREAD_GRAYSCALE);
    //cout << "PRE:  " << image.cols << " x " << image.rows << endl;
    cv::resize(image, image, Size(int(ceil(double(image.cols)/mySize.col)*mySize.row), int(ceil(double(image.rows)/mySize.row)*mySize.row)));
    //cout << "POST: " << image.cols << " x " << image.rows << endl;

    // PARAMETERS (for description see top of file)
    int maxCorners = 10000;
    double qualityLevel = 0.01;
    double minDistance = .5;
    cv::Mat mask;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    // Detect features - this is where the magic happens
    cv::goodFeaturesToTrack( image, corners, maxCorners, qualityLevel, minDistance, mask, blockSize, useHarrisDetector, k );

    // Empty the matrix
    for( int j = 0; j < mySize.row ; j++ )
    { for( int i = 0; i < mySize.col; i++ )
        {
            m_constraintMatrix[j][i].m_numberOfFeaturesDetected = 0;
        }
    }

    int pieceColSize = image.cols/mySize.col;
    int pieceRowSize = image.rows/mySize.row;
    // Calculate number of features for each piece-position
    for( int i = 0; i < corners.size(); i++ )   // For all found features
    {
        // Increment number of found pieces
        m_constraintMatrix[int(corners[i].y/pieceRowSize)][int(corners[i].x/pieceColSize)].m_numberOfFeaturesDetected++;
    }

    // Get minimal and maximal number of features -> TODO: Do in first loop to safe time?
    int minFeatures = int(m_constraintMatrix[0][0].m_numberOfFeaturesDetected);
    int maxFeatures = int(m_constraintMatrix[0][0].m_numberOfFeaturesDetected);
    for( int j = 0; j < mySize.row ; j++ )
    {
        for( int i = 0; i < mySize.col; i++ )
        {
            if(m_constraintMatrix[j][i].m_numberOfFeaturesDetected < minFeatures) minFeatures = int(m_constraintMatrix[j][i].m_numberOfFeaturesDetected);
            if(m_constraintMatrix[j][i].m_numberOfFeaturesDetected > maxFeatures) maxFeatures = int(m_constraintMatrix[j][i].m_numberOfFeaturesDetected);
        }
    }

    // Calculate percentage from 0 to 100% with numberOfFeatures and safe it
    for( int j = 0; j < mySize.row ; j++ )
    {
        for( int i = 0; i < mySize.col; i++ )
        {
            m_constraintMatrix[j][i].m_numberOfFeaturesDetected = (m_constraintMatrix[j][i].m_numberOfFeaturesDetected - minFeatures) / (maxFeatures - minFeatures);
            //cout << fixed << m_constraintMatrix[i][j].m_numberOfFeaturesDetected << " ";
        }
        //cout << endl;
    }

    // DEBUG - Display image
    /*for( size_t i = 0; i < corners.size(); i++ )
    {
        cv::circle( image, corners[i], 2, cv::Scalar( 255. ), -1 );
    }
    cv::namedWindow( "Output", CV_WINDOW_AUTOSIZE );
    cv::imshow( "Output", image );

    cv::waitKey(0);*/

    //TODO: Alle Bilder mit OpenCV öffnen und deren erkannten Features in SURFFeature_Properties der Part-Klasse speichern
    // Speichert die erkannten Features des jeweiligen Bilds im partArray an der Stelle (->at(xxx))
    partArray->at(0)->m_a4.m_numberOfFeaturesDetected = 40;
}

bool AbstractionLayer_SURFFeatures::EvaluateQuality (coor constraintCoordinate, qualityVector& qVector)
{
    //TODO: Vergleichen, welche der in qualityVector erhaltenen ähnlich viele Features besitzen, wie an der jeweiligen constraintCoordinate in der m_constraintMatrix gespeichert sind
}

bool AbstractionLayer_SURFFeatures::SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_SURFFeatures_Properties constraint)
{
    //TODO: Benötigen wir nicht unbedint.
    //TODO: Hier erhalten wir vom Dispatcher welches Teil an welche Position gesetzt wird und wir könnten hier die Features des Bilds in die m_constraintMatrix speichern
}

bool AbstractionLayer_SURFFeatures::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    //TODO: Wie auch beim SetConstraint sollte uns das hier nicht wirklich interessieren.
    //TODO: Außer wir setzen etwas in die Contraintmatrix.
    //TODO: Dann ruft uns der Dispatcher beim Backtrack hier auf und wir müssten das jeweilige PuzzlePart hier wieder rauslöschen.
}