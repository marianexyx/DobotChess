#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pragma once
#include <QString>

//future: this dir may not exists, if every class would have its own dir

const bool SHOW_ERRORS = true;
const bool PRINT_DEBUG = true;

const bool REAL = true;
const bool IMAGINARY = false;

struct Point3D
{
    double x;
    double y;
    double z;

    bool operator==(const struct Point3D& p)
    { return x == p.x && y == p.y && z == p.z; }

    Point3D(): x(0.f), y(0.f), z(0.f) {}
    Point3D(double dX, double dY, double dZ): x(dX), y(dY), z(dZ) {}

    void setPoint(double dX, double dY, double dZ) { x = dX; y = dY; z = dZ; }
    void setPoint(Point3D p3d) { x = p3d.x; y = p3d.y; z = p3d.z; }
    bool isPointSet()
    {
        if (x == 0.f && y == 0.f && z == 0.f) return false;
        else return true;
    }
    QString getAsQStr() const { return QString::number((int)x) + ", " +
                QString::number((int)y) + ", " + QString::number((int)z); }
};

//future: place it near xml reader
struct RealVars
{
    float fPieceHeight;
    Point3D home, homeToMiddleAbove;
    Point3D A1, A8, H1, H8;
    Point3D remWhiteCloserOuter, remWhiteFurtherInner;
    Point3D remBlackCloserOuter, remBlackFurtherInner;
    float fGripperOpened, fGripperClosed;
};

struct DatabaseVars
{
    QString QStrHostName;
    QString QStrDatabaseName;
    QString QStrUserName;
    QString QStrPassword;
};

enum VERTICAL_MOVE { VM_NONE, VM_GRAB, VM_PUT };
inline QString verticalMoveAsQStr(VERTICAL_MOVE VM)
{
    switch(VM)
    {
    case VM_NONE: return "none"; break;
    case VM_GRAB: return "grab"; break;
    case VM_PUT: return "put"; break;
    default:
        qDebug() << "ERROR: verticalMoveAsQStr(): wrong arg =" << VM;
        return "none";
    }
}

inline bool isPointInLimits(Point3D point) //todo: xml
{
    Point3D minErrorCorner(100.f, -180.f, -25.f);
    Point3D maxErrorCorner(340.f, 180.f, 50.f);

    if (point.x > minErrorCorner.x && point.x < maxErrorCorner.x &&
            point.y > minErrorCorner.y && point.y < maxErrorCorner.y &&
            point.z > minErrorCorner.z && point.z < maxErrorCorner.z)
        return true;
    else
    {
        qDebug() << "ERROR: isPointInLimits(): one of the points vals is out of"
                    " scope range. point =" << point.x << point.y << point.z;
        return false;
    }
}

inline QString decToHex(int nDec)
{
    if (nDec > 256)
    {
        qDebug() << "WARNING: nDec > 256. change it to max for this funtion: "
                    "256 (ff). nDec =" << nDec;
        nDec = 256;
    }

    QString QStrReturn = QString::number(nDec, 16);
    if (nDec < 16) QStrReturn = "0" + QStrReturn;
    QStrReturn = "0x" + QStrReturn;

    return QStrReturn;
}

#endif // BASIC_VARS_H
