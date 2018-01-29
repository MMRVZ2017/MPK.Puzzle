#include "AbstractionLayer_ColorMatching.h"
#include "../../../header.h"
#include <iostream>
#include <bitset>

bool AbstractionLayer_ColorMatching::PreProcessing(coor mySize, const vector<Part*>* partArray) {

    InitialiseConstraintMatrixSize(mySize.col, mySize.row); //col row switched in this function

    //**Get color for constraint matrix**//
    Mat puzzle = imread(PATH_FULL_PUZZLE, IMREAD_COLOR); // Read the picture of the puzzle
    if (puzzle.empty())                      // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    int width = puzzle.size().width;
    int height = puzzle.size().height;
    int X = width / mySize.col;
    int Y = height / mySize.row;

    for (int r = 0; r < mySize.row; r++) {
        for (int c = 0; c < mySize.col; c++) {
            Mat ExtPart = puzzle(CvRect(c*X, r*Y, X, Y));

            // crop image to ROI
            Mat ExtPartCropped = ExtPart(
                    Rect(ExtPart.size().width / 3, ExtPart.size().height / 3, ExtPart.size().width / 3,
                         ExtPart.size().height / 3));

            // Create a new matrix to hold the HSV image
            Mat HSVExtPart;
            // convert RGB image to HSV
            cvtColor(ExtPartCropped, HSVExtPart, CV_BGR2HSV);

            //namedWindow("ExtPartCropped", WINDOW_AUTOSIZE); // Create a window for display.
            //imshow("ExtPartCropped", ExtPartCropped);
            //waitKey(0); // Wait for a keystroke in the window

            vector<Mat> hsv_planes_ext;
            split(HSVExtPart, hsv_planes_ext);

            Scalar tempHue_ext = mean(hsv_planes_ext[0]);
            Scalar tempSaturation_ext = mean(hsv_planes_ext[1]);
            Scalar tempValue_ext = mean(hsv_planes_ext[2]);

            //cout << "Hue: " << tempHue_ext.val[0] << endl;
            //cout << "Saturation: " << tempSaturation_ext.val[0] << endl;
            //cout << "Value: " << tempValue_ext.val[0] << endl;

            m_constraintMatrix[c][r].m_centerColor.h = tempHue_ext.val[0];
            m_constraintMatrix[c][r].m_centerColor.s = tempSaturation_ext.val[0];
            m_constraintMatrix[c][r].m_centerColor.v = tempSaturation_ext.val[0];

            //**********************************************************//
        }
    }
            //**Get color of all parts**//

    const vector<Part *> &ref_partArray = *partArray;
    int iterator = 0;

    for(int count =0;count< mySize.row*mySize.col*4;count++)// multiplied by 4 for all rotations
    {
        Scalar tempHue, tempSaturation, tempValue;

        if(count%4) {
            char name[100];

            sprintf(name, PATH, count);
            Mat Part = imread(name, 1);
            if (!Part.data) {
                cerr << "Could not open or find the image" << endl;
                return -1;
            }

            Mat PartCropped = Part(Rect(Part.size().width / 3, Part.size().height / 3, Part.size().width / 3,
                                        Part.size().height / 3));

            //namedWindow("PartCropped", WINDOW_AUTOSIZE); // Create a window for display.
            //imshow("PartCropped", PartCropped);
            //waitKey(0); // Wait for a keystroke in the window

            // Create a new matrix to hold the HSV image
            Mat HSVPart;

            // convert RGB image to HSV
            cvtColor(PartCropped, HSVPart, CV_BGR2HSV);

            vector<Mat> hsv_planes;
            split(HSVPart, hsv_planes);

            tempHue = mean(hsv_planes[0]);
            tempSaturation = mean(hsv_planes[1]);
            tempValue = mean(hsv_planes[2]);

            //cout << "Hue: " << tempHue.val[0] << endl;
            //cout << "Saturation: " << tempSaturation.val[0] << endl;
            //cout << "Value: " << tempValue.val[0] << endl;
        }

        ref_partArray[iterator]->m_acm.m_centerColor.h = tempHue.val[0];
        ref_partArray[iterator]->m_acm.m_centerColor.s = tempSaturation.val[1];
        ref_partArray[iterator]->m_acm.m_centerColor.v = tempValue.val[2];

        iterator ++;
    }

    return true;
}

bool AbstractionLayer_ColorMatching::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i = 0;i<qVector.size();i++)
    {
        float value = PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_acm.m_partColor);
        qVector[i].first = value;
    }
}

float AbstractionLayer_ColorMatching::PlaceOfPartGood(coor myCoor, HSV myPart)
{
    //Hue max 360°
    if(m_constraintMatrix[myCoor.col][myCoor.row].m_partColor.h >= 180)
    {
        return 1-abs((m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h-myPart.h)
                     /m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h);
    }
    else if(m_constraintMatrix[myCoor.col][myCoor.row].m_partColor.h < 180)
    {
        return 1-((myPart.h-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h)
                  /(360-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h));
    }
}