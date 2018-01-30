//
// Created by Niko on 1/11/2018.
//
#include "../../../header.h"
#include "AbstractionLayer_Histogram.h"

using namespace cv;

Mat HistogramComparer::readImages(int count)
{
    char name[100];
    Mat corr;
    Mat ref_gray;

    sprintf(name, PATH, count);
    Mat src = imread(name, 1);
   // namedWindow("UP1", WINDOW_AUTOSIZE);
    //imshow("UP1",src);
    if (!src.data)
    {
        cerr << "Problem loading image!!!" << endl;
        return src;
    }

    if(DISPLAY)imshow("src",src);

    Mat im_color;
    cvtColor(src, im_color, COLOR_BGR2HSV);
    return im_color;

}

bool AbstractionLayer_Histogram::PreProcessing(coor mySize,  const vector<Part*>* partArray){
   // HistogramComparer localImage;
    cout << "Abstraction 2 Preprocessing...  " << flush;
    const vector<Part*>& ref_partArray = *partArray;
    HistogramComparer analyse(mySize.row*mySize.col);
    Part buf;
    int iterator=0;
    if(!analyse.getImages())
    {
        cerr << "Error occured in getImages!" << endl;
        return false;
    }
    else // hier werden alle vier verschiedenen Rotationsarten 'gleichzeitig' abgespeichert
        //TODO rows and cols

        for(int i = 0; i < mySize.row*mySize.col; i++)
        {
            Mat src_img1 = analyse.readImages(i);
            Mat hsv_img1;
            /// Convert to HSV
            cvtColor(src_img1, hsv_img1, COLOR_BGR2HSV);

            /// Using 50 bins for hue and 60 for saturation
            int h_bins = 50;
            int s_bins = 60;
            int histSize[] = {h_bins, s_bins};

            // hue varies from 0 to 179, saturation from 0 to 255
            float h_ranges[] = {0, 180};
            float s_ranges[] = {0, 256};

            const float *ranges[] = {h_ranges, s_ranges};

            // Use the o-th and 1-st channels
            int channels[] = {0, 1};

            /// Histograms
            MatND hist_img1;

            /// Calculate the histograms for the HSV images
            calcHist(&hsv_img1, 1, channels, Mat(), hist_img1, 2, histSize, ranges, true, false);
            // normalize(hist_img1, hist_img1, 0, 1, NORM_MINMAX, -1, Mat());

            ref_partArray[iterator]->m_his.image=hsv_img1;
            iterator++;

        }


    InitialiseConstraintMatrixSize(mySize.col, mySize.row); //col row switched in this function

    cout << "Done!" << endl;
    return true;

}

bool AbstractionLayer_Histogram::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector){

    //evaluateQuality = evaluateProbabilaty
    for(int i = 0;i < qVector.size();i++)
    {
        if(PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_his.image))
        {
            qVector[i].first=1;
            continue;
        }
        if(constraintCoordinate.row<1 || constraintCoordinate.col < 1 )
            qVector[i].first=0.8;
        else
            qVector[i].first=0;
    }


}
bool AbstractionLayer_Histogram::PlaceOfPartGood(coor myCoor, Mat& myPart)
{
    //sets coordinates to correct position for layer


    if( myCoor.row == 1 && myCoor.col == 1){return true;}
    else if(myCoor.col == 1 && myCoor.row >1){
        if(CompareHistogram(m_constraintMatrix[myCoor.col][myCoor.row-1].image, myPart)){
            return true;
        }
        else return false;
    }

    else if( myCoor.row == 1 && myCoor.col >1){
        if(CompareHistogram(m_constraintMatrix[myCoor.col-1][myCoor.row].image, myPart)){
            return true;
        }
        else return false;
    }
    else if (myCoor.col > 1 && myCoor.row >1){
        if(     CompareHistogram(m_constraintMatrix[myCoor.col][myCoor.row-1].image, myPart) &&
                CompareHistogram(m_constraintMatrix[myCoor.col-1][myCoor.row].image, myPart)){

            return true;
        }
        else return false;
    }else return false;


}

bool AbstractionLayer_Histogram::CompareHistogram(Mat hist_img1,Mat hist_img2)
{
    // Correlation
    double Correlation = compareHist(hist_img1, hist_img2, CV_COMP_CORREL);

    if(Correlation > 0.95 ){

        return true;
    }
    else
        return false;
}

bool AbstractionLayer_Histogram::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_Histogram_Properties constraint)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].image=constraint.image;
    //m_constraintMatrix[constraintCoordinate.col+1][constraintCoordinate.row+1].m_connections=constraint.m_connections;
}

bool AbstractionLayer_Histogram::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    Mat dummy(1,1,0);
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].image = dummy;
}
bool HistogramComparer::getImages() {

    Mat src;

    for (int i = 0; i < nr_parts; i++) {
        if (DISPLAY) cout << "Bild " << i << endl;
        Mat img = readImages(i);

        if (!img.data) {
            cerr << "Error: No pic found!!" << endl;
            return false;
        }
        else return true;
    }
}