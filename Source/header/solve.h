
using namespace std;

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

    randomBox(unsigned int m, unsigned int n) : Puzzle(m,n) {srand(time(0));} //passed m n to puzzle constructor
    
    void createRandomPuzzle();
    vector<PuzzlePiece> shuffle();
    void printBox();

private:
    vector<PuzzlePiece> Box;

};

class coor
{
public:
    int m, n;
    coor():m(-1),n(-1){}
    coor(int newm,int newn): m(newm), n(newn)
    {}
};

class LogEntry
{
public:
    vector<PuzzlePiece*> PieceCollector;
    vector<coor> CoorCollector; 
    int abstractionLevel;

    coor myCoor;
    PuzzlePiece* myPuzzlePiece;

    LogEntry()
    {
        abstractionLevel=0;
    }
private:
};

void printBox(vector<PuzzlePiece> myBox);
vector<PuzzlePiece> createBox(uint m, uint n);
void numerateBox(vector<PuzzlePiece>& myBox);