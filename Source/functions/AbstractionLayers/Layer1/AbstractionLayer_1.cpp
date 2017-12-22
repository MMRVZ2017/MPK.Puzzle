//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include "../../../header.h"

#include <iostream>

void AbstractionLayer_1::PreProcessing(const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(32+2, 28+2);
    setEdgeZero();
}

//it through qualityVector and removes all that do not trigger PlaceOfPartGood
bool AbstractionLayer_1::EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(auto it = qVector.begin(); it != qVector.end(); it++)
    {
        if(PlaceOfPartGood(constraintCoordinate, it->first->m_test1.m_connections))
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

bool AbstractionLayer_1::CreateRandomPuzzle()
{
    std::minstd_rand simple_rand;
    simple_rand.seed((unsigned int)"dumbo");

    for(int col = 1; col < m_constraintMatrix.size()-1; col++){
        for(int row = 1; row < (m_constraintMatrix[col].size() - 1);)
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
            if(row==1)
                tempPiece and_eq (uint8_t)0b00111111;
            if(row==m_constraintMatrix[col].size())
                tempPiece and_eq (uint8_t)0b11110011;
            if(col==1)
                tempPiece and_eq (uint8_t)0b11111100;
            if(col==m_constraintMatrix.size()-1)
                tempPiece and_eq (uint8_t)0b11001111;

            //set piece if piece good
            if(PlaceOfPartGood(coor(col,row),tempPiece))
            {
                row++; // Sollte er hier nicht das Puzzleteil irgendwo abspeichern? Weiß aber nicht ob die constraint layer der richtige platz ist, oder nicht eine neue, bzw. die puzzlebox
                m_constraintMatrix[col][row].m_connections = tempPiece;
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
            PuzzleBox[i++].m_test1.m_connections=m_constraintMatrix[col][row].m_connections;

}

void AbstractionLayer_1::setEdgeZero()
{
    for(int col = 0; col < m_constraintMatrix.size(); col++)
        for(int row = 0; row < m_constraintMatrix[col].size(); row++)
            if(col == 0 || col == (m_constraintMatrix.size() - 1) || row == 0 || row == (m_constraintMatrix[col].size() - 1))
                m_constraintMatrix[col][row].m_connections=0b00000000;
}

//checks if the myPart in its current orientation is legal in position m, n
bool AbstractionLayer_1::PlaceOfPartGood(coor myCoor, uint8_t& myPart)
{

    uint8_t negativePart=0b00000000;

    negativePart or_eq (m_constraintMatrix[myCoor.col][myCoor.row+1].m_connections & 0b11000000);
    negativePart or_eq (m_constraintMatrix[myCoor.col-1][myCoor.row].m_connections & 0b00110000);
    negativePart or_eq (m_constraintMatrix[myCoor.col][myCoor.row-1].m_connections & 0b00001100);
    negativePart or_eq (m_constraintMatrix[myCoor.col+1][myCoor.row].m_connections & 0b00000011);
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

