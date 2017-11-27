#ifndef BASIC_VARS_H
#define BASIC_VARS_H

#pramga once

#include <QString>

//todo pozamieniac to

const int WEBSITE = 1;
const int ARDUINO = 2;
const int TEST = 3;

const int ACTUAL_POS = 1000;

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
