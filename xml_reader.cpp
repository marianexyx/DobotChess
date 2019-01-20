#include "xml_reader.h"

/*static*/ LimitsVars XmlReader::m_gameLimitsVars;

XmlReader::XmlReader():
    m_xmlFileLimits(QDir::currentPath() + "/limits.xml"),
    m_xmlFileConfig(QDir::currentPath() + "/gameConfig.xml"),
    m_xmlFileDatabase(QDir::currentPath() + "/../DobotChessDB/DBConnData.xml")
{
    if (!this->openFile(XFT_GAME_LIMITS)) return;
    if (!this->openFile(XFT_GAME_CONFIG)) return;
    if (!this->openFile(XFT_DATABASE)) return;

    this->readGameLimitsNodes();
    this->readGameConfigNodes();
    this->readDatabaseNodes();
}

void XmlReader::readGameLimitsNodes()
{
    m_gameLimitsVars.fPieceHeightMin =
            this->getParam(m_xmlDocLimits, "piece", "pieceParam", "heightMin", "value").toFloat();
    m_gameLimitsVars.fPieceHeightMax =
            this->getParam(m_xmlDocLimits, "piece", "pieceParam", "heightMax", "value").toFloat();
    m_gameLimitsVars.minPoint =
            this->getPointParam(m_xmlDocLimits, "points", "point", "pointMin");
    m_gameLimitsVars.maxPoint =
            this->getPointParam(m_xmlDocLimits, "points", "point", "pointMax");
    m_gameLimitsVars.fGripperMin =
            this->getParam(m_xmlDocLimits, "gripper", "gripperParam", "pwmMin", "value").toFloat();
    m_gameLimitsVars.fGripperMax =
            this->getParam(m_xmlDocLimits, "gripper", "gripperParam", "pwmMax", "value").toFloat();
}

void XmlReader::readGameConfigNodes()
{
    m_gameConfigVars.boards.fPieceHeight =
            this->getParam(m_xmlDocConfig, "piece", "param", "height", "value").toFloat();
    m_gameConfigVars.boards.A1 =
            this->getPointParam(m_xmlDocConfig, "points", "point", "mainFieldA1");
    m_gameConfigVars.boards.A8 =
            this->getPointParam(m_xmlDocConfig, "points", "point", "mainFieldA8");
    m_gameConfigVars.boards.H1 =
            this->getPointParam(m_xmlDocConfig, "points", "point", "mainFieldH1");
    m_gameConfigVars.boards.H8 =
            this->getPointParam(m_xmlDocConfig, "points", "point", "mainFieldH8");
    m_gameConfigVars.boards.remWhiteCloserOuter =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remWhiteCloserOuter");
    m_gameConfigVars.boards.remWhiteCloserInner =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remWhiteCloserInner");
    m_gameConfigVars.boards.remWhiteFurtherOuter =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remWhiteFurtherOuter");
    m_gameConfigVars.boards.remWhiteFurtherInner =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remWhiteFurtherInner");
    m_gameConfigVars.boards.remBlackCloserOuter =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remBlackCloserOuter");
    m_gameConfigVars.boards.remBlackCloserInner =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remBlackCloserInner");
    m_gameConfigVars.boards.remBlackFurtherOuter =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remBlackFurtherOuter");
    m_gameConfigVars.boards.remBlackFurtherInner =
            this->getPointParam(m_xmlDocConfig, "points", "point", "remBlackFurtherInner");
    m_gameConfigVars.dobot.home =
            this->getPointParam(m_xmlDocConfig, "points", "point", "home");
    m_gameConfigVars.retreatLeft =
            this->getPointParam(m_xmlDocConfig, "points", "point", "retreatLeft");
    m_gameConfigVars.retreatRight =
            this->getPointParam(m_xmlDocConfig, "points", "point", "retreatRight");
    m_gameConfigVars.dobot.fGripperOpened =
            this->getParam(m_xmlDocConfig, "gripper", "state", "open", "pwm").toFloat();
    m_gameConfigVars.dobot.fGripperClosed =
            this->getParam(m_xmlDocConfig, "gripper", "state", "close", "pwm").toFloat();

    this->isVarsStructInLimits();
}

void XmlReader::readDatabaseNodes()
{
    QString elMain = "database";
    QString node = "param";
    QString val = "value";

    m_databaseVars.QStrHostName =
            this->getParam(m_xmlDocDatabase, elMain, node, "hostName", val);
    m_databaseVars.QStrDatabaseName =
            this->getParam(m_xmlDocDatabase, elMain, node, "databaseName", val);
    m_databaseVars.QStrUserName =
            this->getParam(m_xmlDocDatabase, elMain, node, "userName", val);
    m_databaseVars.QStrPassword =
            this->getParam(m_xmlDocDatabase, elMain, node, "password", val);
}

bool XmlReader::openFile(XML_FILE_TYPE XFT)
{
    QFile* xmlFile;
    QDomDocument* xmlDoc;
    switch(XFT)
    {
    case XFT_GAME_LIMITS:
        xmlFile = &m_xmlFileLimits;
        xmlDoc = &m_xmlDocLimits;
        break;
    case XFT_GAME_CONFIG:
        xmlFile = &m_xmlFileConfig;
        xmlDoc = &m_xmlDocConfig;
        break;
    case XFT_DATABASE:
        xmlFile = &m_xmlFileDatabase;
        xmlDoc = &m_xmlDocDatabase;
        break;
    default:
        qCritical() << "unknown file type =" << QString::number(XFT);
        return false;
    }

    if (!xmlFile->exists())
    {
        qCritical() << "file name:" << xmlFile->fileName() << "don't exists";
        return false;
    }

    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "failed to load file:" << xmlFile->errorString();
        return false;
    }
    else
    {
        QString errorStr;
        int errorLine;
        int errorColumn;
        if(!xmlDoc->setContent(xmlFile, false, &errorStr, &errorLine, &errorColumn))
        {
            qCritical() << "failed to load document. error:" << errorStr << "at line"
                        << QString::number(errorLine) << "column" << QString::number(errorColumn);
            xmlFile->close();
            return false;
        }
        xmlFile->close();
    }

    return true;
}

