//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include "../../../header.h"

#include <iostream>
#include <bitset>

bool AbstractionLayer_1::PreProcessing(coor mySize,  const vector<Part*>* partArray)
{
    cout << "Abstraction 1 Preprocessing...  " << flush;
    const vector<Part*>& ref_partArray = *partArray;
    analyseParts analyse(mySize.row*mySize.col);
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
            unsigned char poempel = analyse.getTabs(i);;
            for (int j=0;j<4;j++)
            {
                ref_partArray[iterator]->m_a1.m_connections=poempel;
                shift(poempel,1);
                iterator++;
            }
        }

    //Zugriff auf den vector mit den einzelnen teilen: part[0].getConnenctions() entspricht pömpel von bild 0.jpg und liefert ein unsigned char, poempl Belegung wie ausgemacht



    InitialiseConstraintMatrixSize(mySize.col+2, mySize.row+2); //col row switched in this function
    setEdgeZero();

    cout << "Done!" << endl;
    return true;
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool AbstractionLayer_1::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i = 0;i<qVector.size();i++)
    {
        if(PlaceOfPartGood(constraintCoordinate, qVector[i].second->m_a1.m_connections))
        {
            qVector[i].first=1;

            continue;
        }
        qVector[i].first=0;
    }
}

bool AbstractionLayer_1::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{
    m_constraintMatrix[constraintCoordinate.col+1][constraintCoordinate.row+1].m_connections=constraint.m_connections;
}

bool AbstractionLayer_1::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    m_constraintMatrix[constraintCoordinate.col+1][constraintCoordinate.row+1].m_connections=0b11111111;
}

void AbstractionLayer_1::CreateRandomPuzzle()
{
    std::minstd_rand simple_rand;
    simple_rand.seed(u_int(std::time(nullptr)));

    for(int col = 0; col < m_constraintMatrix.size()-2; col++){
        for(int row = 0; row < (m_constraintMatrix[col].size() - 2);)
        {
            //create random piece
            uint8_t tempPiece = 0b00000000;
            if(simple_rand()%2)
                tempPiece|=0b01000000;
            else
                tempPiece|=0b10000000;

            if(simple_rand()%2)
                tempPiece|=0b00010000;
            else
                tempPiece|=0b00100000;

            if(simple_rand()%2)
                tempPiece|=0b00000100;
            else
                tempPiece|=0b00001000;

            if(simple_rand()%2)
                tempPiece|=0b00000001;
            else
                tempPiece|=0b00000010;

            //set edges and corners to 00
            if(row == 0)
                tempPiece and_eq (uint8_t)0b00111111;
            if(row == (m_constraintMatrix[col].size() - 3))
                tempPiece and_eq (uint8_t)0b11110011;
            if(col == 0)
                tempPiece and_eq (uint8_t)0b11111100;
            if(col == (m_constraintMatrix.size() - 3))
                tempPiece and_eq (uint8_t)0b11001111;

            //set piece if piece good
            if(PlaceOfPartGood(coor((unsigned int)col,(unsigned int)row),tempPiece))
            {
                m_constraintMatrix[col+1][row+1].m_connections = tempPiece;
                row++;
            }
        }
    }

}



//puts all pieces of the current constraint matrix into a puzzlebox
qualityVector AbstractionLayer_1::returnInBox(vector<Part>& PuzzleBox)
{
    if(!(PuzzleBox.size()))
        for(int i = 0; i< (m_constraintMatrix.size()-2)*(m_constraintMatrix[0].size()-2);i++)
            PuzzleBox.emplace_back(Part());

    int i=0;
    for(int col=1;col<m_constraintMatrix.size()-1;col++)
        for(int row=1;row<m_constraintMatrix[col].size()-1;row++)
            PuzzleBox[i++].m_a1.m_connections=m_constraintMatrix[col][row].m_connections;

}

void AbstractionLayer_1::printConstraintMatrix() {
    for (int j=0;j<m_constraintMatrix[0].size();j++) {
        for (int i=0;i<m_constraintMatrix.size();i++)

            std::cout << std::bitset<8>(m_constraintMatrix[i][j].m_connections) << " ";
        std::cout << std::endl;
    }
    cout.flush();
}
void AbstractionLayer_1::setEdgeZero()
{
    for(int row = 0; row < m_constraintMatrix.size(); row++)
        for(int col = 0; col < m_constraintMatrix[row].size(); col++)
            if(col == 0 || col == (m_constraintMatrix[row].size() - 1) || row == 0 || row == (m_constraintMatrix.size() - 1))
                m_constraintMatrix[row][col].m_connections=0b00000000;
}

