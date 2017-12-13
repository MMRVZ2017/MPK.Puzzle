#include "../../header.h"

//shifts puzzle piece one to the right
void PuzzlePiece::shift(unsigned int moves)
{
    shifts = (shifts+moves)%4;
    setConnections(((getConnections() >> (moves*2)) | (getConnections() << sizeof(unsigned char)*8 - (moves*2))));     
}


//creates random centerpiece
void PuzzlePiece::randomCenterPiece()
{
    setConnections(0b00000000);

    if(rand()%2)
        setConnections(getConnections() | 0b01000000);
    else
        setConnections(getConnections() | 0b10000000);

    if(rand()%2)
        setConnections(getConnections() | 0b00010000);
    else
        setConnections(getConnections() | 0b00100000);

    if(rand()%2)
        setConnections(getConnections() | 0b00000100);
    else
        setConnections(getConnections() | 0b00001000);

    if(rand()%2)
        setConnections(getConnections() | 0b00000001);
    else
        setConnections(getConnections() | 0b00000010);
}

//tests the myPart in all 4 rotations at position m, n
bool Puzzle::testRotationPiece(coor myCoor, PuzzlePiece& myPart, int nrOfRotations)
{
    for(int rotation=0; rotation < nrOfRotations; rotation++)
    {
        //coor myCoor(m,n);
        if(PlaceOfPartGood(myCoor,myPart))
            return true;
        //cout << "was rotated in testRotationPiece" << endl;
        myPart.shift(1);
    }

        //cout << "Was a bad part" << endl;
    return false;
}

//insterts piece at position in box according to boxidentifier and removes piece from puzzle
//this returns the position after!! the puzzle piece was put back in! not the boxidentifier of the piece. look that up in other function.
unsigned int Puzzle::putBackIntoBox(coor myCoor, vector<PuzzlePiece>& myBox)
{
#ifdef debug
cout << "putting back" << endl;
cout << "Old Box: "; 
printBox(myBox);
cout << endl;
#endif
    for(int i = 0; i < myBox.size();i++)
    {
        if(myBox[i].getBoxIdentifier()>getPiece(myCoor.m,myCoor.n).getBoxIdentifier())
            {
                myBox.insert(myBox.begin()+i,getPiece(myCoor.m,myCoor.n));
                removePiece(myCoor);
                return i+1;
            }
    }
    //using push back, if the element was the last element in the vector chain
    myBox.push_back(getPiece(myCoor.m,myCoor.n));
    removePiece(myCoor);
    return myBox.size();
}

