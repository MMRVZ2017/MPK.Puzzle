#ifndef SOURCE_ABSTRAKTIONLAYER_BASE_H
#define SOURCE_ABSTRAKTIONLAYER_BASE_H

#include <map>
#include <vector>
#include "../../header/input.h"

using namespace std;

typedef map<Part*, float> qualityVector;

/*
 * Die Logik mit der Template-Basisklasse und den abgeleiteten Layern kam mit der Idee, dass die Layer
 * nicht auf die Layer-Eigenschaften der anderen Layer zugreifen können, da die mit friend geschützt sind.
 * Ansonsten könnte man auch verschiedene Objekte der Template Basisklasse erstellen
 */
/**
 * @brief       template base class for all different layers
 * @tparam T    template parameter which should be the property class of the layer
 */
template<typename T>
class AbstractionLayer_Base
{
public:
    /**
     * @brief   pure virtual method for the pre processing of the layer
     * @param   [in] partArray - References of all Parts, in which the properties of the Layer will be written
     */
    virtual bool PreProcessing(coor mySize, const vector<Part*>* partArray) = 0;

    /**
     * @brief   pure virtual method for the quality evaluation of the layer
     * @param   [in] constraintCoordinate - Coordinate where the quality should evaluate for each given part
     * @param   [in/out] qualityVector - References of all parts with the quality of their quality to fit in the given coordinate
     * @return  Boolean if the quality was calculated right or not
     */
    virtual bool EvaluateQuality (const coor constraintCoordinate, qualityVector& qVector) = 0;

    /**
     * @brief   pure virtual method which sets the constraint on the given Coordinate in the m_constraintMatrix
     *          This function will be called from main for all layers
     * @param   [in] constraintCoordinate - Coordinate where the constraint should be placed
     * @param   [in] constraint - constraint which should be placed
     * @return  Boolean if the constraint was set or a failure happened
     */
    virtual bool SetConstraintOnPosition(const coor constraintCoordinate, const T constraint) = 0;

    /**
     * @brief   pure virtual method which remove the constraint on the given Coordinate of the m_constraintMatrix
     *          This function will be called from main for all layers
     * @param   [in] constraintCoordinate - Coordinate where the constraint should be removed
     * @return  Boolean if the remove was successfull or failed
     */
    virtual bool RemoveConstraintOnPosition(const coor constraintCoordinate) = 0;

    /**
     * @brief   virtual method to initialise the m_constraintMatrix to a given size
     * @param   [in] collumns - Wished collumns of the m_constraintMatrix
     * @param   [in] rows - Wished rows of the m_constraintMatrix
     */
    void InitialiseConstraintMatrixSize(const int32_t collumns, const int32_t rows)
    {
        m_constraintMatrix = vector<vector<T>>(collumns, vector<T>(rows));
    }

    vector<vector<T>> m_constraintMatrix{}; //!<-- Matrix where the constraints of the layer will be saved
};


#endif //SOURCE_ABSTRAKTIONLAYER_BASE_H
