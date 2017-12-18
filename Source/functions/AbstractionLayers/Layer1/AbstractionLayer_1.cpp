//
// Created by mpapa on 05.12.2017.
//

#include "AbstractionLayer_1.h"
#include <iostream>

void AbstractionLayer_1::PreProcessing(const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(32+2, 28+2);
    setEdgeZero();
}

bool AbstractionLayer_1::EvalueteQuality (const coor constraintCoordinate, qualityVector& qVector)
{
    for(int i=0;i<qVector.size();i++)
    {
        if(PlaceOfPartGood(constraintCoordinate, qVector[i].Part))
            qVector[i].setQuality(1);
        else
            qVector[i].setQuality(0);
    }
}

bool AbstractionLayer_1::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{
    m_constraintMatrix[constraintCoordinate.col][constraintCoordinate.row]=
}

bool AbstractionLayer_1::RemoveConstraintOnPosition(const coor constraintCoordinate)
{

}

bool AbstractionLayer_1::CreateRandomPuzzle()
{
    for(int col=1;col<m_constraintMatrix.size()-1;col++){
        for(int row=1;row<m_constraintMatrix[col].size()-1;)
        {
            uint8_t tempPiece = 0b00000000;
            if(rand()%2)
                tempPiece|=0b01000000;
            else
                tempPiece|=0b10000000;

            if(rand()%2)
                tempPiece|=0b00010000;
            else
                tempPiece|=0b00100000;

            if(rand()%2)
                tempPiece|=0b00000100;
            else
                tempPiece|=0b00001000;

            if(rand()%2)
                tempPiece|=0b00000001;
            else
                tempPiece|=0b00000010;

            if(PlaceOfPartGood(coor(col,row),tempPiece))
                row++;
        }
    }

}

void AbstractionLayer_1::setEdgeZero()
{
    for(int col=0;col<m_constraintMatrix.size();col++)
        for(int row=0;row<m_constraintMatrix[col].size();row++)
            if(col ==0 || col == m_constraintMatrix.size() || row == 0 || row == m_constraintMatrix[col].size())
                m_constraintMatrix[col][row].m_connections=0b00000000;
}

//checks if the myPart in its current orientation is legal in position m, n
bool AbstractionLayer_1::PlaceOfPartGood(coor myCoor, uint8_t& myPart)
{

    uint8_t negativePart(0)=0b00000000;

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

