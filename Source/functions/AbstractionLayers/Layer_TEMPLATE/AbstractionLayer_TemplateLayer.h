
#ifndef SOURCE_ABSTRACTIONLAYER_TemplateLayer_H     //%TODO: Change include guards (Replace TEMPLATELAYER)
#define SOURCE_ABSTRACTIONLAYER_TemplateLayer_H
                                                    //TODO: Replace every occurrence of "TemplateLayer" in this file with Layer Name

#include "AbstractionLayer_TemplateLayer.h"
#include "../AbstraktionLayer_Base.h"

                                                    //TODO: Check which includes are necessary for your layer
#include <vector>
#include <iostream>
#include <cmath>
//#include <utility>
//#include <bitset>
//#include <random>
//#include <cstdio>
//#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"





#ifdef _WIN32
#define PATH "..\\..\\..\\pieces\\%04d.jpg"
#elif defined __unix__
#define PATH "..//..//..//pieces//%04d.jpg"
#elif defined __APPLE__
    #define PATH "..//..//..//pieces//%04d.jpg"
#endif


using namespace std;

class AbstractionLayer_TemplateLayer : public AbstractionLayer_Base<AbstractionLayer_TemplateLayer_Properties>
{
public:                                                                             //TODO: implement at least these functions ind AbstractionLayer_TemplateLayer.cpp
    bool PreProcessing(coor mySize, const vector<Part*>* partArray) ;
    bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector);
    bool SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_TemplateLayer_Properties constraint);
    bool RemoveConstraintOnPosition(const coor constraintCoordinate);

//--------------------implemented in base class----------------------
//
//    void InitialiseConstraintMatrixSize(const int32_t collumns, const int32_t rows)  //TODO: overload function if special initialisation of ConstraintMatrix is needed
//    {
//        m_constraintMatrix = vector<vector<T>>(collumns, vector<T>(rows));
//    }

//    vector<vector<T>> m_constraintMatrix{}; //<-- Matrix where the constraints of the layer will be saved
//--------------------------------------------------------------------


private:
};




#endif //SOURCE_ABSTRACTIONLAYER_TemplateLayer_H
