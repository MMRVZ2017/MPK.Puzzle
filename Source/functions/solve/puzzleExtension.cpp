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
                //TODO! add all other layerswith their rotaionvariance here
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
void Puzzle::setConstraints(coor setConstraints, Part* constraintPiece)
{
    //dp
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.clear();
    for(auto it:this->tmp_destructionArray)
    this->dp.m_constraintMatrix[setConstraints.col][setConstraints.row].DestructionArray.emplace_back(it);

    //a1
    this->a1.SetConstraintOnPosition(setConstraints,constraintPiece->m_a1);
}

void Puzzle::createRandomPuzzle()
{
    a1.CreateRandomPuzzle();
}

void Puzzle::createp_box()
{
    for(int i=0;i<cols*rows*4;i++)
    p_myBox.push_back(&myBox[i]);
}

//creates a box of puzzlepieces with nothing other than puzzle piece id and correct nr of pieces
void Puzzle::createBox(){
    for(int i=0;i<cols*rows;i++)
    {
        Part temp;
        temp.SetPartID(i);
        for(uint8_t j=0;j<4;j++)
        {
            temp.SetNumOfRotations(j);
            myBox.push_back(temp);
        }


    }

}

bool Puzzle::allSet() {
    for(auto it:myBox)
        if(!it.set)
            return false;
    return true;
}

void Puzzle::clearMat()
{
    for(unsigned int i=0;i<cols;i++)
    {
        for(unsigned int j=0;j<rows;j++)
        {
            this->removeConstrains({i,j});
        }
    }
}
