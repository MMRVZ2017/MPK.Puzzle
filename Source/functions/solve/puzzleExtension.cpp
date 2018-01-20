#include "../../header/solve.h"
#include "../../header/input.h"

typedef std::vector<std::vector<Point>> Contour_t;
typedef std::vector<Vec4i> Hierarchy_t; //hierarchy object is needed for drawContours apparently
Mat crop2ContourInv(const Mat & img);
Contour_t getLongestContour(Mat bw);
int getLongestContourIndex(Contour_t contours);

void Puzzle::printPuzzle()
{
    cout << "a1: " << endl;
    a1.printConstraintMatrix();

}

void Puzzle::printBox()
{
    int i=0;
    for(auto it:myBox)
    {
        cout << "Part " << i++ << ":" << endl;
        it.print(); cout << endl;

    }
}


//puts a puzzlepiece back into its box
void Puzzle::putIntoBox()
{
    Part tmpPart;
    int id = 0;
    for(int i=0;i<this->getSizeAsCoor().col;i++)
    {
        for(int j=0;j<this->getSizeAsCoor().row;j++)
        {

            tmpPart.m_a1=this->a1.m_constraintMatrix[i+1][j+1];
            //sets part id
            tmpPart.SetPartID(id++);
            // adds all 4 rotations to Box
            for(int rotations=0;rotations<4;rotations++)
            {
                tmpPart.m_a1.shift(1);
                //TODO! add all other layer with their rotaionvariance into "tmpPart"
                //if it piece is roation invariant no need to do anything
                myBox.emplace_back(tmpPart);

            }

        }
    }
}

//shuffles the existing box in Puzzle
void Puzzle::shuffle()
{
    random_shuffle(myBox.begin(),myBox.end());
}

//deletes all constraints from all abstractionlayers
void Puzzle::removeConstrains(coor removeCoordinates)
{
    this->a1.RemoveConstraintOnPosition(removeCoordinates);
    //TODO!! Add other layer remove here
}
void Puzzle::setConstraints(coor setConstraints, Part* constraintPiece)
{
    //dp
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.clear();
    for(auto it:this->tmp_destructionArray)
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.emplace_back(it);

    //a1
    this->a1.SetConstraintOnPosition(setConstraints,constraintPiece->m_a1);

    //TODO!! Add other layer remove here
}

int Puzzle::removeSimilar(qualityVector& qVector, Part& myPart)
{
    //a1
    uint8_t tmpConnections=myPart.m_a1.getConnections();
    a1.RemoveSimilar(qVector,tmpConnections);
}

void Puzzle::createRandomPuzzle()
{
    a1.CreateRandomPuzzle();
}

void Puzzle::createp_box()
{
    for(int i=0;i<cols*rows*4;i++)
    p_myBox.push_back(&myBox[i]);
}

//creates a box of puzzlepieces with nothing other than puzzle piece id and correct nr of pieces
void Puzzle::createBox(){
    for(int i=0;i<cols*rows;i++)
    {
        Part temp;
        temp.SetPartID(i);
        for(uint8_t j=0;j<4;j++)
        {
            temp.SetNumOfRotations(j);
            myBox.push_back(temp);
        }
    }
}

bool Puzzle::allSet() {
    for(auto it:myBox)
        if(!it.set)
            return false;
    return true;
}

void Puzzle::clearMat()
{
    for(unsigned int i=0;i<cols;i++)
    {
        for(unsigned int j=0;j<rows;j++)
        {
            this->removeConstrains({i,j});
        }
    }
}

Mat Puzzle::readImage(int fileIndex, const char* inputDir){

    char indexstr[12];
    sprintf(indexstr,"%d.jpg",fileIndex);
    char * inputstr = (char *) malloc(1 + strlen(inputDir)+ strlen(indexstr) );
    strcpy(inputstr, inputDir);
    strcat(inputstr,indexstr);
    //cout<<inputstr<<endl;
    Mat source = imread(inputstr,1);
    return source;
}
Mat Puzzle::resultImage( vector<LogEntry>& log){
    int Y_size = 1200; // chose this to fit your monitor!
    int separator = 1;
    int partHeight = 90;
    int partWidth;
    auto imageH =  int(round(partHeight* cols));

    if(imageH > Y_size){
        imageH = Y_size;
    }
    partHeight = int(round(imageH /  cols));
    partWidth= partHeight;
    int imageW = int(round( partWidth*rows));

    int temp = imageW;
    imageW = imageH;
    imageH = temp;

    Mat result(imageH,imageW,CV_8UC3);

    char name[100];
    for (auto it:log)
    {
        if (it.myCoor.col == 12 && it.myCoor.row == 0)
        {
            ;
            // imshow("result",result);
            // waitKey(0);
        }

        cout << it.PieceCollector[0].second->GetPartID() << endl;

        int imageNumber = it.PieceCollector[0].second->GetPartID();
        //cout<<"imageIndex: "<< imageNumber << endl;

        sprintf(name, PATH, imageNumber);
        Mat img = imread(name, 1);

        copyMakeBorder(img,img,200,200,200,200,BORDER_CONSTANT,Scalar(255,255,255));
        Mat invert = Mat::ones(img.size(), CV_8UC3); // invert for rotation to work correctly
        bitwise_not ( img, invert );
        if (it.myCoor.col == 12 && it.myCoor.row == 0)
        {
            //imshow("img",img);
            //waitKey(0);
            ;
        }


        int angle = ((int)it.PieceCollector[0].second->GetNumOfRotations())*-90;
        Point2f center;
        center.x = img.cols/2;
        center.y = img.rows/2;
        Mat RotMatrix = getRotationMatrix2D(center,angle,1);
        warpAffine(invert,invert,RotMatrix, invert.size());
        bitwise_not(invert,img);
        Mat cropped = crop2ContourInv(img);
        auto ROI_X = int(round(it.myCoor.col*partWidth));
        auto ROI_Y = int(round(it.myCoor.row*partHeight));
//            cout<<"ROI X: "<< ROI_X<<endl;
//            cout<<"ROI Y: "<< ROI_Y<<endl;

        Rect ROI(ROI_X,ROI_Y , partWidth-separator, partHeight-separator); // j is the x coordinate not i!!
        Mat temp(Scalar(255,255,255));
        resize(cropped,temp, Size(ROI.width, ROI.height));
        temp.copyTo(result(ROI));
    }
    imshow("result",result);
    waitKey(0);
    return result;
}

Mat crop2ContourInv(const Mat & img){ // for white background images
    Mat grey;
    cvtColor(img, grey,CV_BGR2GRAY);
    Contour_t c; Hierarchy_t h;
    threshold(grey,grey,200,255,THRESH_BINARY_INV);
    findContours(grey,c,h,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    int longest = getLongestContourIndex(c);
    Rect croppingRect = boundingRect(c[longest]);
    Mat cropped = img(croppingRect);
    // imshow("cropped", cropped);
    return cropped;
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