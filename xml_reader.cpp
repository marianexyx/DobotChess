#include "xml_reader.h"

XmlReader::XmlReader(QString QStrFileName, XML_FILE_TYPE XFT):
    _xmlFile(QStrFileName)
{
    if (!_xmlFile.exists())
    {
        qDebug() << "ERROR: XmlReader::XmlReader(): file name:" << QStrFileName
                 << "don't exists";
        return;
    }

    QString errMsg;
    if (!_xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errMsg = _xmlFile.errorString();
        qDebug() << "ERROR: XmlReader::XmlReader(): failed to load file:" << errMsg;

        return;
    }
    else
    {
        QString errorStr;
        int errorLine;
        int errorColumn;
        if(!_xmlDoc.setContent(&_xmlFile, false, &errorStr, &errorLine, &errorColumn))
        {
            qDebug() << "ERROR: XmlReader::XmlReader(): failed to load document. error:"
                     << errorStr << "at line" << errorLine << "column" << errorColumn;
            _xmlFile.close();
            return;
        }
        _xmlFile.close();
    }

    if (XFT == XFT_GAME_CONFIG)
        this->readGameConfigNodes();
    else if (XFT == XFT_DATABASE)
        this->readDatabaseNodes();
    else qDebug() << "ERROR: XmlReader::XmlReader(): unknown XFT type =" << XFT;
}

void XmlReader::readDatabaseNodes()
{
    QDomElement docElem = _xmlDoc.documentElement();
    QDomNode node = docElem.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement(); //try to convert the node to an element
        if(!element.isNull())
        {
            //qDebug() << qPrintable(element.tagName()); //the node really is an element

            if (element.tagName() == "database")
            {
                QDomNodeList paramDomNodeList = docElem.elementsByTagName("param");

                for(int i=0; i<paramDomNodeList.count(); i++)
                {
                    QDomNode paramNode = paramDomNodeList.at(i);
                    if (paramNode.isElement())
                    {
                        QDomElement param = paramNode.toElement();

                        if (param.attribute("name") == "hostName")
                            _databaseVars.QStrHostName = param.attribute("value");
                        else if (param.attribute("name") == "databaseName")
                            _databaseVars.QStrDatabaseName = param.attribute("value");
                        else if (param.attribute("name") == "userName")
                            _databaseVars.QStrUserName = param.attribute("value");
                        else if (param.attribute("name") == "password")
                            _databaseVars.QStrPassword = param.attribute("value");
                        else qDebug() << "ERROR: XmlReader::readDatabaseNodes(): unknown state"
                                         " name =" << param.attribute("name");
                    }
                    else qDebug() << "ERROR: XmlReader::readDatabaseNodes(): "
                                     "paramNode.isElement() == false";
                }
            }
            else qDebug() << "ERROR: XmlReader::readDatabaseNodes(): element.tagName() "
                             "!= 'database'. it's ==" << element.tagName();
        }
        node = node.nextSibling();
    }
}

void XmlReader::readGameConfigNodes()
{
    QDomElement docElem = _xmlDoc.documentElement();
    QDomNode node = docElem.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement(); //try to convert the node to an element
        if(!element.isNull())
        {
            //qDebug() << qPrintable(element.tagName()); //the node really is an element

            if (element.tagName() == "piece")
                this->readPiece(docElem.elementsByTagName("param"));
            else if (element.tagName() == "points")
                this->readPoints(docElem.elementsByTagName("point"));
            else if (element.tagName() == "gripper")
                this->readGripper(docElem.elementsByTagName("state"));
        }
        node = node.nextSibling();
    }

    this->isVarsStructInLimits();
}

void XmlReader::readPiece(QDomNodeList piece)
{
    //qDebug() << "piece amount =" << piece.count();
    for(int i=0; i<piece.count(); i++)
    {
        QDomNode paramNode = piece.at(i);
        if (paramNode.isElement())
        {
            QDomElement param = paramNode.toElement();

            if (param.attribute("name") == "height")
                _gameConfigVars.fPieceHeight = param.attribute("val").toFloat();
            else qDebug() << "ERROR: XmlReader::readPiece(): param name != "
                             "height. it's =" << param.attribute("name");
        }
    }
}

void XmlReader::readPoints(QDomNodeList points)
{
    //qDebug() << "points amount =" << points.count();
    for(int i=0; i<points.count(); i++)
    {
        QDomNode pointNode = points.at(i);
        if (pointNode.isElement())
        {
            QDomElement p = pointNode.toElement();

            Point3D p3d(p.attribute("x").toDouble(), p.attribute("y").toDouble(),
                        p.attribute("z").toDouble());

            if (p.attribute("name") == "mainFieldA1")
                _gameConfigVars.A1.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldA8")
                _gameConfigVars.A8.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldH1")
                _gameConfigVars.H1.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldH8")
                _gameConfigVars.H8.setPoint(p3d);
            else if (p.attribute("name") == "remWhiteCloserOuter")
                _gameConfigVars.remWhiteCloserOuter.setPoint(p3d);
            else if (p.attribute("name") == "remWhiteFurtherInner")
                _gameConfigVars.remWhiteFurtherInner.setPoint(p3d);
            else if (p.attribute("name") == "remBlackCloserOuter")
                _gameConfigVars.remBlackCloserOuter.setPoint(p3d);
            else if (p.attribute("name") == "remBlackFurtherInner")
                _gameConfigVars.remBlackFurtherInner.setPoint(p3d);
            else if (p.attribute("name") == "home")
                _gameConfigVars.home.setPoint(p3d);
            else if (p.attribute("name") == "homeToMiddleAbove")
                _gameConfigVars.homeToMiddleAbove.setPoint(p3d);
            else qDebug() << "ERROR: XmlReader::readPoints(): unknown param name ="
                          << p.attribute("name");
        }
    }
}

void XmlReader::readGripper(QDomNodeList gripper)
{
    for(int i=0; i<gripper.count(); i++)
    {
        QDomNode stateNode = gripper.at(i);
        if (stateNode.isElement())
        {
            QDomElement state = stateNode.toElement();

            if (state.attribute("name") == "open")
                _gameConfigVars.fGripperOpened = state.attribute("pwm").toFloat();
            else if (state.attribute("name") == "close")
                _gameConfigVars.fGripperClosed = state.attribute("pwm").toFloat();
            else qDebug() << "ERROR: XmlReader::readGripper(): unknown state name ="
                          << state.attribute("name");
        }
    }
}

bool XmlReader::isVarsStructInLimits()
{
    //todo: add few more tests and locks
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

bool XmlReader::isPieceHeightInLimits(float fPieceHeight)
{
    if (fPieceHeight > 1.f && fPieceHeight < 100.f)
        return true;
    else
    {
        qDebug() << "ERROR: XmlReader::isPieceHeightInLimits(): pieceHeight "
                    "out of scope (0,100):" << fPieceHeight;
        return false;
    }
}

bool XmlReader::isGripperParamInLimits(float fGripperPar)
{
    if (fGripperPar >= 3.f && fGripperPar <= 12.f)
        return true;
    else
    {
        qDebug() << "ERROR: XmlReader::isGripperParamInLimits(): one of the gripper "
                    "vals is out of scope (3-12):" << fGripperPar;
        return false;
    }
}
