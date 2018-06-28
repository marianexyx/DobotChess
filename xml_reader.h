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

class XmlReader
{
private:
    QDomDocument _xmlDoc;
    QFile _xmlFile;
    RealVars _vars;

    void readPiece(QDomNodeList piece);
    void readPoints(QDomNodeList points);
    void readGripper(QDomNodeList gripper);

    bool isPieceHeightInLimits(float fPieceHeight);
    bool isGripperParamInLimits(float fGripperPar);

public:
    XmlReader(QString QStrFileName);

    bool isVarsStructInLimits();
    RealVars getRealVars() { this->isVarsStructInLimits(); return _vars; }
};

#endif // XML_READER_H