QString XmlReader::getParam(QDomDocument xmlDoc, QString QStrDomElMain, QString QStrDomNodeList,
                            QString QStrDomElName, QString QStrValue)
{
    QDomElement docElem = xmlDoc.documentElement();
    QDomNode node = docElem.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement(); //try to convert the node to an element
        if(!element.isNull())
        {
            if (element.tagName() == QStrDomElMain)
            {
                QDomNodeList paramDomNodeList = docElem.elementsByTagName(QStrDomNodeList);
                for(int i=0; i<paramDomNodeList.count(); i++)
                {
                    QDomNode paramNode = paramDomNodeList.at(i);
                    if (paramNode.isElement())
                    {
                        QDomElement param = paramNode.toElement();

                        if (param.attribute("name") == QStrDomElName)
                            return param.attribute(QStrValue);
                    }
                }
            }
        }
        node = node.nextSibling();
    }
    qCritical() << "element.tagName() not found. name =" << QStrDomElMain
                << ", QStrDomNodeList =" << QStrDomNodeList << ", QStrDomElName ="
                << QStrDomElName << ", QStrValue =" << QStrValue;
    return "";
}

Point3D XmlReader::getPointParam(QDomDocument xmlDoc, QString QStrDomElMain,
                                 QString QStrDomNodeList, QString QStrDomElName)
{
    Point3D p3d;
    p3d.x = this->getParam(xmlDoc, QStrDomElMain, QStrDomNodeList, QStrDomElName, "x").toDouble();
    p3d.y = this->getParam(xmlDoc, QStrDomElMain, QStrDomNodeList, QStrDomElName, "y").toDouble();
    p3d.z = this->getParam(xmlDoc, QStrDomElMain, QStrDomNodeList, QStrDomElName, "z").toDouble();
    return p3d;
}

bool XmlReader::isVarsStructInLimits()
{
    if (this->isPieceHeightInLimits(m_gameConfigVars.boards.fPieceHeight) &&
            isPointInLimits(m_gameConfigVars.dobot.home) &&
            isPointInLimits(m_gameConfigVars.boards.A1) &&
            isPointInLimits(m_gameConfigVars.boards.A8) &&
            isPointInLimits(m_gameConfigVars.boards.H1) &&
            isPointInLimits(m_gameConfigVars.boards.H8) &&
            isPointInLimits(m_gameConfigVars.boards.remWhiteCloserOuter) &&
            isPointInLimits(m_gameConfigVars.boards.remWhiteFurtherInner) &&
            isPointInLimits(m_gameConfigVars.boards.remBlackCloserOuter) &&
            isPointInLimits(m_gameConfigVars.boards.remBlackFurtherInner) &&
            isGripperParamInLimits(m_gameConfigVars.dobot.fGripperOpened) &&
            isGripperParamInLimits(m_gameConfigVars.dobot.fGripperClosed))
    {
        return true;
    }
    else qFatal("real data out of scope");
}

/*static*/ bool XmlReader::isPieceHeightInLimits(float fPieceHeight)
{
    float fHmin = XmlReader::m_gameLimitsVars.fPieceHeightMin;
    float fHmax = XmlReader::m_gameLimitsVars.fPieceHeightMax;
    if (fPieceHeight > fHmin && fPieceHeight < fHmax)
        return true;
    else
    {
        qCritical() << "pieceHeight out of scope range(" << QString::number(fHmin) << ","
                    << QString::number(fHmax) << "):" << QString::number(fPieceHeight);
        return false;
    }
}

/*static*/ bool XmlReader::isPointInLimits(Point3D point)
{
    Point3D minErrorCorner = XmlReader::m_gameLimitsVars.minPoint;
    Point3D maxErrorCorner = XmlReader::m_gameLimitsVars.maxPoint;
    if (point.x > minErrorCorner.x && point.x < maxErrorCorner.x &&
            point.y > minErrorCorner.y && point.y < maxErrorCorner.y &&
            point.z > minErrorCorner.z && point.z < maxErrorCorner.z)
        return true;
    else
    {
        qCritical() << "one of the points vals is out of scope range("
                    << minErrorCorner.getAsQStr() << "," << maxErrorCorner.getAsQStr()
                    << "). point =" << point.getAsQStr();
        return false;
    }
}

/*static*/ bool XmlReader::isGripperParamInLimits(float fGripperPar)
{
    float fGmin = XmlReader::m_gameLimitsVars.fPieceHeightMin;
    float fGmax = XmlReader::m_gameLimitsVars.fPieceHeightMax;
    if (fGripperPar > fGmin && fGripperPar < fGmax)
        return true;
    else
    {
        qCritical() << "one of the gripper vals is out of scope range(" << QString::number(fGmin)
                    << "," << QString::number(fGmax) << "):" << QString::number(fGripperPar);
        return false;
    }
}
