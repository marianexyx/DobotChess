#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pramga once

#include <QString>

enum COMMUNICATION_TYPES { WEBSITE = 1,
                           ARDUINO = 2,
                           TEST = 3 };

inline QString communicationTypeAsQStr(COMMUNICATION_TYPES type)
{
    QString typeAsQStr;

    switch(type)
    {
    case WEBSITE: typeAsQStr = "website"; break;
    case ARDUINO: typeAsQStr = "arduino"; break;
    case TEST: typeAsQStr = "test"; break;
    default: qDebug() << "ERROR: communicationTypeAsQStr: unknown parametr =" <<
                         communicationTypeAsQStr;
    }

    return typeAsQStr;
}


const int ACTUAL_POS = 1000; //todo: zamienic na limity, jezeli jeszce bedzie potrzebne

struct Point3D
{
    double x;
    double y;
    double z;
};

QString **create2DArray()
{
    QString **array2D = new char*[8];
    for (int i=0; i<8; i++)
    {
       array2D[i] = new char[8];
       for (int j=0; j<8; j++)
         array2D[i][j] = "-";
    }

    return array2D;
}

void freeBoard(QString **array2D)
{
    for (int i=0; i<8; i++)
      delete [] array2D[i];
    delete [] array2D;
}

#endif // BASIC_VARS_H
