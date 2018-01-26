#include "AbstractionLayer_SURFFeatures.h"

bool AbstractionLayer_SURFFeatures::PreProcessing(coor mySize, const vector<Part*>* partArray)
{
    InitialiseConstraintMatrixSize(mySize.col+2, mySize.row+2);

    //TODO: Gesamtbild mit OpenCV einlesen
    //TODO: Gesamtbild anhand der berechneten Spalten und Zeilen auseinander schneiden (Sind in der puzzleKlasse gespeichert)
    //TODO: Features der einzelnen Felder des ausgeschnittenen Gesamtbildes in der m_constraintMatrix speichern

    // Speichert die Features der linken oberen Ecke des Gesamtpuzzles in die constraintMatrix
    m_constraintMatrix[0][0].m_numberOfFeaturesDetected = 50;

    //TODO: Alle Bilder mit OpenCV öffnen und deren erkannten Features in SURFFeature_Properties der Part-Klasse speichern
    // Speichert die erkannten Features des jeweiligen Bilds im partArray an der Stelle (->at(xxx))
    partArray->at(0)->m_a4.m_numberOfFeaturesDetected = 40;
}

bool AbstractionLayer_SURFFeatures::EvaluateQuality (coor constraintCoordinate, qualityVector& qVector)
{
    //TODO: Vergleichen, welche der in qualityVector erhaltenen ähnlich viele Features besitzen, wie an der jeweiligen constraintCoordinate in der m_constraintMatrix gespeichert sind
}

bool AbstractionLayer_SURFFeatures::SetConstraintOnPosition(const coor constraintCoordinate,const AbstractionLayer_SURFFeatures_Properties constraint)
{
    //TODO: Benötigen wir nicht unbedint.
    //TODO: Hier erhalten wir vom Dispatcher welches Teil an welche Position gesetzt wird und wir könnten hier die Features des Bilds in die m_constraintMatrix speichern
}

bool AbstractionLayer_SURFFeatures::RemoveConstraintOnPosition(const coor constraintCoordinate)
{
    //TODO: Wie auch beim SetConstraint sollte uns das hier nicht wirklich interessieren.
    //TODO: Außer wir setzen etwas in die Contraintmatrix.
    //TODO: Dann ruft uns der Dispatcher beim Backtrack hier auf und wir müssten das jeweilige PuzzlePart hier wieder rauslöschen.
}