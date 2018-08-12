#include "intermediate_points_calculator.h"

/*static*/ IntermediatePoints IntermediatePointsCalc::calculate(RealVars RV, Chessboard* pBoard)
{
    IntermediatePoints IP;

    //don't change calculations order
    IP.minGame = calculateMarginalGamePoint(IP_MIN_GAME, pBoard);
    IP.maxGame = calculateMarginalGamePoint(IP_MAX_GAME, pBoard);
    IP.middleAbove = calculateMiddleAbovePoint(IP.minGame, IP.maxGame);
    IP.retreatLeft = calculateRetreatPoint(IP_RETREAT_LEFT, IP.middleAbove,
                                           RV.retreatLeft, IP.maxGame);
    IP.retreatRight = calculateRetreatPoint(IP_RETREAT_RIGHT, IP.middleAbove,
                                            RV.retreatRight, IP.maxGame);
    IP.cournerBelow = calculateCournerPoint(IP_COURNER_BELOW, RV);
    IP.cournerAbove = calculateCournerPoint(IP_COURNER_ABOVE, RV);
    IP.safeAxisZ = calculateSafeAxisZPoint(RV);

    return IP;
}


/*static*/ Point3D IntermediatePointsCalc::calculateMarginalGamePoint(INTERMEDIATE_POINTS IP,
                                                           Chessboard* pBoard)
{
    if (IP == IP_MIN_GAME)
    {
        Point3D minBoard;
        minBoard.x = minBoard.y = minBoard.z = std::numeric_limits<double>::max();
        for (int i=1; i<=64; ++i)
        {
            if (pBoard->getBoardType() == B_REMOVED && i>=32)
                break;
            if (pBoard->getField(i)->getLocation3D().x < minBoard.x)
                minBoard.x = pBoard->getField(i)->getLocation3D().x;
            if (pBoard->getField(i)->getLocation3D().y < minBoard.y)
                minBoard.y = pBoard->getField(i)->getLocation3D().y;
            if (pBoard->getField(i)->getLocation3D().z < minBoard.z)
                minBoard.z = pBoard->getField(i)->getLocation3D().z;
        }
        return minBoard;
    }
    else if (IP == IP_MAX_GAME)
    {
        Point3D maxBoard;
        maxBoard.x = maxBoard.y = maxBoard.z = std::numeric_limits<double>::min();
        for (int i=1; i<=64; ++i)
        {
            if (pBoard->getBoardType() == B_REMOVED && i>=32)
                break;
            if (pBoard->getField(i)->getLocation3D().x > maxBoard.x)
                maxBoard.x = pBoard->getField(i)->getLocation3D().x;
            if (pBoard->getField(i)->getLocation3D().y > maxBoard.y)
                maxBoard.y = pBoard->getField(i)->getLocation3D().y;
            //fPieceHeight on two sides of the equation is shortened:
            if (pBoard->getField(i)->getLocation3D().z > maxBoard.z)
                maxBoard.z = pBoard->getField(i)->getLocation3D().z;
        }
        maxBoard.z += (double)pBoard->fMaxPieceHeight;
        return maxBoard;
    }
    else
    {
        qCritical() << "wrong arg =" << QString::number(IP);
        return *new Point3D;
    }
}

/*static*/ Point3D IntermediatePointsCalc::calculateMiddleAbovePoint(Point3D min, Point3D max)
{
    Point3D middleAbove;
    middleAbove.x = (min.x + max.x)/2;
    middleAbove.y = (min.y + max.y)/2;
    middleAbove.z = max.z;
    return middleAbove;
}

/*static*/ Point3D IntermediatePointsCalc::calculateRetreatPoint(INTERMEDIATE_POINTS IP, Point3D midAbove,
                                                      Point3D retreat, Point3D max)
{
    if (IP == IP_RETREAT_LEFT || IP == IP_RETREAT_RIGHT)
        return *new Point3D(midAbove.x, retreat.y, max.z);
    else
    {
        qCritical() << "wrong IP arg =" << QString::number(IP);
        return *new Point3D;
    }
}

/*static*/ Point3D IntermediatePointsCalc::calculateCournerPoint(INTERMEDIATE_POINTS IP, RealVars RV)
{
    if (IP == IP_COURNER_BELOW)
        return *new Point3D(RV.home.x, RV.retreatLeft.y, RV.home.z);
    else if (IP == IP_COURNER_ABOVE)
        return *new Point3D(RV.home.x, RV.retreatLeft.y, RV.home.z + RV.fPieceHeight);
    else
    {
        qCritical() << "wrong IP arg =" << QString::number(IP);
        return *new Point3D;
    }
}

/*static*/ Point3D IntermediatePointsCalc::calculateSafeAxisZPoint(RealVars RV)
{
    Point3D safeAxisZPoint;
    safeAxisZPoint.z = qMin(qMin(RV.A1.z, RV.A8.z), qMin(RV.H1.z, RV.H8.z)) + RV.fPieceHeight;
    return safeAxisZPoint;
}
