#include <stdint.h>

#define NR_PARTS 1008
#define NR_CORNERS 4
#define NR_EDGES 120
#define NR_INNERS 884

using namespace std;

//part from group header file
class Part
{
public:
    Part(): connections(0){}
    ~Part() {}
    uint8_t getConnections() const
    {
        return connections;
    }
    void setConnections(uint8_t newconnections)
    {
        connections = newconnections;
    }

private:
    uint8_t connections;
};

//puzzlepiece extens part with essential identifiers and functions
class PuzzlePiece: public Part
{
public:

    PuzzlePiece(unsigned int flag = 0)
    {
        shifts=0;
        boxidentifier=-1;
        switch(flag)
        {
            case 0:
            setConnections(0b00000000);
            break;
            case 1:
            setConnections(0b11111111);
            break;
            case 3:
            randomCenterPiece();
            break;
        }
    }

    void          shift(unsigned int moves);
    void          randomCenterPiece();
    void          printPiece() { cout << bitset<sizeof(unsigned char)*8> (getConnections()); }

    void          setBoxIdentifier(int new_boxid) { boxidentifier = new_boxid; }   
    int           getBoxIdentifier() { return boxidentifier; } 
    void          assignIdentifier() { identifier = idcount;idcount++; }
    unsigned int  getIdentifier() { return identifier;}   

private:
    unsigned int  shifts;
    unsigned int  boxidentifier;
    unsigned int  identifier;
    static unsigned int idcount;
};

unsigned int PuzzlePiece::idcount(0);

class Puzzle
{
    friend class randomBox;
public:
    //constructor creates matrix with 00 outside and 11 inside
    Puzzle(uint m = 7, uint n = 4): col(m), row(n)
    {
        Matrix = new PuzzlePiece* [n+2];
        for(int i = 0;i<n+2;i++)
        {
            Matrix[i] = new PuzzlePiece[m+2];
            for(int j = 0;j<m+2;j++)
            {
                if(i==0 || j==0 || i==n+1 || j==m+1)
                {
                    Matrix[i][j] = PuzzlePiece(0);
                }
                else
                {
                    Matrix[i][j] = PuzzlePiece(1);
                }
            }
        }
    }

    bool setPiece(uint m,uint n,PuzzlePiece newPiece)
    {
        if(PlaceOfPartGood(m,n,newPiece))
        {
            Matrix[n+1][m+1] = newPiece;    
            return 1;
        }
        return 0;
    }

    //removes piece and instead puts undefined piece
    bool removePiece(uint m,uint n)
    {
        Matrix[n+1][m+1].setConnections(0b11111111);  
        return 1;
    }

    //getter
    PuzzlePiece getPiece(uint m,uint n) { return Matrix[n+1][m+1]; }
    uint getCols(){ return col; }
    uint getRows(){ return row; }

    //functtion definitions
    void printPuzzle();
    bool PlaceOfPartGood(unsigned int m, unsigned int n, PuzzlePiece& myPart);
    bool PlaceOfPart2Good(unsigned int m,unsigned int n, PuzzlePiece& myPart);

    bool testRotationPiece(unsigned int m, unsigned int n, PuzzlePiece& myPart);
    unsigned int tryAllPieces(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox, unsigned int separator);
    unsigned int putBackIntoBox(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox);


private:
    uint row;
    uint col;

    PuzzlePiece** Matrix;
    
};

//use this for random puzzle creation
class randomBox: public Puzzle
{
public:

    randomBox(unsigned int m, unsigned int n) : Puzzle(m,n) {} //passed m n to puzzle constructor
    
    void createRandomPuzzle();
    vector<PuzzlePiece> shuffle();
    void printBox();

private:
    vector<PuzzlePiece*> Box;

};

class coor
{
    int m, n;
    coor(int newm,int newn): m(newm), n(newn)
    {}
}

class LogEntry
{
public:
    vector<PuzzlePiece*> PieceCollector;
    vector<coor> CoorCollector; 
    int abstractionLevel;

    uint myCoor;
    PuzzlePiece* myPuzzlePiece;

    PuzzleCollector()
    {

        myCoor.m=-1;
        myCoor.n=-1;
        abstractionLevel=0;
    }
private:
}


//functiondefinitions
void printBox(vector<PuzzlePiece> myBox);
vector<PuzzlePiece> createBox(uint m, uint n);
void numerateBox(vector<PuzzlePiece>& myBox);

//functions

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

//tries all pieces in box from separator to end and places fitting into matrix. removes fitting piece
//use separator if you have to retract to a position
//seperator may be bigger than box size, if all puzzle pieces have already been looked at.
// it immediately retracts again then (returns -1)
unsigned int Puzzle::tryAllPieces(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox, unsigned int separator)
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
bool Puzzle::testRotationPiece(unsigned int m, unsigned int n, PuzzlePiece& myPart)
{
    for(int rotation=0; rotation < 4; rotation++)
    {
        if(PlaceOfPartGood(m,n, myPart))
            return 1;
        myPart.shift(1);
    }
#ifdef debug
cout << "bad part: ";
myPart.printPiece();
cout << endl;
#endif
    return 0;
}