//checks if the myPart in its current orientation is legal in position m, n
bool Puzzle::PlaceOfPartGood(coor myCoor, PuzzlePiece& myPart)
{
       
    PuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m,myCoor.n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m-1,myCoor.n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m,myCoor.n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m+1,myCoor.n).getConnections() & 0b00000011));
    negativePart.shift(2);


    if  ( 
          (    ((((negativePart.getConnections() & 0b11000000) ^  (myPart.getConnections() & 0b11000000))  != 0b00000000)                  && (((myPart.getConnections() & 0b11000000) != 0b00000000) && (negativePart.getConnections() & 0b11000000) != 0b00000000))
            || ((((negativePart.getConnections() & 0b11000000) == 0b11000000) || ((myPart.getConnections() &  0b11000000) == 0b11000000))  && (((myPart.getConnections() & 0b11000000) != 0b00000000) && (negativePart.getConnections() & 0b11000000) != 0b00000000))
            ||  (((negativePart.getConnections() & 0b11000000) == 0b00000000) && ((myPart.getConnections() &  0b11000000) == 0b00000000))  ) 
        &&
          (    ((((negativePart.getConnections() & 0b00110000) ^ (myPart.getConnections() & 0b00110000))  != 0b00000000)                   && (((myPart.getConnections() & 0b00110000) != 0b00000000) && (negativePart.getConnections() & 0b00110000) != 0b00000000))
            || ((((negativePart.getConnections() & 0b00110000) == 0b00110000) || ((myPart.getConnections() &  0b00110000) == 0b00110000))  && (((myPart.getConnections() & 0b00110000) != 0b00000000) && (negativePart.getConnections() & 0b00110000) != 0b00000000))
            ||  (((negativePart.getConnections() & 0b00110000) == 0b00000000) && ((myPart.getConnections() &  0b00110000) == 0b00000000))  ) 
        &&
          (    ((((negativePart.getConnections() & 0b00001100) ^  (myPart.getConnections() & 0b00001100))  != 0b00000000)                  && (((myPart.getConnections() & 0b00001100) != 0b00000000) && (negativePart.getConnections() & 0b00001100) != 0b00000000))
            || ((((negativePart.getConnections() & 0b00001100) == 0b00001100) || ((myPart.getConnections() &  0b00001100) == 0b00001100))  && (((myPart.getConnections() & 0b00001100) != 0b00000000) && (negativePart.getConnections() & 0b00001100) != 0b00000000))
            ||  (((negativePart.getConnections() & 0b00001100) == 0b00000000) && ((myPart.getConnections() &  0b00001100) == 0b00000000))  ) 
        &&
          (    ((((negativePart.getConnections() & 0b00000011) ^  (myPart.getConnections() & 0b00000011))  != 0b00000000)                  && (((myPart.getConnections() & 0b00000011) != 0b00000000) && (negativePart.getConnections() & 0b00000011) != 0b00000000))
            || ((((negativePart.getConnections() & 0b00000011) == 0b00000011) || ((myPart.getConnections() &  0b00000011) == 0b00000011))  && (((myPart.getConnections() & 0b00000011) != 0b00000000) && (negativePart.getConnections() & 0b00000011) != 0b00000000))
            ||  (((negativePart.getConnections() & 0b00000011) == 0b00000000) && ((myPart.getConnections() &  0b00000011) == 0b00000000))  )    
        )
    {    
        //cout << "good Part: ";
        //myPart.printPiece();
        //cout << endl;       
        return true;
    }
    //cout << "bad Part: ";
    //myPart.printPiece();
    //cout << endl;       

    return false;

}


//TODO!!
//simpler algorithm to the first placeofpartgood 
//not yet functional!!!
bool Puzzle::PlaceOfPart2Good(coor myCoor, PuzzlePiece& myPart)
{
        PuzzlePiece tmpPuzzlePiece = myPart;    
    
    //make tmp a negative part
    if(((tmpPuzzlePiece.getConnections() & 0b11000000) != 0b11000000) || ((tmpPuzzlePiece.getConnections() & 0b11000000) != 0b00000000))
        tmpPuzzlePiece.setConnections(tmpPuzzlePiece.getConnections() ^ 0b11000000);
    if(((tmpPuzzlePiece.getConnections() & 0b00110000) != 0b00110000) || ((tmpPuzzlePiece.getConnections() & 0b00110000) != 0b00000000))
        tmpPuzzlePiece.setConnections(tmpPuzzlePiece.getConnections() ^ 0b00110000);
    if(((tmpPuzzlePiece.getConnections() & 0b00001100) != 0b00001100)|| ((tmpPuzzlePiece.getConnections() & 0b00001100) != 0b00000000))
        tmpPuzzlePiece.setConnections(tmpPuzzlePiece.getConnections() ^ 0b00001100);
    if(((tmpPuzzlePiece.getConnections() & 0b00000011) != 0b00000011) || ((tmpPuzzlePiece.getConnections() & 0b00000011) != 0b00000000))
        tmpPuzzlePiece.setConnections(tmpPuzzlePiece.getConnections() ^ 0b00000011);
        
    PuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m,myCoor.n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m-1,myCoor.n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m,myCoor.n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(myCoor.m+1,myCoor.n).getConnections() & 0b00000011));
    
    negativePart.shift(2);
    
    //check tmp part with environment
    if(((negativePart.getConnections() & 0b11000000) == (tmpPuzzlePiece.getConnections() & 0b11000000)) && ((negativePart.getConnections() & 0b00110000) == (tmpPuzzlePiece.getConnections ()& 0b00110000)) && 
            ((negativePart.getConnections() & 0b00001100) == (tmpPuzzlePiece.getConnections() & 0b00001100)) && ((negativePart.getConnections() & 0b00000011) == (tmpPuzzlePiece.getConnections() & 0b00000011)))
        return true;
    
    return false;

}