//checks if the myPart in its current orientation is legal in position m, n
bool AbstractionLayer_1::PlaceOfPartGood(coor myCoor, uint8_t& myPart)
{
    //sets coordinates to correct position for layer
    myCoor.row++;
    myCoor.col++;

    uint8_t negativePart=0b00000000;

    negativePart or_eq (m_constraintMatrix[myCoor.col+1][myCoor.row].m_connections & 0b00000011);
    negativePart or_eq (m_constraintMatrix[myCoor.col][myCoor.row-1].m_connections & 0b00001100);
    negativePart or_eq (m_constraintMatrix[myCoor.col-1][myCoor.row].m_connections & 0b00110000);
    negativePart or_eq (m_constraintMatrix[myCoor.col][myCoor.row+1].m_connections & 0b11000000);
    shift(negativePart,2);
    if  (
            (    ((((negativePart & 0b11000000) ^ (myPart & 0b11000000))  != 0b00000000) && (((myPart & 0b11000000) != 0b00000000) && (negativePart & 0b11000000) != 0b00000000))
                 || ((((negativePart & 0b11000000) == 0b11000000) || ((myPart &  0b11000000) == 0b11000000))  && (((myPart & 0b11000000) != 0b00000000) && (negativePart & 0b11000000) != 0b00000000))
                 ||  (((negativePart & 0b11000000) == 0b00000000) && ((myPart &  0b11000000) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00110000) ^ (myPart & 0b00110000))  != 0b00000000) && (((myPart & 0b00110000) != 0b00000000) && (negativePart & 0b00110000) != 0b00000000))
                 || ((((negativePart & 0b00110000) == 0b00110000) || ((myPart &  0b00110000) == 0b00110000))  && (((myPart & 0b00110000) != 0b00000000) && (negativePart & 0b00110000) != 0b00000000))
                 ||  (((negativePart & 0b00110000) == 0b00000000) && ((myPart &  0b00110000) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00001100) ^  (myPart & 0b00001100))  != 0b00000000) && (((myPart & 0b00001100) != 0b00000000) && (negativePart & 0b00001100) != 0b00000000))
                 || ((((negativePart & 0b00001100) == 0b00001100) || ((myPart &  0b00001100) == 0b00001100))  && (((myPart & 0b00001100) != 0b00000000) && (negativePart & 0b00001100) != 0b00000000))
                 ||  (((negativePart & 0b00001100) == 0b00000000) && ((myPart &  0b00001100) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00000011) ^  (myPart & 0b00000011))  != 0b00000000)&& (((myPart & 0b00000011) != 0b00000000) && (negativePart & 0b00000011) != 0b00000000))
                 || ((((negativePart & 0b00000011) == 0b00000011) || ((myPart &  0b00000011) == 0b00000011))  && (((myPart & 0b00000011) != 0b00000000) && (negativePart & 0b00000011) != 0b00000000))
                 ||  (((negativePart & 0b00000011) == 0b00000000) && ((myPart &  0b00000011) == 0b00000000))  )
            )
        return true;
    return false;

}

void AbstractionLayer_1::shift(uint8_t& Part, int shifts)
{
    Part = Part >> (shifts*2) | Part << sizeof(uint8_t)*8 - (shifts*2);
}

void AbstractionLayer_1_Properties::shift(int shifts)
{
    this->m_connections = this->m_connections >> (shifts*2) | this->m_connections << sizeof(uint8_t)*8 - (shifts*2);
}

void AbstractionLayer_1_Properties::print()
{
    std::cout << std::bitset<8>(this->m_connections);
}


Mat analyseParts::makeBorder(Mat& im_bw) {
    Mat im_part;

    Scalar value = (0,0,0);
    Point center = findCenter(im_bw);
    copyMakeBorder(im_bw, im_part, abs((IMG_SIZE/2)-center.y),abs((IMG_SIZE/2)-(im_bw.rows-center.y)), abs((IMG_SIZE/2)-center.x),abs( (IMG_SIZE/2)-(im_bw.cols-center.x )), BORDER_CONSTANT, value);

    return im_part;
}

Mat analyseParts::readImages(int count)
{
    char name[100];
    Mat corr;
    Mat ref_gray;

    sprintf(name, PATH, count);
    Mat src = imread(name, 1);
    if (!src.data)
    {
        cerr << "Problem loading image!!!" << endl;
        return src;
    }

    if(DISPLAY)imshow("src",src);

    Mat im_gray, im_bw;
    cvtColor(src, im_gray, CV_RGB2GRAY);
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

    disa = lengthTwoPoints(two, three);
    disb = lengthTwoPoints(one, three);
    disc = lengthTwoPoints(one, two);

    angle = acos( ((disa*disa) + (disc *disc) - (disb * disb)) / (2 * disa * disc) );
    angle = angle * 180 / CV_PI;
    return angle;
}

bool analyseParts::getImages(){
    Details mask;
    Mat src;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> corners;

    vector<Mat> puzzleimages;
    vector<vector<Point> > contours1;
    Mat mask_gray;

    for (int i = 0; i < nr_parts; i++) {
        if(DISPLAY) cout << "Bild " << i << endl;
        Mat im_bw = readImages(i);

        if(!im_bw.data)
        {
            cerr << "Error: No pic found!!" << endl;
            return false;
        }

        Mat dst = morphDilateErode(im_bw);
        contours1 = findingContours(dst);
        mask_gray = polyApprox(contours1);
        mask.setImage(mask_gray);
        mask.setCenter(findCenter(mask_gray));
        findContours(mask_gray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        mask.setContour(contours);
        mask.setHierarchy(hierarchy);
        corners = findCorners(contours[0],mask.getCenter());
        if((corners.empty()) || (corners.size() < 4))
        {
            cerr << "Error occured in findCorners" << endl;
            return false;
        }
        mask.setCorners(corners);
        mask.setTabs(analyseContour(corners,contours[0]));
        masks.push_back(mask);
        destroyAllWindows();
    }

    return true;
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

    if((buf0.empty()) || (buf1.empty()) || (buf2.empty()) || (buf3.empty()))
    {
        // return an empty vector if not all corners were found!
        return vector<Point>();
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
    if(DISPLAY) imshow("draw",drawing);
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
        if(contour[i] == corners[0])
            corner0 = i;
        if(contour[i] == corners[1])
            corner1 = i;
        if(contour[i] == corners[2])
            corner2 = i;
        if(contour[i] == corners[3])
            corner3 = i;
    }
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

    if(DISPLAY)imshow("corners",drawing);
    if(DISPLAY)waitKey(0);
    return tabs;
}

