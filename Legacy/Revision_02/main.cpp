//Raphael Maenle - 11.11.2017
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

#define debug

#include "puzzle.h"

#include "test_puzzle.h"

using namespace std;

Puzzle solveOuterFirst( unsigned int rows, unsigned int cols, vector<PuzzlePiece>& myFirstBox);
void retractOuterFirst(int& i, int& j, int cols, int rows, int& perimiter);

int main()
{
	srand(time(0));

    vector<Part> part_array(NR_PARTS);
    vector<Part *> corners_array(NR_CORNERS);
    vector<Part *> edges_array(NR_EDGES);
    vector<Part *> inners_array(NR_INNERS);
    
    //randomBox myPuzzle(cols,rows);
    //myPuzzle.createRandomPuzzle();


    unsigned int rows=5, cols=5;
    
    //vector<PuzzlePiece> myFirstBox = createBox(cols, rows);
    
    //create4040hardBox(myFirstBox);
    /*myFirstBox[0].setConnections(0b01100010);
    myFirstBox[1].setConnections(0b00010100);
    myFirstBox[2].setConnections(0b00011010);
    myFirstBox[3].setConnections(0b10000001);
    myFirstBox[4].setConnections(0b00011000);
    myFirstBox[5].setConnections(0b01100000);*/

    Puzzle myFirstPuzzle(3,4);
    PuzzlePiece myFirstPiece(1);
    myFirstPiece.setConnections(0b00010100);

    if(myFirstPuzzle.PlaceOfPartGood(myFirstPiece));
        cout << "good" << endl;

    //printBox(myFirstBox);
    cout << endl;

    //Puzzle newPuzzle = solveOuterFirst(cols, rows, myFirstBox);
    return 0;
}


