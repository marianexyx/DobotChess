#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pramga once

#include <QString>

const bool SHOW_ERRORS = true; //todo: poupychać do wszystkich erro logów

enum COMMUNICATION_TYPE { WEBSITE = 1, ARDUINO, TEST };

inline QString communicationTypeAsQStr(COMMUNICATION_TYPE type)
{
    QString typeAsQStr;

    switch(type)
    {
    case WEBSITE: typeAsQStr = "website"; break;
    case ARDUINO: typeAsQStr = "arduino"; break;
    case TEST: typeAsQStr = "test"; break;
    default: qDebug() << "ERROR: communicationTypeAsQStr(): unknown parametr =" << type;
    }

    return typeAsQStr;
}

struct Point3D
{
    double x;
    double y;
    double z;

    Point3D() { x = 0.0; y = 0.0, z = 0.0; }
    Point3D(double dX, double dY, double dZ) { x = dX; y = dY; z = dZ; }
};

#endif // BASIC_VARS_H
