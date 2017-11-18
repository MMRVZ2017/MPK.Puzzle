//Raphael Maenle - 11.11.2017

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm> 

using namespace std;

class PuzzlePiece
{
public:

	unsigned char Ringbuffer;

	PuzzlePiece(unsigned int flag = 0)
	{
		shifts=0;
		boxidentifier=-1;
		switch(flag)
		{
			case 0:
			Ringbuffer=0b00000000;
			break;
			case 1:
			Ringbuffer=0b11111111;
			break;
			case 3:
			randomCenterPiece();
			break;
		}

	}

	void Shift(unsigned int moves)
	{
		//Shift ringbuffer to the right n number of times
		shifts = (shifts+moves)%4;
		Ringbuffer = ((Ringbuffer >> (moves*2)) | (Ringbuffer << sizeof(unsigned char)*8 - (moves*2)));		
	}

	void printPiece()
	{
		cout << bitset<sizeof(unsigned char)*8> (Ringbuffer);
	}
	//makes piece to random center piece
	void randomCenterPiece()
	{
		this->Ringbuffer= 0b00000000;

		if(rand()%2)
			this->Ringbuffer |= 0b01000000;
		else
			this->Ringbuffer |= 0b10000000;

		if(rand()%2)
			this->Ringbuffer |= 0b00010000;
		else
			this->Ringbuffer |= 0b00100000;

		if(rand()%2)
			this->Ringbuffer |= 0b00000100;
		else
			this->Ringbuffer |= 0b00001000;

		if(rand()%2)
			this->Ringbuffer |= 0b00000001;
		else
			this->Ringbuffer |= 0b00000010;
	}

	void assignIdentifier()
	{
		identifier = idcount;
		idcount++;
	}

	unsigned int getIdentifier()
	{
		return identifier;
	}	
	void setBoxIdentifier(int new_boxid)
	{
		boxidentifier = new_boxid;
	}	
	int getBoxIdentifier()
	{
		return boxidentifier;
	}	
private:
	unsigned int shifts;
	unsigned int identifier;
	int boxidentifier;
	static unsigned int idcount;


};

unsigned int PuzzlePiece::idcount(0);

//saves a matrix cluster of puzzlepieces
class Puzzle
{
public:
	Puzzle(uint m = 7, uint n = 4): col(m), row(n)
	{
		Matrix = new PuzzlePiece* [n+2];
		//Box = new PuzzlePiece[n*m];
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
				/*else
					Matrix[i][j] = PuzzlePiece(1);*/
			}

		}
	}
	//this adds new piece end excludes the 0 bounds around the area
	bool setPiece(uint m,uint n,PuzzlePiece newPiece)
	{
		if(this->PlaceOfPartGood(m,n,newPiece))
		{
			Matrix[n+1][m+1] = newPiece; 	
			return 1;
		}
		return 0;
	}
	bool removePiece(uint m,uint n)
	{
	
		Matrix[n+1][m+1] = 0b11111111; 	
		return 1;

	}

	PuzzlePiece getPiece(uint m,uint n)
	{
		return Matrix[n+1][m+1];
	}
	PuzzlePiece sudogetPiece(uint m,uint n)
	{
		return Matrix[n][m];
	}

	//check if place of the part in the matrix is correct
	bool PlaceOfPartGood(unsigned int m, unsigned int n, PuzzlePiece& myPart);
	bool testRotationPiece(unsigned int m, unsigned int n, PuzzlePiece& myPart);
	unsigned int tryAllPieces(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox, unsigned int separator);
	unsigned int putBackIntoBox(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox);

	void printPuzzle();

	uint getCols()
	{
		return col;
	}

	uint getRows()
	{
		return row;
	}
	//creates a random puzzle of size m columns, n rows
	void createRandomPuzzle();
	//shuffles Box;
	vector<PuzzlePiece> Shuffle();
	//prints box
	void printBox();

