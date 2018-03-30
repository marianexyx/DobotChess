#ifndef XML_READER_H
#define XML_READER_H

#pragma once
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <QFile>
#include <QString>
#include "vars/basic_vars.h"

//https://www.youtube.com/watch?v=NzQwJdcdRKE&index=95&list=PL2D1942A4688E9D63

//todo: nic nie testowane
class XmlReader
{
private:
    QDomDocument xmlDoc;
    QFile xmlFile;
    RealVars _vars;

    bool areVarsInLimits();
    bool isPieceHeightInLimits(float fPieceHeight);
    bool isPointInLimits(Point3D point);
    bool isGripperParamInLimits(float fGripperPar);

public:
    XmlReader(QString QStrFileName);

    void listElements(QDomElement root, QString QStrTagName, QString QStrAttribute);
    RealVars getRealVars();
};

#endif // XML_READER_H
