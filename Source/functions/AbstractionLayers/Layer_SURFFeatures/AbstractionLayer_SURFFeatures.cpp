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
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

bool AbstractionLayer_SURFFeatures::PreProcessing(coor mySize, const vector<Part*>* partArray)
{
    cout << "Abstraction 4 (Features) Preprocessing...  " << flush;
    InitialiseConstraintMatrixSize(mySize.col, mySize.row);
    if(!PreProcessingFullImg(mySize)) return false;
    if(!PreProcessingPieces(mySize, partArray)) return false;

    return true;
}

bool AbstractionLayer_SURFFeatures::EvaluateQuality (coor constraintCoordinate, qualityVector& qVector)
{
    // Calculate absolute difference between constraints and each piece and safe it
    for( int i = 0; i < qVector.size(); i++ )
    {
        float diff = abs(m_constraintMatrix[constraintCoordinate.row][constraintCoordinate.col].m_numberOfFeaturesDetected - qVector[i].second->m_a4.m_numberOfFeaturesDetected);
        qVector[i].first = 1 - diff;
        //cout << fixed << qVector[i].first << endl;
    }

    return true;
}

bool AbstractionLayer_SURFFeatures::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_SURFFeatures_Properties constraint)
{
    //TODO: Benötigen wir nicht unbedint.
    //TODO: Hier erhalten wir vom Dispatcher welches Teil an welche Position gesetzt wird und wir könnten hier die Features des Bilds in die m_constraintMatrix speichern
    return true;
}

bool AbstractionLayer_SURFFeatures::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    //TODO: Wie auch beim SetConstraint sollte uns das hier nicht wirklich interessieren.
    //TODO: Außer wir setzen etwas in die Contraintmatrix.
    //TODO: Dann ruft uns der Dispatcher beim Backtrack hier auf und wir müssten das jeweilige PuzzlePart hier wieder rauslöschen.
    return true;
}

bool AbstractionLayer_SURFFeatures::PreProcessingFullImg(coor mySize)
{
    std::vector< cv::Point2f > corners;                         // Variable to store corner-positions at

    // Load and resize image, so that number of parts in row and col fit in
    Mat image = imread(PATH_FULL_PUZZLE, IMREAD_GRAYSCALE);
    if (!image.data) {
        cerr << "Problem loading image of complete puzzle!" << endl;
        return false;
    }
    //cout << "PRE:  " << image.cols << " x " << image.rows << endl;
    resize(image, image, Size(int(ceil(double(image.cols)/mySize.col)*mySize.col), int(ceil(double(image.rows)/mySize.row)*mySize.row)));
    //cout << "POST: " << image.cols << " x " << image.rows << endl;

    // PARAMETERS (for description see top of file)
    int maxCorners = 10000;
    double qualityLevel = 0.01;
    double minDistance = .5;
    Mat mask;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    // Detect features - this is where the magic happens
    goodFeaturesToTrack( image, corners, maxCorners, qualityLevel, minDistance, mask, blockSize, useHarrisDetector, k );

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

    // Calculate percentage from 0 to 100% (normalized 0-1) with numberOfFeatures and safe it
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

    return true;
}

bool AbstractionLayer_SURFFeatures::PreProcessingPieces(coor mySize, const vector<Part*>* partArray)
{
    std::vector< cv::Point2f > corners;                         // Variable to store corner-positions at

    // PARAMETERS (for description see top of file)
    int maxCorners = 500;
    double qualityLevel = 0.05;
    double minDistance = .5;
    Mat mask;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    int minFeatures = maxCorners;
    int maxFeatures = 0;
    char name[100];

    for (unsigned imgID = 0; imgID < mySize.col*mySize.row; imgID++) {
        sprintf(name, PATH1, imgID);
        Mat src = imread(name, IMREAD_GRAYSCALE);
        if (!src.data) {
            cerr << "Problem loading image of puzzle piece!" << endl;
            return false;
        } else {
            goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, minDistance, mask, blockSize,
                                    useHarrisDetector, k);
            if (corners.size() < minFeatures) minFeatures = corners.size();
            if (corners.size() > maxFeatures) maxFeatures = corners.size();
            partArray->at(imgID)->m_a4.m_numberOfFeaturesDetected = corners.size();
            /*for( size_t i = 0; i < corners.size(); i++ ) {
                cv::circle( src, corners[i], 2, cv::Scalar( 255. ), -1 );
            }
            cv::namedWindow( "Output", CV_WINDOW_AUTOSIZE );
            cv::imshow( "Output", src );
            cout << count << " " << corners.size() << endl;
            cv::waitKey(0);*/
        }
    }

    // Calculate percentage from 0 to 100% (normalized 0-1) with numberOfFeatures and safe it
    for( unsigned i = 0; i < mySize.col*mySize.row; i++ )
    {
        partArray->at(i)->m_a4.m_numberOfFeaturesDetected = (partArray->at(i)->m_a4.m_numberOfFeaturesDetected - minFeatures) / (maxFeatures - minFeatures);
        //cout << fixed << partArray->at(i)->m_a4.m_numberOfFeaturesDetected << endl;
    }
    //cout << endl;
    return true;
}