//rotates in snail form clockwise around board and tries placing puzzle pieces.
//if no piece fits in position it tries next piece of the piece before
Puzzle solveOuterFirst( unsigned int cols, unsigned int rows, vector<PuzzlePiece>& myFirstBox)
{
    int i = 0, j= 0, Boxsize = rows*cols, separator=0;
    Puzzle myFirstPuzzle(cols, rows);
    //first Piece
    

    //rotate through puzzle
    int perimiter;
    int numberofsolutions=0;
    int newPerimiter = 1;
    for(perimiter = 0; /*perimiter <= (cols>rows?rows:cols)/2;*/;)
    {
        if(Boxsize)
        {
#ifdef debug
cout << "perimiter: " << perimiter << endl;
#endif
        }
        else 
        {
            break;
            retractOuterFirst(i,j,cols,rows,perimiter);
            Boxsize++;
            separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
            numberofsolutions++;
        }    

        if((i==rows/2 && j==cols/2 )&& cols==rows)
        {
            if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
            {
                retractOuterFirst(i,j,cols,rows,perimiter);
                Boxsize++;
                separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
            }
            else
            {
                break;
                separator=0;
                Boxsize--;
            }
        }

        
        if((i==perimiter+1 && j==perimiter))
        {
#ifdef debug
cout << "#0 i: " << i << ", j: " << j << endl;
#endif
            if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
            {
                retractOuterFirst(i,j,cols,rows,perimiter);
                Boxsize++;
                separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
#ifdef debug
cout << "New Box: "; 
printBox(myFirstBox);
cout << endl;
myFirstPuzzle.printPuzzle();
cout << "Boxsize: " << Boxsize << endl;
cout << "separator: " << separator << endl; 
#endif
            }
            else
            {
                perimiter=newPerimiter;
                newPerimiter++;
                j++;
                separator=0;
                Boxsize--;
#ifdef debug
cout << " Perimiter: " << perimiter << endl;
myFirstPuzzle.printPuzzle();
#endif
            }
            
        }
        else
        {


            //cout << "general information i: " << i << ", j: " << j << endl;
            while(( i == 0+perimiter && j < cols-perimiter-1) && Boxsize)
            {
#ifdef debug                
cout << "#1 i: " << i << ", j: " << j << endl;
#endif
                if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
                {
                    retractOuterFirst(i,j,cols,rows,perimiter);
                    separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
                    Boxsize++;
#ifdef debug
cout << "New Box: "; 
printBox(myFirstBox);
cout << endl;
myFirstPuzzle.printPuzzle();
cout << "Boxsize: " << Boxsize << endl;
cout << "separator: " << separator << endl; 
#endif
                    break;
                }
                else
                {
                    j++;
                    separator=0;
                    Boxsize--;
#ifdef debug
myFirstPuzzle.printPuzzle();
#endif
                }
            }
            while(( i < rows-perimiter-1 && j == cols-perimiter-1) && Boxsize)
            {
#ifdef debug                
cout << "#2 i: " << i << ", j: " << j << endl;
#endif   
                if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
                {
                    retractOuterFirst(i,j,cols,rows,perimiter);

                    separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
                    Boxsize++;
#ifdef debug
cout << "New Box: "; 
printBox(myFirstBox);
cout << endl;
myFirstPuzzle.printPuzzle();

cout << "Boxsize: " << Boxsize << endl;
cout << "separator: " << separator << endl; 
#endif            
                    break;
                }
                else
                {
                    i++;
                    separator=0;
                    Boxsize--;
#ifdef debug                   
myFirstPuzzle.printPuzzle();              
#endif            
                }
            }

            while(( i == rows-perimiter-1 && j > 0+perimiter) && Boxsize)
            {

#ifdef debug               
cout << "#3 i: " << i << ", j: " << j << endl;
#endif             
                if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
                {
                    retractOuterFirst(i,j,cols,rows,perimiter);
                    separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
                    Boxsize++;
#ifdef debug
cout << "New Box: "; 
printBox(myFirstBox);
cout << endl;
myFirstPuzzle.printPuzzle();              
cout << "Boxsize: " << Boxsize << endl;
cout << "separator: " << separator << endl; 
#endif
                    break;
                }
                else
                {
                    j--;
                    separator=0;
                    Boxsize--;
#ifdef debug
myFirstPuzzle.printPuzzle();
#endif            
                }
            }

            while(( i > 0+perimiter+1 && j == 0+perimiter) && Boxsize)
            {
#ifdef debug                
cout << "#4 i: " << i << ", j: " << j << endl;
#endif            
                if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
                {
                    retractOuterFirst(i,j,cols,rows,perimiter);
                    separator = myFirstPuzzle.putBackIntoBox(j, i, myFirstBox);
                    Boxsize++;
#ifdef debug                    
cout << "New Box: "; 
printBox(myFirstBox);
cout << endl;
myFirstPuzzle.printPuzzle();

cout << "Boxsize: " << Boxsize << endl;
cout << "separator: " << separator << endl; 
#endif            
                    break;
                }
                else
                {
                    i--;
                    separator=0;
                    Boxsize--;
#ifdef debug                    
myFirstPuzzle.printPuzzle();
#endif                   
                }
            }
        }
    }
    myFirstPuzzle.printPuzzle();

    return myFirstPuzzle;
}

//move i and j to position before
void retractOuterFirst(int& i, int& j, int cols, int rows, int& perimiter)
{
#ifdef debug    
    cout << "retracting: i=" << i << " j=" << j << endl;
#endif
    if(( i == 0+perimiter && j <= cols-perimiter-1))
    {
        j--;
    }
    else if(( i <= rows-perimiter-1 && j == cols-perimiter-1))
    {
        i--;            
    }

    else if(( i == rows-perimiter-1 && j >= 0+perimiter))
    {
        j++;
    }

    else if(( i >= 0+perimiter+1 && j == 0+perimiter))
    {
        i++;
    }
#ifdef debug
    cout << "to: i=" << i << " j=" << j << endl;
#endif
}