//Raphael Maenle - 11.11.2017
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

#include "puzzle.h"
//#include "Solver.h"

using namespace std;

void numerateBox(vector<PuzzlePiece>& myBox);
Puzzle solveOuterFirst( unsigned int rows, unsigned int cols, vector<PuzzlePiece>& myFirstBox);
void retractOuterFirst(int& i, int& j, int cols, int rows, int perimiter);

int main()
{
	srand(time(0));

	//Puzzle myFirstPuzzle(3,4);

	int rows = 2, cols = 3;

	vector<PuzzlePiece> myFirstBox = createBox(cols, rows);
	numerateBox(myFirstBox);
	cout << "now in solve: " << endl << endl;
	solveOuterFirst(rows,cols,myFirstBox);
}

//set box identifiers
void numerateBox(vector<PuzzlePiece>& myBox)
{
	for(int i = 0; i< myBox.size();i++)
		myBox[i].setBoxIdentifier(i);

	return;
}

//goes round and round puzzle and tries all pieces
Puzzle solveOuterFirst( unsigned int rows, unsigned int cols, vector<PuzzlePiece>& myFirstBox)
{
	int i = 0, j= 0, Boxsize = rows*cols, separator=0;
	Puzzle myFirstPuzzle(cols, rows);
	//first Piece
	cout << "#0 i: " << i << ", j: " << j << endl;

	if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
	{
		cout << "error at first piece" << endl;
	}
	myFirstPuzzle.printPuzzle();
/*
	Boxsize--;

	//rotate through puzzle
	for(int perimiter = 0; perimiter <= (cols>rows?rows:cols)/2;)
	{
		cout << "perimiter: " << perimiter << endl;
		while(( i == 0+perimiter && j < cols-perimiter-1) && Boxsize)
		{
			j++;
			Boxsize--;
			cout << "#1 i: " << i << ", j: " << j << endl;

			if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
			{
				retractOuterFirst(i,j,cols,rows,perimiter);
				Boxsize++;
				break;
			}
			else
			{
				Boxsize--;
			}
		}

		while(( i < rows-perimiter-1 && j == cols-perimiter-1) && Boxsize)
		{
			i++;
			Boxsize--;
			cout << "#2 i: " << i << ", j: " << j << endl;
			if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
			{
				retractOuterFirst(i,j,cols,rows,perimiter);
				Boxsize++;
				break;
			}
			else
			{
				Boxsize--;
			}
		}

		while(( i == rows-perimiter-1 && j > 0+perimiter) && Boxsize)
		{
			j--;
			Boxsize--;
			cout << "#3 i: " << i << ", j: " << j << endl;
			if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
			{
				retractOuterFirst(i,j,cols,rows,perimiter);
				Boxsize++;
				break;
			}
			else
			{
				Boxsize--;
			}
		}

		while(( i > 0+perimiter+1 && j == 0+perimiter) && Boxsize)
		{
			i--;
			cout << "#4 i: " << i << ", j: " << j << endl;
			if(myFirstPuzzle.tryAllPieces(j, i, myFirstBox,separator) == -1)
			{
				retractOuterFirst(i,j,cols,rows,perimiter);
				Boxsize++;
				break;
			}
			else
			{
				Boxsize--;
				perimiter++;
			}
		}
	}
	myFirstPuzzle.printPuzzle();
*/	
	return myFirstPuzzle;
}

//move i and j to position before
void retractOuterFirst(int& i, int& j, int cols, int rows, int perimiter)
{

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

}

