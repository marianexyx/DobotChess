#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pragma once
#include <QString>

//future: this dir may not exists, if every class would have its own dir

const bool SHOW_ERRORS = true;
const bool PRINT_DEBUG = true;
const bool INFORM_CLIENTS = true;
const bool SERVICE = true;

const bool REAL = true;
const bool IMAGINARY = false;

struct Point3D
{
    double x, y, z;

    bool operator==(const struct Point3D& p)
    { return (x == p.x && y == p.y && z == p.z); }

    bool operator!=(const struct Point3D& p)
    { return (x != p.x || y != p.y || z != p.z); }

    Point3D &operator=(const Point3D &p)
    { x = p.x; y = p.y; z = p.z; return *this; }

    Point3D(): x(0.f), y(0.f), z(0.f) {}
    Point3D(double dX, double dY, double dZ): x(dX), y(dY), z(dZ) {}

    void setPoint(double dX, double dY, double dZ) { x = dX; y = dY; z = dZ; }
    bool isPointSet() { return (x == 0.f && y == 0.f && z == 0.f) ? false : true; }
    QString getAsQStr() const { return QString::number((int)x) + ", " +
                QString::number((int)y) + ", " + QString::number((int)z); }
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

inline QString decToHex(int nDec)
{
    if (nDec > 256)
    {
        qDebug() << "WARNING: nDec > 256. change it to max for this funtion: "
                    "256 (ff). nDec =" << nDec;
        nDec = 256;
    }

    QString QStrReturn = QString::number(nDec, 16);
    //if (nDec < 16) QStrReturn = "0" + QStrReturn;
    QStrReturn = /*"0x" +*/ QStrReturn;

    return QStrReturn;
}

#endif // BASIC_VARS_H
