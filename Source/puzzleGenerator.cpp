#include "puzzleGenerator.hpp"

using namespace std;

Part randomPuzzlePiece::setPart(randomPuzzlePiece oldrandomPuzzlePiece)
{
    Part newPart;
    newPart.setConnections(oldrandomPuzzlePiece.getConnections());
    return newPart;
}

bool randomPuzzle::setPiece(uint m,uint n,randomPuzzlePiece newPiece)
{
    if(PlaceOfPartGood(m,n,newPiece))
    {
        Matrix[n+1][m+1] = newPiece;
        return 1;
    }
    return 0;
}

//removes piece and instead puts undefined piece
bool randomPuzzle::removePiece(uint m,uint n)
{
    Matrix[n+1][m+1].setConnections(0b11111111);
    return 1;
}


unsigned int randomPuzzlePiece::idcount(0);


void printBox(vector<randomPuzzlePiece> myBox);
vector<Part> createBox(uint m, uint n);
void numerateBox(vector<randomPuzzlePiece>& myBox);


//shifts puzzle piece one to the right
void randomPuzzlePiece::shift(unsigned int moves)
{
    shifts = (shifts+moves)%4;
    setConnections(((getConnections() >> (moves*2)) | (getConnections() << sizeof(unsigned char)*8 - (moves*2))));
}


//creates random centerpiece
void randomPuzzlePiece::randomCenterPiece()
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

//tries all pieces in box from separator to end and places fitting into matrix. removes fitting piece
//use separator if you have to retract to a position
//seperator may be bigger than box size, if all puzzle pieces have already been looked at.
// it immediately retracts again then (returns -1)
unsigned int randomPuzzle::tryAllPieces(unsigned int m, unsigned int n, vector<randomPuzzlePiece>& myBox, unsigned int separator)
{
    for(int i=separator; i<myBox.size();i++)
    {
        if(testRotationPiece(m,n,myBox[i]))
        {
            setPiece(m,n,myBox[i]);
            myBox.erase(myBox.begin()+i);
            return i;
        }
    }

    return -1;
}

//tests the myPart in all 4 rotations at position m, n
bool randomPuzzle::testRotationPiece(unsigned int m, unsigned int n, randomPuzzlePiece& myPart)
{
    for(int rotation=0; rotation < 4; rotation++)
    {
        if(PlaceOfPartGood(m,n,myPart))
            return 1;
        myPart.shift(1);
    }

    //cout << "Was a bad part" << endl;
    return 0;
}

//insterts piece at position in box according to boxidentifier and removes piece from puzzle
//this returns the position after!! the puzzle piece was put back in! not the boxidentifier of the piece. look that up in other function.
unsigned int randomPuzzle::putBackIntoBox(unsigned int m, unsigned int n, vector<randomPuzzlePiece>& myBox)
{
#ifdef debug
    cout << "putting back" << endl;
cout << "Old Box: ";
printBox(myBox);
cout << endl;
#endif
    for(int i = 0; i < myBox.size();i++)
    {
        if(myBox[i].getBoxIdentifier()>getPiece(m,n).getBoxIdentifier())
        {
            myBox.insert(myBox.begin()+i,getPiece(m,n));
            removePiece(m,n);
            return i+1;
        }
    }
    //using push back, if the element was the last element in the vector chain
    myBox.push_back(getPiece(m,n));
    removePiece(m,n);
    return myBox.size();
}

//checks if the myPart in its current orientation is legal in position m, n
bool randomPuzzle::PlaceOfPartGood(unsigned int m,unsigned int n, randomPuzzlePiece& myPart)
{

    randomPuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m-1,n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m+1,n).getConnections() & 0b00000011));
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
        return 1;
    }
    //cout << "bad Part: ";
    //myPart.printPiece();
    //cout << endl;

    return 0;

}


