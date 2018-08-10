#ifndef INTERMEDIATE_POINTS_CALCULATOR_H
#define INTERMEDIATE_POINTS_CALCULATOR_H

#pragma once
#include "vars/point3d.h"
#include "vars/intermediate_points.h"
#include "chessboard/chessboard.h"
#include "xml_reader.h"

class IntermediatePointsCalc
{
private:
    IntermediatePoints _IP; //todo: this class shouldnt have its own IP. it...
    //...should only returns calculated vars based on given vars

    Point3D calculateMarginalGamePoint(INTERMEDIATE_POINTS IP, Chessboard* pBoard);
    Point3D calculateMiddleAbovePoint(Point3D min, Point3D max);
    Point3D calculateRetreatPoint(INTERMEDIATE_POINTS IP, Point3D midAbove,
                                  Point3D retreat, Point3D max);
    Point3D calculateCournerPoint(INTERMEDIATE_POINTS IP, RealVars RV);
    Point3D calculateSafeAxisZPoint(RealVars RV);

public:
    IntermediatePointsCalc(RealVars RV, Chessboard *pBoard);

    IntermediatePoints getIntermediatePoints() const { return _IP; }
    QString dumpAllData();
};

#endif // INTERMEDIATE_POINTS_CALCULATOR_H