private:
	uint row;
	uint col;

	PuzzlePiece** Matrix;
	vector<PuzzlePiece> Box;

	void sudosetPiece(uint m,uint n,PuzzlePiece newPiece)
	{
		if(this->PlaceOfPartGood(m,n,newPiece))
			Matrix[n][m] = newPiece; 	
	}
	void sudoprintPuzzle()
	{
		for(int i=0;i<row+2;i++)
		{
			for(int j=0;j<col+2;j++)
			{
				Matrix[i][j].printPiece();
				cout << "  ";
			}
			cout << endl;
		}
	}
};

//checks if piece fits into m n position in matrix;
bool Puzzle::PlaceOfPartGood(unsigned int m,unsigned int n, PuzzlePiece& myPart)
{

	PuzzlePiece tmpPuzzlePiece = myPart;


	PuzzlePiece negativePart(0);

	negativePart.Ringbuffer = negativePart.Ringbuffer | (getPiece(m,n+1).Ringbuffer & 0b11000000);
	negativePart.Ringbuffer = negativePart.Ringbuffer | (getPiece(m-1,n).Ringbuffer & 0b00110000);
	negativePart.Ringbuffer = negativePart.Ringbuffer | (getPiece(m,n-1).Ringbuffer & 0b00001100);
	negativePart.Ringbuffer = negativePart.Ringbuffer | (getPiece(m+1,n).Ringbuffer & 0b00000011);

	negativePart.Shift(2);
/*		cout << "negativePart: ";
	negativePart.printPiece();
	unsigned char topPart 	= ((negativePart.Ringbuffer & 0b11000000) ^ (tmpPuzzlePiece.Ringbuffer & 0b11000000));
	cout << "topPart:      " << bitset<sizeof(unsigned char)*8> (topPart) << endl;

	unsigned char rightPart = ((negativePart.Ringbuffer & 0b00110000) ^ (tmpPuzzlePiece.Ringbuffer & 0b00110000));
	cout << "rightPart:    " << bitset<sizeof(unsigned char)*8> (rightPart) << endl;

	unsigned char lowPart 	= ((negativePart.Ringbuffer & 0b00001100) ^ (tmpPuzzlePiece.Ringbuffer & 0b00001100));
	cout << "lowPart:      " << bitset<sizeof(unsigned char)*8> (lowPart) << endl;

	unsigned char leftPart	= ((negativePart.Ringbuffer & 0b00000011) ^ (tmpPuzzlePiece.Ringbuffer & 0b00000011));
	cout << "leftPart:     " << bitset<sizeof(unsigned char)*8> (leftPart) << endl;
*/
	
	if  (	//simplify
		  (    ((((negativePart.Ringbuffer & 0b11000000) ^  (tmpPuzzlePiece.Ringbuffer & 0b11000000))  != 0b00000000)                  && (tmpPuzzlePiece.Ringbuffer & 0b11000000) != 0b00000000)
			|| ((((negativePart.Ringbuffer & 0b11000000) == 0b11000000) || ((tmpPuzzlePiece.Ringbuffer &  0b11000000) == 0b11000000))  && (tmpPuzzlePiece.Ringbuffer & 0b11000000) != 0b00000000)
			||  (((negativePart.Ringbuffer & 0b11000000) == 0b00000000) && ((tmpPuzzlePiece.Ringbuffer &  0b11000000) == 0b00000000))  ) 
		&&
		  (    ((((negativePart.Ringbuffer & 0b00110000) ^ (tmpPuzzlePiece.Ringbuffer & 0b00110000))  != 0b00000000)                   && (tmpPuzzlePiece.Ringbuffer & 0b001100) != 0b00000000) 
			|| ((((negativePart.Ringbuffer & 0b00110000) == 0b00110000) || ((tmpPuzzlePiece.Ringbuffer &  0b00110000) == 0b00110000))  && (tmpPuzzlePiece.Ringbuffer & 0b00110000) != 0b00000000)
			||  (((negativePart.Ringbuffer & 0b00110000) == 0b00000000) && ((tmpPuzzlePiece.Ringbuffer &  0b00110000) == 0b00000000))  ) 
		&&
		  (    ((((negativePart.Ringbuffer & 0b00001100) ^  (tmpPuzzlePiece.Ringbuffer & 0b00001100))  != 0b00000000)                  && (tmpPuzzlePiece.Ringbuffer & 0b00001100) != 0b00000000) 
			|| ((((negativePart.Ringbuffer & 0b00001100) == 0b00001100) || ((tmpPuzzlePiece.Ringbuffer &  0b00001100) == 0b00001100))  && (tmpPuzzlePiece.Ringbuffer & 0b00001100) != 0b00000000)
			||  (((negativePart.Ringbuffer & 0b00001100) == 0b00000000) && ((tmpPuzzlePiece.Ringbuffer &  0b00001100) == 0b00000000))  ) 
		&&
		  (    ((((negativePart.Ringbuffer & 0b00000011) ^  (tmpPuzzlePiece.Ringbuffer & 0b00000011))  != 0b00000000)                  && (tmpPuzzlePiece.Ringbuffer & 0b00000011) != 0b00000000)
			|| ((((negativePart.Ringbuffer & 0b00000011) == 0b00000011) || ((tmpPuzzlePiece.Ringbuffer &  0b00000011) == 0b00000011))  && (tmpPuzzlePiece.Ringbuffer & 0b00000011) != 0b00000000)
			||  (((negativePart.Ringbuffer & 0b00000011) == 0b00000000) && ((tmpPuzzlePiece.Ringbuffer &  0b00000011) == 0b00000000))  )    
		)

		return 1;


	return 0;
}

