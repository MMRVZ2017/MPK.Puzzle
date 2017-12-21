//
// Created by Raphael Maenle on 21/12/2017.
//

#include "../../header/solve.h"


void Puzzle::printPuzzle()
{
    //print every layer individually for now and later make complete visual
}

//puts a puzzlepiece back into its box
void Puzzle::putIntoBox()
{
    Part tmpPart;
    for(int i=0;i<this->getSizeAsCoor().col;i++)
    {
        for(int j=0;j<this->getSizeAsCoor().row;j++)
        {
            //TODO! add rotation of all parts
            //TODO! add id to every part (how)
            tmpPart.m_test1=this->a1->m_constraintMatrix[i][j];
            //TODO! add all other layers here
            myBox.emplace_back(tmpPart);
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
    this->a1->RemoveConstraintOnPosition(removeCoordinates);
}

void Puzzle::createRandomPuzzle()
{
    a1->CreateRandomPuzzle();
}

void Puzzle::createp_box()
{
}
