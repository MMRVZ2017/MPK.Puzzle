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
            Mat ExtPart = puzzle(CvRect(c * X, r * Y, X, Y));

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
            m_constraintMatrix[c][r].m_centerColor.v = tempValue_ext.val[0];

            //**********************************************************//
        }
    }
    //**Get color of all parts**//

    const vector<Part *> &ref_partArray = *partArray;
   for(int count =0;count< mySize.row*mySize.col;count++)// multiplied by 4 for all rotations
    {
        Scalar tempHue, tempSaturation, tempValue;


        char name[100];

        sprintf(name, PATH, count);
        Mat Part = imread(name, 1);
        if (!Part.data) {
            cerr << "Could not open or find the image" << endl;
            return -1;
        }
        Scalar value(255,  255, 255 );
        copyMakeBorder( Part, Part, 3, 3, 3, 3, BORDER_CONSTANT, value );

        Mat PartCropped = Part;//(Rect(Part.size().width / 2, Part.size().height / 2, Part.size().width / 2, Part.size().height / 2));
        Mat gray;

        //Prepare the image for findContours
        cvtColor(PartCropped, gray, CV_BGR2GRAY);
        threshold(gray, gray, 250, 255, CV_THRESH_BINARY_INV);


        //namedWindow("PartCropped", WINDOW_AUTOSIZE); // Create a window for display.
        //imshow("PartCropped", gray);
        //waitKey(0); // Wait for a keystroke in the window

        //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
        std::vector<std::vector<cv::Point> > contours;
        //cv::Mat contourOutput = gray.clone();
        cv::findContours( gray.clone(), contours, CV_RETR_LIST , CV_CHAIN_APPROX_SIMPLE );

        //Draw the contours
        Scalar colors[3];
        colors[0] = cv::Scalar(255, 0, 0);
        colors[1] = cv::Scalar(0, 255, 0);
        colors[2] = cv::Scalar(0, 0, 255);

        double length;
        int max=0;
        int maxidx=0;
        for (size_t idx = 0; idx < contours.size(); idx++) {
            length=arcLength(contours[idx], true);
            if(length>max && (contours[idx].max_size()>10)) {
                max = length;
                maxidx = idx;
            }
        }
        //cv::drawContours(PartCropped, contours, maxidx, colors[maxidx % 3],3);

        //namedWindow("PartCropped", WINDOW_AUTOSIZE); // Create a window for display.
        //imshow("PartCropped", PartCropped);
        //waitKey(0); // Wait for a keystroke in the window

        // Create a new matrix to hold the HSV image
        Mat HSVPart;

        // convert RGB image to HSV
        cvtColor(PartCropped, HSVPart, CV_BGR2HSV);

        vector<Mat> hsv_planes;
        split(HSVPart, hsv_planes);

        Rect roi = boundingRect(contours[maxidx]);

        Mat resImageMeanRGB = PartCropped.clone();
        Scalar mean_RGB = mean(PartCropped(roi), gray(roi));
        //drawContours( resImageMeanRGB, contours, maxidx,  mean_RGB, FILLED );
        //imshow( "MeanRGB", resImageMeanRGB );

        tempHue = mean(hsv_planes[0](roi), gray(roi));
        tempSaturation = mean(hsv_planes[1](roi), gray(roi));
        tempValue = mean(hsv_planes[2](roi), gray(roi));

        /*Mat restempHue = PartCropped.clone();
        drawContours( restempHue, contours, maxidx,  tempHue, FILLED );
        imshow( "MeanHue", restempHue );
        Mat restempSaturation = PartCropped.clone();
        drawContours( restempSaturation, contours, maxidx,  tempSaturation, FILLED );
        imshow( "MeanSat", restempSaturation );
        Mat restempValue = PartCropped.clone();
        drawContours( restempValue, contours, maxidx,  tempValue, FILLED );
        imshow( "MeanValue", restempValue );

        cout << "RGB: " << mean_RGB << endl;
        cout << "Hue: " << tempHue.val[0] << endl;
        cout << "Saturation: " << tempSaturation.val[0] << endl;
        cout << "Value: " << tempValue.val[0] << endl;

        imshow("Input Image", PartCropped);
        cvMoveWindow("Input Image", 0, 0);
        waitKey(0);*/

        for(int rotate=0;rotate<4;rotate++)
        {
            ref_partArray[count*4+rotate]->m_acm.m_centerColor.h = tempHue.val[0];
            ref_partArray[count*4+rotate]->m_acm.m_centerColor.s = tempSaturation.val[0];
            ref_partArray[count*4+rotate]->m_acm.m_centerColor.v = tempValue.val[0];
        }
    }

    return true;
}
bool AbstractionLayer_ColorMatching::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i = 0;i<qVector.size();i++)
    {
       //float value1 = PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_acm.m_centerColor);

        if(constraintCoordinate.col==5 && constraintCoordinate.row ==27)
        {
            ;
        }

        float value1 = (float)(1-(abs(m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].m_centerColor.h-qVector[i].second->m_acm.m_centerColor.h))/180);
        float value2 = (float)(1-(abs(m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].m_centerColor.s-qVector[i].second->m_acm.m_centerColor.s))/255);
        float value3 = (float)(1-(abs(m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].m_centerColor.v-qVector[i].second->m_acm.m_centerColor.v))/255);


        qVector[i].first = (value1*4+value2*2+value3*1)/6;
    }
}

float AbstractionLayer_ColorMatching::PlaceOfPartGood(coor myCoor, HSV myPart)
{
    //Hue max 180°
    if(m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h >= myPart.h)
    {
        if((m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h-myPart.h) > 90)
        {
            return 1-((myPart.h+180-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h)
                      /90);
        } else
        {
            return 1-((m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h-myPart.h)
                      /90);
        }
    }
    else if(m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h < myPart.h)
    {
        if((myPart.h-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h) > 90)
        {
            return 1-abs((myPart.h-180-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h)
                         /90);
        } else
        {
            return 1-((myPart.h-m_constraintMatrix[myCoor.col][myCoor.row].m_centerColor.h)
                      /90);
        }
    }
}
