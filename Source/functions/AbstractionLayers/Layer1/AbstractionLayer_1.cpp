//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include "../../../header.h"

#include <iostream>
#include <bitset>

void AbstractionLayer_1::PreProcessing(coor mySize,  const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(mySize.col+2, mySize.row+2);
    setEdgeZero();
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool AbstractionLayer_1::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(auto it = qVector.begin(); it != qVector.end(); it++)
    {
        if(PlaceOfPartGood(constraintCoordinate, it->first->m_a1.m_connections))
            continue;
        qVector.erase(it++);
    }
}

bool AbstractionLayer_1::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].m_connections=constraint.m_connections;
}

bool AbstractionLayer_1::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row].m_connections=0b11111111;
}

void AbstractionLayer_1::CreateRandomPuzzle()
{
    std::minstd_rand simple_rand;
    simple_rand.seed((unsigned int)"dumbo");

    for(int row = 1; row < m_constraintMatrix.size()-1; row++){
        for(int col = 1; col < (m_constraintMatrix[row].size() - 1);)
        {
            //create random piece
            uint8_t tempPiece = 0b00000000;
            if(simple_rand()%2)
                tempPiece|=0b01000000;
            else
                tempPiece|=0b10000000;

            if(simple_rand()%2)
                tempPiece|=0b00010000;
            else
                tempPiece|=0b00100000;

            if(simple_rand()%2)
                tempPiece|=0b00000100;
            else
                tempPiece|=0b00001000;

            if(simple_rand()%2)
                tempPiece|=0b00000001;
            else
                tempPiece|=0b00000010;

            //set edges and corners to 00
            if(row == 1)
                tempPiece and_eq (uint8_t)0b00111111;
            if(row == (m_constraintMatrix.size() - 2))
                tempPiece and_eq (uint8_t)0b11110011;
            if(col == 1)
                tempPiece and_eq (uint8_t)0b11111100;
            if(col == (m_constraintMatrix[row].size() - 2))
                tempPiece and_eq (uint8_t)0b11001111;

            //set piece if piece good
            if(PlaceOfPartGood(coor(col,row),tempPiece))
            {
                m_constraintMatrix[row][col].m_connections = tempPiece;
                col++;
            }
        }
    }

}



//puts all pieces of the current constraint matrix into a puzzlebox
qualityVector AbstractionLayer_1::returnInBox(vector<Part>& PuzzleBox)
{
    if(!(PuzzleBox.size()))
        for(int i = 0; i< (m_constraintMatrix.size()-2)*(m_constraintMatrix[0].size()-2);i++)
            PuzzleBox.emplace_back(Part());

    int i=0;
    for(int col=1;col<m_constraintMatrix.size()-1;col++)
        for(int row=1;row<m_constraintMatrix[col].size()-1;row++)
            PuzzleBox[i++].m_a1.m_connections=m_constraintMatrix[col][row].m_connections;

}

void AbstractionLayer_1::printConstraintMatrix() {
    for (auto it1:m_constraintMatrix) {
        for (auto it2:it1)
            std::cout << std::bitset<8>(it2.m_connections) << " ";
        std::cout << std::endl;
    }
    cout.flush();
}
void AbstractionLayer_1::setEdgeZero()
{
    for(int row = 0; row < m_constraintMatrix.size(); row++)
        for(int col = 0; col < m_constraintMatrix[row].size(); col++)
            if(col == 0 || col == (m_constraintMatrix[row].size() - 1) || row == 0 || row == (m_constraintMatrix.size() - 1))
                m_constraintMatrix[row][col].m_connections=0b00000000;
}

//checks if the myPart in its current orientation is legal in position m, n
bool AbstractionLayer_1::PlaceOfPartGood(coor myCoor, uint8_t& myPart)
{

    uint8_t negativePart=0b00000000;

    negativePart or_eq (m_constraintMatrix[myCoor.row+1][myCoor.col].m_connections & 0b11000000);
    negativePart or_eq (m_constraintMatrix[myCoor.row][myCoor.col-1].m_connections & 0b00110000);
    negativePart or_eq (m_constraintMatrix[myCoor.row-1][myCoor.col].m_connections & 0b00001100);
    negativePart or_eq (m_constraintMatrix[myCoor.row][myCoor.col+1].m_connections & 0b00000011);
    shift(negativePart,2);
    if  (
            (    ((((negativePart & 0b11000000) ^ (myPart & 0b11000000))  != 0b00000000) && (((myPart & 0b11000000) != 0b00000000) && (negativePart & 0b11000000) != 0b00000000))
                 || ((((negativePart & 0b11000000) == 0b11000000) || ((myPart &  0b11000000) == 0b11000000))  && (((myPart & 0b11000000) != 0b00000000) && (negativePart & 0b11000000) != 0b00000000))
                 ||  (((negativePart & 0b11000000) == 0b00000000) && ((myPart &  0b11000000) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00110000) ^ (myPart & 0b00110000))  != 0b00000000) && (((myPart & 0b00110000) != 0b00000000) && (negativePart & 0b00110000) != 0b00000000))
                 || ((((negativePart & 0b00110000) == 0b00110000) || ((myPart &  0b00110000) == 0b00110000))  && (((myPart & 0b00110000) != 0b00000000) && (negativePart & 0b00110000) != 0b00000000))
                 ||  (((negativePart & 0b00110000) == 0b00000000) && ((myPart &  0b00110000) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00001100) ^  (myPart & 0b00001100))  != 0b00000000) && (((myPart & 0b00001100) != 0b00000000) && (negativePart & 0b00001100) != 0b00000000))
                 || ((((negativePart & 0b00001100) == 0b00001100) || ((myPart &  0b00001100) == 0b00001100))  && (((myPart & 0b00001100) != 0b00000000) && (negativePart & 0b00001100) != 0b00000000))
                 ||  (((negativePart & 0b00001100) == 0b00000000) && ((myPart &  0b00001100) == 0b00000000))  )
            &&
            (    ((((negativePart & 0b00000011) ^  (myPart & 0b00000011))  != 0b00000000)&& (((myPart & 0b00000011) != 0b00000000) && (negativePart & 0b00000011) != 0b00000000))
                 || ((((negativePart & 0b00000011) == 0b00000011) || ((myPart &  0b00000011) == 0b00000011))  && (((myPart & 0b00000011) != 0b00000000) && (negativePart & 0b00000011) != 0b00000000))
                 ||  (((negativePart & 0b00000011) == 0b00000000) && ((myPart &  0b00000011) == 0b00000000))  )
            )
        return true;
    return false;

}

void AbstractionLayer_1::shift(uint8_t& Part, int shifts)
{
    Part = Part >> (shifts*2) | Part << sizeof(uint8_t)*8 - (shifts*2);
}

void AbstractionLayer_1_Properties::shift(int shifts)
{
    this->m_connections = this->m_connections >> (shifts*2) | this->m_connections << sizeof(uint8_t)*8 - (shifts*2);
}

void AbstractionLayer_1_Properties::print()
{
    std::cout << std::bitset<8>(this->m_connections);
}