//TODO!!
//simpler algorithm to the first placeofpartgood
//not yet functional!!!
bool randomPuzzle::PlaceOfPart2Good(unsigned int m,unsigned int n, randomPuzzlePiece& myPart)
{
    randomPuzzlePiece tmprandomPuzzlePiece = myPart;

    //make tmp a negative part
    if(((tmprandomPuzzlePiece.getConnections() & 0b11000000) != 0b11000000) || ((tmprandomPuzzlePiece.getConnections() & 0b11000000) != 0b00000000))
        tmprandomPuzzlePiece.setConnections(tmprandomPuzzlePiece.getConnections() ^ 0b11000000);
    if(((tmprandomPuzzlePiece.getConnections() & 0b00110000) != 0b00110000) || ((tmprandomPuzzlePiece.getConnections() & 0b00110000) != 0b00000000))
        tmprandomPuzzlePiece.setConnections(tmprandomPuzzlePiece.getConnections() ^ 0b00110000);
    if(((tmprandomPuzzlePiece.getConnections() & 0b00001100) != 0b00001100)|| ((tmprandomPuzzlePiece.getConnections() & 0b00001100) != 0b00000000))
        tmprandomPuzzlePiece.setConnections(tmprandomPuzzlePiece.getConnections() ^ 0b00001100);
    if(((tmprandomPuzzlePiece.getConnections() & 0b00000011) != 0b00000011) || ((tmprandomPuzzlePiece.getConnections() & 0b00000011) != 0b00000000))
        tmprandomPuzzlePiece.setConnections(tmprandomPuzzlePiece.getConnections() ^ 0b00000011);

    randomPuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m-1,n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m+1,n).getConnections() & 0b00000011));

    negativePart.shift(2);

    //check tmp part with environment
    if(((negativePart.getConnections() & 0b11000000) == (tmprandomPuzzlePiece.getConnections() & 0b11000000)) && ((negativePart.getConnections() & 0b00110000) == (tmprandomPuzzlePiece.getConnections ()& 0b00110000)) &&
       ((negativePart.getConnections() & 0b00001100) == (tmprandomPuzzlePiece.getConnections() & 0b00001100)) && ((negativePart.getConnections() & 0b00000011) == (tmprandomPuzzlePiece.getConnections() & 0b00000011)))
        return 1;

    return 0;

}

//prints the true puzzle (without 0 edges)
void randomPuzzle::printPuzzle()
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
void randomBox::createRandomPuzzle()
{
    randomPuzzlePiece temporaryRandomPiece(0);
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

            if(PlaceOfPartGood(j,i,temporaryRandomPiece))
            {
                temporaryRandomPiece.assignIdentifier();
                setPiece(j,i,temporaryRandomPiece);
                j++;
                Box.push_back(temporaryRandomPiece.setPart(temporaryRandomPiece));
            }
        }
    }
}

//prints a box contents on console
void randomBox::printBox()
{
    shuffle();
    for (vector<Part>::iterator i = Box.begin(); i != Box.end(); i++)
    {
        cout << bitset<sizeof(unsigned char)*8> ((*i).getConnections()) << endl;
    }
    cout << endl;
}

//shuffles around a box, randomizing pieces and orientation
vector<Part> randomBox::shuffle()
{
    //random_shuffle(Box.begin(),Box.end());
    for (vector<Part>::iterator i = Box.begin(); i != Box.end(); i++)
    {
        int moves = rand()%4;
        (*i).setConnections((((*i).getConnections() >> (moves*2)) | ((*i).getConnections() << sizeof(unsigned char)*8 - (moves*2))));
    }
    return Box;
}

//creates a random box size m, n, shuffles it, and then retuns it
vector<Part> createBox(uint m, uint n)
{
    randomBox myFirstPuzzleBox(m,n);
    myFirstPuzzleBox.createRandomPuzzle();
    return myFirstPuzzleBox.shuffle();
}

//prints contents of box
void printBox(vector<Part> myBox)
{
    cout << "current Box: " << endl;
    for (vector<Part>::iterator i = myBox.begin(); i != myBox.end(); i++)
    {
        cout << bitset<sizeof(unsigned char)*8> ((*i).getConnections());
        cout << endl;
    }
    cout << endl;
    return;
}

//gives every element in box a box identifier.
void numerateBox(vector<randomPuzzlePiece>& myBox)
{
    for(int i = 0; i< myBox.size();i++)
        myBox[i].setBoxIdentifier(i);

    return;
}