void Puzzle::printPuzzle()
{
	for(int i=1;i<row+1;i++)
	{
		for(int j=1;j<col+1;j++)
		{
			Matrix[i][j].printPiece();
			cout << "  ";
		}
		cout << endl;
	}
}

void Puzzle::createRandomPuzzle()
{
	PuzzlePiece temporaryRandomPiece(0);
	for(int i=0;i<getRows();i++)
	{
		for(int j = 0; j < getCols();)
		{
			//create random piece, set edges according to position and check if piece is good

			temporaryRandomPiece.randomCenterPiece();
			if(i==0)
				temporaryRandomPiece.Ringbuffer &= 0b00111111;	
			if(i==getRows()-1)
				temporaryRandomPiece.Ringbuffer &= 0b11110011;
			if(j==0)
				temporaryRandomPiece.Ringbuffer &= 0b11111100;
			if(j==getCols()-1)
				temporaryRandomPiece.Ringbuffer &= 0b11001111;

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

void Puzzle::printBox()
{
	Shuffle();
	for (vector<PuzzlePiece>::iterator i = Box.begin(); i != Box.end(); i++)	
	{
		(*i).printPiece();
		cout << ' ';
	}		
	cout << endl;
}

vector<PuzzlePiece> Puzzle::Shuffle()
{
	random_shuffle(Box.begin(),Box.end());
	return Box;
}

bool Puzzle::testRotationPiece(unsigned int m, unsigned int n, PuzzlePiece& myPart)
{
	for(int rotation=0; rotation < 4; rotation++)
	{
  		myPart.Shift(1);
  		myPart.printPiece();
  		cout << endl;
		if(PlaceOfPartGood(m,n, myPart))
			{
			
			return 1;
			}
	}
	return 0;
}

//tries all pieces in box from separator to end and places fitting into matrix. removes fitting piece
//use separator if you have to retract to a position
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

//insterts piece at position in box according to boxidentifier and removes piece from puzzle
//this returns the position the puzzle piece was put back in! not the boxidentifier of the piece. look that up in other function.
unsigned int Puzzle::putBackIntoBox(unsigned int m, unsigned int n, vector<PuzzlePiece>& myBox)
{
	for(int i = 0; i < myBox.size();i++)
	{
		if(myBox[i].getBoxIdentifier()>getPiece(m,n).getBoxIdentifier())
			{
				myBox.insert(myBox.begin()+i,getPiece(m,n));
				removePiece(m,n);
				return i;
			}
	}
}

vector<PuzzlePiece> createBox(uint m, uint n)
{
	Puzzle myFirstPuzzle(m,n); 
	myFirstPuzzle.createRandomPuzzle(); 
	return myFirstPuzzle.Shuffle(); 
}