//insterts piece at position in box according to boxidentifier and removes piece from puzzle
//this returns the position after!! the puzzle piece was put back in! not the boxidentifier of the piece. look that up in other function.
unsigned int Puzzle::putBackIntoBox(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox)
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
bool Puzzle::PlaceOfPartGood(unsigned int m,unsigned int n, PuzzlePiece& myPart)
{

    PuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m-1,n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m+1,n).getConnections() & 0b00000011));
    negativePart.shift(2);



    if  ( 
          (    ((((negativePart.getConnections() & 0b11000000) ^  (myPart.getConnections() & 0b11000000))  != 0b00000000)                  && (((myPart.getConnections() & 0b11000000) != 0b00000000) && (negativePart.getConnections() & 0b11000000) != 0b00000000))
            || ((((negativePart.getConnections() & 0b11000000) == 0b11000000) || ((myPart.getConnections() &  0b11000000) == 0b11000000))  && (( myPart.getConnections() & 0b11000000) != 0b00000000) && (negativePart.getConnections() & 0b11000000) != 0b00000000)
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
#ifdef debug        
cout << "good Part: ";
myPart.printPiece();
cout << endl;
#endif        
        return 1;
    }



    return 0;
}


//TODO!!
//simpler algorithm to the first placeofpartgood 
//not yet functional!!!
bool Puzzle::PlaceOfPart2Good(unsigned int m,unsigned int n, PuzzlePiece& myPart)
{

    PuzzlePiece negativePart(0);

    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n+1).getConnections() & 0b11000000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m-1,n).getConnections() & 0b00110000));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m,n-1).getConnections() & 0b00001100));
    negativePart.setConnections(negativePart.getConnections() | (getPiece(m+1,n).getConnections() & 0b00000011));
    negativePart.shift(2);

    //A and D or B and C or not A and not B and not C and not D

    if  ( 
          (    (  (negativePart.getConnections() & 0b10000000)  &  (myPart.getConnections() & 0b01000000))
            || (  (negativePart.getConnections() & 0b01000000)  &  (myPart.getConnections() & 0b10000000))
            || ((!(negativePart.getConnections() & 0b10000000)  & !(myPart.getConnections() & 0b10000000)) & (!(negativePart.getConnections() & 0b01000000) & !(myPart.getConnections() & 0b01000000)))
          )
        &&
          (    (  (negativePart.getConnections() & 0b00100000)  &  (myPart.getConnections() & 0b00010000))
            || (  (negativePart.getConnections() & 0b00010000)  &  (myPart.getConnections() & 0b00100000))
            || ((!(negativePart.getConnections() & 0b00100000)  & !(myPart.getConnections() & 0b00100000)) & (!(negativePart.getConnections() & 0b00010000) & !(myPart.getConnections() & 0b00010000)))
          )
        &&
          (    (  (negativePart.getConnections() & 0b00001000)  &  (myPart.getConnections() & 0b00000100))
            || (  (negativePart.getConnections() & 0b00000100)  &  (myPart.getConnections() & 0b00001000))
            || ((!(negativePart.getConnections() & 0b00001000)  & !(myPart.getConnections() & 0b00001000)) & (!(negativePart.getConnections() & 0b00000100) & !(myPart.getConnections() & 0b00000100)))
          )
        &&
          (    (  (negativePart.getConnections() & 0b00000010)  &  (myPart.getConnections() & 0b00000001))
            || (  (negativePart.getConnections() & 0b00000001)  &  (myPart.getConnections() & 0b00000010))
            || ((!(negativePart.getConnections() & 0b00000010)  & !(myPart.getConnections() & 0b00000010)) & (!(negativePart.getConnections() & 0b00000001) & !(myPart.getConnections() & 0b00000001)))
          )
        )

        return 1;

    cout << "nogood" << endl;
    return 0;
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
void randomBox::createRandomPuzzle()
{
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

            if(PlaceOfPartGood(j,i,temporaryRandomPiece))
            {
                temporaryRandomPiece.assignIdentifier();
                setPiece(j,i,temporaryRandomPiece);     
                j++;
                Box.push_back(temporaryRandomPiece);
            }
        }
    }
}

//prints a box contents on console
void randomBox::printBox()
{
    shuffle();
    for (vector<PuzzlePiece>::iterator i = Box.begin(); i != Box.end(); i++)    
    {
        (*i).printPiece();
        cout << ' ';
    }       
    cout << endl;
}

//shuffles around a box, randomizing pieces and orientation
vector<PuzzlePiece> randomBox::shuffle()
{
    random_shuffle(Box.begin(),Box.end());
    for (vector<PuzzlePiece>::iterator i = Box.begin(); i != Box.end(); i++)  
        (*i).shift(rand()%4);
    numerateBox(Box);
    return Box;
}

//creates a random box size m, n, shuffles it, and then retuns it
vector<PuzzlePiece> createBox(uint m, uint n)
{
    randomBox myFirstPuzzleBox(m,n); 
    myFirstPuzzleBox.createRandomPuzzle(); 
    return myFirstPuzzleBox.shuffle(); 
}

//prints contents of box
void printBox(vector<PuzzlePiece> myBox)
{
    cout << "current Box: " << endl;
    for (vector<PuzzlePiece>::iterator i = myBox.begin(); i != myBox.end(); i++)    
    {
        (*i).printPiece();
        cout << ' ';
    }   
    cout << endl;
    return;
}

//gives every element in box a box identifier.
void numerateBox(vector<PuzzlePiece>& myBox)
{
    for(int i = 0; i< myBox.size();i++)
        myBox[i].setBoxIdentifier(i);

    return;
}