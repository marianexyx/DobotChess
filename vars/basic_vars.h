#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pramga once
#include <QString>

//future: w zasadzie folder vars/ mógłby nie istnieć, jeżeli każda większa klasa miała by...
//...swój folder podklas

const bool SHOW_ERRORS = true;

enum COMMUNICATION_TYPE { WEBSITE = 1, ARDUINO, TEST };
inline QString communicationTypeAsQStr(COMMUNICATION_TYPE type)
{
    switch(type)
    {
    case WEBSITE: return "website";
    case ARDUINO: return "arduino";
    case TEST: return "test";
    default:
        qDebug() << "ERROR: communicationTypeAsQStr(): unknown parametr =" << type;
        return "";
    }
}

struct Point3D
{
    double x;
    double y;
    double z;

    bool operator ==(const struct Point3D& p) { return x == p.x && y == p.y && z == p.z; }

    Point3D() { x = 0.0; y = 0.0, z = 0.0; }
    Point3D(double dX, double dY, double dZ) { x = dX; y = dY; z = dZ; }
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
        return VM_NONE;
    }
}

#endif // BASIC_VARS_H
