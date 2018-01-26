#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pramga once

#include <QString>

enum COMMUNICATION_TYPE { WEBSITE = 1,
                          ARDUINO = 2,
                          TEST = 3 };

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
};

#endif // BASIC_VARS_H
