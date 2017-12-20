#include "../../header.h"



//use this in second abstraction layer maybe ey?
void createRandomAbstraction2()
{
    //get a picture
    cv::Mat PuzzlePicture = cv::imread("../../Codicil/Images/Balloons.jpg");
    if(! PuzzlePicture.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return;
    }
    //split a picture into mxn pieces
    cv::Size smallSize ((int)(PuzzlePicture.cols/getCols()) ,(int)(PuzzlePicture.rows/getRows()));
    // get the image data

    vector<cv::Mat> smallChannels;
    unsigned int i=0,j=0;
    for  ( int y=0; y<=PuzzlePicture.rows-smallSize.height; y += smallSize.height )
    {

        for  ( int x=0 ; x<=PuzzlePicture.cols-smallSize.width; x += smallSize.width )
        {
            //split into different ROIs
            cv::Rect rect =   cv::Rect (x ,y , smallSize.width, smallSize.height );
            cv::Mat tmpSmallPicture = cv::Mat(PuzzlePicture,rect);
            cv::split(tmpSmallPicture, smallChannels);

            //save color into individual PuzzlePieces in Matrix
            PuzzlePiece tmpPiece = getPiece(j,i);
            tmpPiece.r = cv::mean(smallChannels[0]).operator[](0);
            tmpPiece.g = cv::mean(smallChannels[1]).operator[](0);
            tmpPiece.b = cv::mean(smallChannels[2]).operator[](0);
            setPiece(coor(j,i),tmpPiece);
            j++;
        }
        i++;
        j=0;
    }
}

//shuffles around a box, randomizing pieces and orientation
vector<Part> randomBox::shuffle()
{
    random_shuffle(Box.begin(),Box.end());
    for (auto &i:Box)
    {
        i.shift(rand()%4);
        i.resetShift();
    }

    numerateBox(Box);
    return Box;
}