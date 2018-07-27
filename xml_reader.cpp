#include "xml_reader.h"

/*static*/ LimitsVars XmlReader::_gameLimitsVars;

XmlReader::XmlReader():
    _xmlFileLimits(QDir::currentPath() + "/limits.xml"),
    _xmlFileConfig(QDir::currentPath() + "/gameConfig.xml"),
    _xmlFileDatabase(QDir::currentPath() + "/../DobotChessDB/DBConnData.xml")
{
    if (!this->openFile(XFT_GAME_LIMITS)) return;
    if (!this->openFile(XFT_GAME_CONFIG)) return;
    if (!this->openFile(XFT_DATABASE)) return;

    this->readGameLimitsNodes();
    this->readGameConfigNodes();
    this->readDatabaseNodes();
}

bool XmlReader::openFile(XML_FILE_TYPE XFT)
{
    QFile* xmlFile;
    QDomDocument* xmlDoc;
    switch(XFT)
    {
    case XFT_GAME_LIMITS:
        xmlFile = &_xmlFileLimits;
        xmlDoc = &_xmlDocLimits;
        break;
    case XFT_GAME_CONFIG:
        xmlFile = &_xmlFileConfig;
        xmlDoc = &_xmlDocConfig;
        break;
    case XFT_DATABASE:
        xmlFile = &_xmlFileDatabase;
        xmlDoc = &_xmlDocDatabase;
        break;
    default:
        qDebug() << "ERROR: XmlReader::openFile(): unnknown file type =" << XFT;
        return false;
    }

    if (!xmlFile->exists())
    {
        qDebug() << "ERROR: XmlReader::openFile(): file name:" << xmlFile->fileName()
                 << "don't exists";
        return false;
    }

    QString errMsg;
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errMsg = xmlFile->errorString();
        qDebug() << "ERROR: XmlReader::openFile(): failed to load file:" << errMsg;

        return false;
    }
    else
    {
        QString errorStr;
        int errorLine;
        int errorColumn;
        if(!xmlDoc->setContent(xmlFile, false, &errorStr, &errorLine, &errorColumn))
        {
            qDebug() << "ERROR: XmlReader::openFile(): failed to load document. error:"
                     << errorStr << "at line" << errorLine << "column" << errorColumn;
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
            /*qDebug() << "XmlReader::readGameLimitsNodes(): element.tagName()"
                     << qPrintable(element.tagName()); //the node really is an element*/
            if (element.tagName() == QStrDomElMain)
            {
                QDomNodeList paramDomNodeList = docElem.elementsByTagName(QStrDomNodeList);
                //qDebug() << "paramDomNodeList amount =" << paramDomNodeList.count();
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
    qDebug() << "ERROR: XmlReader::getParam(): element.tagName() not found. name ="
             << QStrDomElMain << ", QStrDomNodeList =" << QStrDomNodeList
             << ", QStrDomElName =" << QStrDomElName << ", QStrValue =" << QStrValue;
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

void XmlReader::readGameLimitsNodes()
{
    _gameLimitsVars.fPieceHeightMin =
            this->getParam(_xmlDocLimits, "piece", "pieceParam", "heightMin", "value").toFloat();
    _gameLimitsVars.fPieceHeightMax =
            this->getParam(_xmlDocLimits, "piece", "pieceParam", "heightMax", "value").toFloat();
    _gameLimitsVars.minPoint =
            this->getPointParam(_xmlDocLimits, "points", "point", "pointMin");
    _gameLimitsVars.maxPoint =
            this->getPointParam(_xmlDocLimits, "points", "point", "pointMax");
    _gameLimitsVars.fGripperMin =
            this->getParam(_xmlDocLimits, "gripper", "gripperParam", "pwmMin", "value").toFloat();
    _gameLimitsVars.fGripperMax =
            this->getParam(_xmlDocLimits, "gripper", "gripperParam", "pwmMax", "value").toFloat();
}

void XmlReader::readGameConfigNodes()
{
    _gameConfigVars.fPieceHeight =
            this->getParam(_xmlDocConfig, "piece", "param", "height", "value").toFloat();
    _gameConfigVars.A1 =
            this->getPointParam(_xmlDocConfig, "points", "point", "mainFieldA1");
    _gameConfigVars.A8 =
            this->getPointParam(_xmlDocConfig, "points", "point", "mainFieldA8");
    _gameConfigVars.H1 =
            this->getPointParam(_xmlDocConfig, "points", "point", "mainFieldH1");
    _gameConfigVars.H8 =
            this->getPointParam(_xmlDocConfig, "points", "point", "mainFieldH8");
    _gameConfigVars.remWhiteCloserOuter =
            this->getPointParam(_xmlDocConfig, "points", "point", "remWhiteCloserOuter");
    _gameConfigVars.remWhiteFurtherInner =
            this->getPointParam(_xmlDocConfig, "points", "point", "remWhiteFurtherInner");
    _gameConfigVars.remBlackCloserOuter =
            this->getPointParam(_xmlDocConfig, "points", "point", "remBlackCloserOuter");
    _gameConfigVars.remBlackFurtherInner =
            this->getPointParam(_xmlDocConfig, "points", "point", "remBlackFurtherInner");
    _gameConfigVars.home =
            this->getPointParam(_xmlDocConfig, "points", "point", "home");
    _gameConfigVars.homeToMiddleAbove =
            this->getPointParam(_xmlDocConfig, "points", "point", "homeToMiddleAbove");
    _gameConfigVars.retreatLeft =
            this->getPointParam(_xmlDocConfig, "points", "point", "retreatLeft");
    _gameConfigVars.retreatRight =
            this->getPointParam(_xmlDocConfig, "points", "point", "retreatRight");
    _gameConfigVars.fGripperOpened =
            this->getParam(_xmlDocConfig, "gripper", "state", "open", "pwm").toFloat();
    _gameConfigVars.fGripperClosed =
            this->getParam(_xmlDocConfig, "gripper", "state", "close", "pwm").toFloat();

    this->isVarsStructInLimits();
}

void XmlReader::readDatabaseNodes()
{
    QString elMain = "database";
    QString node = "param";
    QString val = "value";

    _databaseVars.QStrHostName =
            this->getParam(_xmlDocDatabase, elMain, node, "hostName", val);
    _databaseVars.QStrDatabaseName =
            this->getParam(_xmlDocDatabase, elMain, node, "databaseName", val);
    _databaseVars.QStrUserName =
            this->getParam(_xmlDocDatabase, elMain, node, "userName", val);
    _databaseVars.QStrPassword =
            this->getParam(_xmlDocDatabase, elMain, node, "password", val);
}

bool XmlReader::isVarsStructInLimits()
{
    if (this->isPieceHeightInLimits(_gameConfigVars.fPieceHeight) &&
            isPointInLimits(_gameConfigVars.home) &&
            isPointInLimits(_gameConfigVars.homeToMiddleAbove) &&
            isPointInLimits(_gameConfigVars.A1) &&
            isPointInLimits(_gameConfigVars.A8) &&
            isPointInLimits(_gameConfigVars.H1) &&
            isPointInLimits(_gameConfigVars.H8) &&
            isPointInLimits(_gameConfigVars.remWhiteCloserOuter) &&
            isPointInLimits(_gameConfigVars.remWhiteFurtherInner) &&
            isPointInLimits(_gameConfigVars.remBlackCloserOuter) &&
            isPointInLimits(_gameConfigVars.remBlackFurtherInner) &&
            isGripperParamInLimits(_gameConfigVars.fGripperOpened) &&
            isGripperParamInLimits(_gameConfigVars.fGripperClosed))
    {
        return true;
    }
    else
    {
        qDebug() << "FATAL ERROR: real data out of scope. crash app";
        exit(EXIT_FAILURE);
    }
}

/*static*/ bool XmlReader::isPieceHeightInLimits(float fPieceHeight)
{
    float fHmin = XmlReader::_gameLimitsVars.fPieceHeightMin;
    float fHmax = XmlReader::_gameLimitsVars.fPieceHeightMax;
    if (fPieceHeight > fHmin && fPieceHeight < fHmax)
        return true;
    else
    {
        qDebug() << "ERROR: XmlReader::isPieceHeightInLimits(): pieceHeight out of "
                    "scope range(" << fHmin << "," << fHmax << "):" << fPieceHeight;
        return false;
    }
}

/*static*/ bool XmlReader::isPointInLimits(Point3D point)
{
    Point3D minErrorCorner = XmlReader::_gameLimitsVars.minPoint;
    Point3D maxErrorCorner = XmlReader::_gameLimitsVars.maxPoint;
    if (point.x > minErrorCorner.x && point.x < maxErrorCorner.x &&
            point.y > minErrorCorner.y && point.y < maxErrorCorner.y &&
            point.z > minErrorCorner.z && point.z < maxErrorCorner.z)
        return true;
    else
    {
        qDebug() << "ERROR: isPointInLimits(): one of the points vals is out of "
                    "scope range(" << minErrorCorner.getAsQStr() << ","
                 << maxErrorCorner.getAsQStr() << "). point =" << point.getAsQStr();
        return false;
    }
}

/*static*/ bool XmlReader::isGripperParamInLimits(float fGripperPar)
{
    float fGmin = XmlReader::_gameLimitsVars.fPieceHeightMin;
    float fGmax = XmlReader::_gameLimitsVars.fPieceHeightMax;
    if (fGripperPar > fGmin && fGripperPar < fGmax)
        return true;
    else
    {
        qDebug() << "ERROR: XmlReader::isGripperParamInLimits(): one of the gripper "
                    "vals is out of scope range(" << fGmin << "," << fGmax << "):"
                 << fGripperPar;
        return false;
    }
}
