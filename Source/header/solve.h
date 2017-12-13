#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

class coor
{
public:
    int m, n;
    coor(int newm=-1,int newn=-1): m(newm), n(newn)
    {}
};

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
    unsigned int  getShift(){return shifts;}
    void          resetShift(){shifts=0;}
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



class Puzzle
{
    friend class randomBox;
public:
    //constructor creates matrix with 00 outside and 11 inside
    Puzzle(unsigned int m = 7, unsigned int n = 4): col(m), row(n)
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

    bool setPiece(coor myCoor,PuzzlePiece newPiece) // 5th change: works
    {
        if(PlaceOfPartGood(myCoor,newPiece))
        {
            Matrix[myCoor.n+1][myCoor.m+1] = newPiece;
            return 1;
        }
        return 0;
    }

    //removes piece and instead puts undefined piece
    bool removePiece(coor myCoor)   // 6th change: works
    {
        Matrix[myCoor.n+1][myCoor.m+1].setConnections(0b11111111);
        return 1;
    }

    //getter
    PuzzlePiece getPiece(unsigned int m, unsigned int n) { return Matrix[n+1][m+1]; }    // 7th change: error m+1, n+1
    unsigned int getCols(){ return col; }
    unsigned int getRows(){ return row; }

    //functtion definitions
    void printPuzzle();
    bool PlaceOfPartGood(coor myCoor, PuzzlePiece& myPart); // 8th change: works
    bool PlaceOfPart2Good(coor myCoor, PuzzlePiece& myPart); // 2nd change: works

    bool testRotationPiece(coor myCoor, PuzzlePiece& myPart, int nrOfRotations=4); // 9th change: works
    unsigned int tryAllPieces(coor myCoor, vector<PuzzlePiece>& myBox, unsigned int separator); // 3rd change: works
    unsigned int putBackIntoBox(coor myCoor, vector<PuzzlePiece>& myBox); // 4th change: works


private:
    unsigned int row;
    unsigned int col;

    PuzzlePiece** Matrix;
    
};

//use this for random puzzle creation
class randomBox: public Puzzle
{
public:

    randomBox(unsigned int m, unsigned int n) : Puzzle(m,n) {srand(time(0));} //passed m n to puzzle constructor

    void createRandomAbstraction1();
    void createRandomAbstraction2();
    void putAllIntoBox();
    vector<PuzzlePiece> shuffle();
    void printBox();

private:
    vector<PuzzlePiece> Box;

};


class LogEntry
{
public:
    vector<PuzzlePiece*> PieceCollector;
    int abstractionLevel;
    coor myCoor;

    void advance(){abstractionLevel++;}
    void Set(){set=1;}
    bool isSet(){return set;}
    void advanceRandomed() { randomed++;}
    void decreaseRandomed() { randomed--;}
    int hasRandomed(){return randomed;}

    LogEntry()
    {
        myCoor = coor();
        abstractionLevel=0;
        set=0;
    }
private:
    bool set;
    static int randomed;
};

void printBox(vector<PuzzlePiece> myBox);
vector<PuzzlePiece> createBox(coor myCoor);
void numerateBox(vector<PuzzlePiece>& myBox);

bool next(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
coor calculateNextCoor(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
void solve(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
void abstractionlayer0solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
void abstractionlayer1solver(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
void setsolution(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);
bool backtrack(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);

void createNextLogElement(vector<LogEntry>& log, vector<PuzzlePiece*>& p_Box, Puzzle& puzzleMat);