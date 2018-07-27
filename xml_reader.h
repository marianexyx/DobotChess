#ifndef XML_READER_H
#define XML_READER_H

#pragma once
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <QFile>
#include <QString>
#include "vars/basic_vars.h"

enum XML_FILE_TYPE { XFT_GAME_LIMITS, XFT_GAME_CONFIG, XFT_DATABASE };

struct LimitsVars
{
    float fPieceHeightMin, fPieceHeightMax;
    Point3D minPoint, maxPoint;
    float fGripperMin, fGripperMax;
};

struct RealVars
{
    float fPieceHeight;
    Point3D home, homeToMiddleAbove;
    Point3D A1, A8, H1, H8;
    Point3D remWhiteCloserOuter, remWhiteFurtherInner;
    Point3D remBlackCloserOuter, remBlackFurtherInner;
    Point3D retreatLeft, retreatRight;
    float fGripperOpened, fGripperClosed;
};

struct DatabaseVars
{
    QString QStrHostName;
    QString QStrDatabaseName;
    QString QStrUserName;
    QString QStrPassword;
};

class XmlReader
{
private:
    QFile _xmlFileLimits, _xmlFileConfig, _xmlFileDatabase;
    QDomDocument _xmlDocLimits, _xmlDocConfig, _xmlDocDatabase;
    static LimitsVars _gameLimitsVars;
    RealVars _gameConfigVars;
    DatabaseVars _databaseVars;

    bool openFile(XML_FILE_TYPE XFT);
    QString getParam(QDomDocument xmlDoc, QString QStrDomElMain, QString QStrDomNodeList,
                     QString QStrDomElName, QString QStrValue);
    Point3D getPointParam(QDomDocument xmlDoc, QString QStrDomElMain,
                          QString QStrDomNodeList, QString QStrDomElName);
    void readGameLimitsNodes();
    void readGameConfigNodes();
    void readDatabaseNodes();

public:
    XmlReader();

    bool isVarsStructInLimits();
    static bool isPieceHeightInLimits(float fPieceHeight);
    static bool isPointInLimits(Point3D point);
    static bool isGripperParamInLimits(float fGripperPar);
    RealVars getRealVars() { this->isVarsStructInLimits(); return _gameConfigVars; }
    DatabaseVars getDatabaseVars() const { return _databaseVars; }
};

#endif // XML_READER_H
