#ifndef POINT3D_H
#define POINT3D_H

#pragma once
#include <QString>

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

    static bool isPointCloseToAnother(Point3D p1, Point3D p2)
    { return qAbs(p1.x-p2.x) < 3.f && qAbs(p1.y-p2.y) < 3.f && qAbs(p1.z-p2.z) < 3.f; }
    bool isPointSet() { return (x == 0.f && y == 0.f && z == 0.f) ? false : true; }
    void setPoint(double dX, double dY, double dZ) { x = dX; y = dY; z = dZ; }
    QString getAsQStr() const { return QString::number((int)x) + ", " +
                QString::number((int)y) + ", " + QString::number((int)z); }
};

#endif // POINT3D_H
