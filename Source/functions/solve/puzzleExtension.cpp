//
// Created by Raphael Maenle on 21/12/2017.
//

#include "../../header/solve.h"


void Puzzle::printPuzzle() {}
void Puzzle::putIntoBox() {}//puts a puzzlepiece back into its box

//shuffles the existing box in Puzzle
void Puzzle::shuffle()
{
    random_shuffle(myBox.begin(),myBox.end());
    for (auto &i:myBox)
    {
        //i.myLayers->m_test1
        //shift(rand()%4,i);
        //i.resetShift();
    }
}


void Puzzle::removeConstrains(coor removeCoordinates) {}//deletes all constraints from all abstractionlayers

void Puzzle::createRandomPuzzle()
{
    a1->CreateRandomPuzzle();
}