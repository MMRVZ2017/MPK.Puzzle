//
// Created by Raphael Maenle on 21/12/2017.
//

#include "../../header/solve.h"
#include "../../header/input.h"

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
                //TODO! add all other layerswith their rotaionvariance here
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
}
void Puzzle::setConstraints(coor setConstraints, Part* constraintPiece)
{
    //dp
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.clear();
    for(auto it:this->tmp_destructionArray)
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.emplace_back(it);

    //a1
    this->a1.SetConstraintOnPosition(setConstraints,constraintPiece->m_a1);
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

    cout<<"imageW "<<imageW <<endl<<"imageH " <<imageH<<endl<<endl;
    cout<<"partW "<<partWidth <<endl<<"partH " <<partHeight<<endl<<endl;
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
        cout << log.size() << endl;
        cout << log[0].PieceCollector.size() << endl;

            cout << it.PieceCollector[0].second->GetPartID() << endl;

            int imageNumber = it.PieceCollector[0].second->GetPartID();
            //cout<<"imageIndex: "<< imageNumber << endl;

            sprintf(name, PATH, imageNumber);
            Mat img = imread(name, 1);

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
            warpAffine(img,img,RotMatrix, img.size());
//            imshow("readImg",img); // you can comment with Ctrl + / did you know? :D
//            waitKey(0);

            auto ROI_X = int(round(it.myCoor.col*partWidth));
            auto ROI_Y = int(round(it.myCoor.row*partHeight));
//            cout<<"ROI X: "<< ROI_X<<endl;
//            cout<<"ROI Y: "<< ROI_Y<<endl;

            Rect ROI(ROI_X,ROI_Y , partWidth-separator, partHeight-separator); // j is the x coordinate not i!!
            Mat temp; resize(img,temp, Size(ROI.width, ROI.height));
            temp.copyTo(result(ROI));

//            imshow("result",result);
//            waitKey(0);


    }
    imshow("result",result);
    waitKey(0);
    return result;
}
