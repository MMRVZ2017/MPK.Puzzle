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

}

bool AbstractionLayer_1::SetConstraintOnPosition(const coor constraintCoordinate, const AbstractionLayer_1_Properties constraint)
{

}

bool AbstractionLayer_1::RemoveConstraintOnPosition(const coor constraintCoordinate)
{

}

bool AbstractionLayer_1::CreateRandomPuzzle()
{
    for(int col=1;col<m_constraintMatrix.size()-1;col++){
        for(int row=1;row<m_constraintMatrix[col].size()-1;row++)
        {
            uint8_t tempPiece = 0b00000000;
            if(rand()%2)
                tempPiece|=0b01000000;
            else
                tempPiece|=0b10000000;



            if(rand()%2)
                tempPiece|=0b00000100;
            else
                tempPiece|=0b00001000;





            if(m_constraintMatrix[col-1][row].m_connections != 0b00000000 || m_constraintMatrix[col-1][row].m_connections != 0b11111111)

            {
                tempPiece|=(0b00110000 xor (0b00110000 & m_constraintMatrix[col-1][row].m_connections));
                //tempPiece.shift();
            }
            }
                //add shift function
            else
            {
                if(rand()%2)
                    tempPiece|=0b00000001;
                else
                    tempPiece|=0b00000010;
            }

            if(m_constraintMatrix[col-1][row].m_connections != 0b00000000 || m_constraintMatrix[col-1][row].m_connections != 0b11111111)
                tempPiece |= (0b00110000 xor (0b00110000 & m_constraintMatrix[col-1][row].m_connections));
            else
            {
                if(rand()%2)
                    tempPiece |= 0b00010000;
                else
                    tempPiece |= 0b00100000;
            }

        }

    //insert temppuzzlepiece into matrix
    }

}

void AbstractionLayer_1::setEdgeZero()
{
    for(int col=0;col<m_constraintMatrix.size();col++)
        for(int row=0;row<m_constraintMatrix[col].size();row++)
            if(col ==0 || col == m_constraintMatrix.size() || row == 0 || row == m_constraintMatrix[col].size())
                m_constraintMatrix[col][row].m_connections=0b00000000;
}

