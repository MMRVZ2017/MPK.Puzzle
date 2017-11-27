//
// Created by Christoph Weidinger on 23.11.17.
//
#include <iostream>
#include <vector>
#include "defines.h"
#include "puzzleBox.h"
#include "step.h"
#include "constrMatrix.h"

#ifndef INC_2017_11_17_PUZZLESOLVER_PUZZLEGENERATOR_HPP
#define INC_2017_11_17_PUZZLESOLVER_PUZZLEGENERATOR_HPP


using namespace std;


class randomPuzzlePiece: public Part
{
public:

    randomPuzzlePiece(unsigned int flag = 0)
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
    Part          setPart(randomPuzzlePiece oldrandomPuzzlePiece);

private:
    unsigned int  shifts;
    unsigned int  boxidentifier;
    unsigned int  identifier;

    static unsigned int idcount;
};





class coor
{
public:
    int m, n;
    coor(int newm=-1,int newn=-1): m(newm), n(newn)
    {}
};


class BoxClassify
{
    vector<randomPuzzlePiece*> p_Box;
    vector<vector<randomPuzzlePiece*>> ab1class;
};


class randomPuzzle
{
    friend class randomBox;
public:
    //constructor creates matrix with 00 outside and 11 inside
    randomPuzzle(uint m = 7, uint n = 4): col(m), row(n)
    {
        Matrix = new randomPuzzlePiece* [n+2];
        for(int i = 0;i<n+2;i++)
        {
            Matrix[i] = new randomPuzzlePiece[m+2];
            for(int j = 0;j<m+2;j++)
            {
                if(i==0 || j==0 || i==n+1 || j==m+1)
                {
                    Matrix[i][j] = randomPuzzlePiece(0);
                }
                else
                {
                    Matrix[i][j] = randomPuzzlePiece(1);
                }
            }
        }
    }



    bool setPiece(uint m,uint n,randomPuzzlePiece newPiece);
    bool removePiece(uint m,uint n);

    //getter
    randomPuzzlePiece getPiece(uint m,uint n) { return Matrix[n+1][m+1]; }
    uint getCols(){ return col; }
    uint getRows(){ return row; }

    //function definitions
    void printPuzzle();
    bool PlaceOfPartGood(unsigned int m, unsigned int n, randomPuzzlePiece& myPart);
    bool PlaceOfPart2Good(unsigned int m,unsigned int n, randomPuzzlePiece& myPart);

    bool testRotationPiece(unsigned int m, unsigned int n, randomPuzzlePiece& myPart);
    int tryAllPieces(unsigned int m, unsigned int n, vector<randomPuzzlePiece>& myBox, unsigned int separator);
    unsigned int putBackIntoBox(unsigned int m, unsigned int n, vector<randomPuzzlePiece>& myBox);


private:
    uint row;
    uint col;

    randomPuzzlePiece** Matrix;

};

//use this for random puzzle creation
class randomBox: public randomPuzzle
{
public:

    randomBox(unsigned int m, unsigned int n) : randomPuzzle(m,n) {srand(time(0));} //passed m n to puzzle constructor

    void createRandomPuzzle();
    vector<Part> shuffle();
    void printBox();

private:
    vector<Part> Box;

};

#endif //INC_2017_11_17_PUZZLESOLVER_PUZZLEGENERATOR_HPP