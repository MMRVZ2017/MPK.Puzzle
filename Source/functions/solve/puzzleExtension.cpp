//
// Created by Raphael Maenle on 21/12/2017.
//

#include "../../header/solve.h"


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

            tmpPart.m_a1=this->a1.m_constraintMatrix[i][j];
            //sets part id
            tmpPart.SetPartID(id++);
            // adds all 4 rotations to Box
            for(int rotations=0;rotations<4;rotations++)
            {
                tmpPart.m_a1.shift(1);
                //TODO! add all other layers and rotationvariance here
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

void Puzzle::createRandomPuzzle()
{
    a1.CreateRandomPuzzle();
}

void Puzzle::createp_box()
{

}
