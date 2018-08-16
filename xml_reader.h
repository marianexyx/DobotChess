#ifndef XML_READER_H
#define XML_READER_H

#pragma once
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <QFile>
#include <QString>
#include "vars/const_flags.h"
#include "vars/point3d.h"

enum XML_FILE_TYPE { XFT_GAME_LIMITS, XFT_GAME_CONFIG, XFT_DATABASE };

//future: make game vars dir for those structs + some other ones
struct LimitsVars
{
    float fPieceHeightMin, fPieceHeightMax;
    Point3D minPoint, maxPoint;
    float fGripperMin, fGripperMax;
};

struct RealVars
{
    float fPieceHeight;
    Point3D home;
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
    QFile m_xmlFileLimits, m_xmlFileConfig, m_xmlFileDatabase;
    QDomDocument m_xmlDocLimits, m_xmlDocConfig, m_xmlDocDatabase;
    static LimitsVars m_gameLimitsVars;
    RealVars m_gameConfigVars;
    DatabaseVars m_databaseVars;

    void readGameLimitsNodes();
    void readGameConfigNodes();
    void readDatabaseNodes();
    bool openFile(XML_FILE_TYPE XFT);
    QString getParam(QDomDocument xmlDoc, QString QStrDomElMain, QString QStrDomNodeList,
                     QString QStrDomElName, QString QStrValue);
    Point3D getPointParam(QDomDocument xmlDoc, QString QStrDomElMain,
                          QString QStrDomNodeList, QString QStrDomElName);

public:
    XmlReader();

    bool isVarsStructInLimits();
    static bool isPieceHeightInLimits(float fPieceHeight);
    static bool isPointInLimits(Point3D point);
    static bool isGripperParamInLimits(float fGripperPar);
    RealVars getRealVars() { this->isVarsStructInLimits(); return m_gameConfigVars; }
    DatabaseVars getDatabaseVars() const { return m_databaseVars; }
};

#endif // XML_READER_H