//prints the true puzzle (without 0 edges)
void Puzzle::printPuzzle()
{
    cout << "current Puzzle: " << endl;
    for(int i=1;i<row+1;i++)
    {
        for(int j=1;j<col+1;j++)
        {
            Matrix[i][j].printPiece();
            cout << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

//creates a legal puzzle out of random pieces

void randomBox::createRandomAbstraction1()
{
    coor myCoor;
    PuzzlePiece temporaryRandomPiece(0);

    for(int i=0;i<getRows();i++)
    {
        for(int j = 0; j < getCols();)
        {

            //create random piece, set edges according to position and check if piece is good
            temporaryRandomPiece.randomCenterPiece();
            if(i==0)
                temporaryRandomPiece.setConnections(0b00111111 & temporaryRandomPiece.getConnections());
            if(i==getRows()-1)
                temporaryRandomPiece.setConnections(0b11110011 & temporaryRandomPiece.getConnections());
            if(j==0)
                temporaryRandomPiece.setConnections(0b11111100 & temporaryRandomPiece.getConnections());
            if(j==getCols()-1)
                temporaryRandomPiece.setConnections(0b11001111 & temporaryRandomPiece.getConnections());

            myCoor.m = j;
            myCoor.n = i;
            if(PlaceOfPartGood(myCoor,temporaryRandomPiece))
            {
                temporaryRandomPiece.assignIdentifier();
                setPiece(myCoor,temporaryRandomPiece);
                j++;
            }
        }
    }
}

void randomBox::createRandomAbstraction2()
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

void randomBox::putAllIntoBox() {
    for (int i = 0; i < getRows(); i++)
    {
        for (int j = 0; j < getCols(); j++)
        {
            Box.push_back(getPiece(j,i));
        }
    }
}

//prints a box contents on console
void randomBox::printBox()
{
    shuffle();
    for (auto i:Box)
    {
        i.printPiece();
        cout << ' ';
    }
    cout << endl;
}

//shuffles around a box, randomizing pieces and orientation
vector<PuzzlePiece> randomBox::shuffle()
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

//creates a random box size m, n, shuffles it, and then retuns it
vector<PuzzlePiece> createBox(coor myCoor)
{
    randomBox myFirstPuzzleBox(myCoor.m,myCoor.n);
    myFirstPuzzleBox.createRandomAbstraction1();
    myFirstPuzzleBox.createRandomAbstraction2();
    myFirstPuzzleBox.putAllIntoBox();

    myFirstPuzzleBox.printPuzzle();
    return myFirstPuzzleBox.shuffle();
}

//prints contents of box
void printBox(vector<PuzzlePiece> myBox)
{
    cout << "current Box: " << endl;
    for (auto &i:myBox)
    {
        i.printPiece();
        cout << ' ';
    }
    cout << endl;
}

//gives every element in box a box identifier.
void numerateBox(vector<PuzzlePiece>& myBox)
{
    for(int i = 0; i< myBox.size();i++)
        myBox[i].setBoxIdentifier(i);

}

std::vector<PuzzlePiece> convertPart2PuzzlePiece(std::vector<Part> simplePartBox)
{
	std::vector<PuzzlePiece> advancedPartBox;
	for(auto const &i:simplePartBox)
	{
		PuzzlePiece tmpNewPiece(0);
		tmpNewPiece.setConnections(i.getConnections());
		advancedPartBox.push_back(tmpNewPiece);
	}
	return advancedPartBox;
}