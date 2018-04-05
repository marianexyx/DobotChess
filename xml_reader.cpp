#include "xml_reader.h"

XmlReader::XmlReader(QString QStrFileName):
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
                _vars.fPieceHeight = param.attribute("val").toFloat();
            else qDebug() << "ERROR: XmlReader::readPiece(): param name != height";
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
                _vars.A1.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldA8")
                _vars.A8.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldH1")
                _vars.H1.setPoint(p3d);
            else if (p.attribute("name") == "mainFieldH8")
                _vars.H8.setPoint(p3d);
            else if (p.attribute("name") == "remWhiteCloserOuter")
                _vars.remWhiteCloserOuter.setPoint(p3d);
            else if (p.attribute("name") == "remWhiteFurtherInner")
                _vars.remWhiteFurtherInner.setPoint(p3d);
            else if (p.attribute("name") == "remBlackCloserOuter")
                _vars.remBlackCloserOuter.setPoint(p3d);
            else if (p.attribute("name") == "remBlackFurtherInner")
                _vars.remBlackFurtherInner.setPoint(p3d);
            else if (p.attribute("name") == "home")
                _vars.home.setPoint(p3d);
            else if (p.attribute("name") == "homeToMiddleAbove")
                _vars.homeToMiddleAbove.setPoint(p3d);
            else qDebug() << "ERROR: XmlReader::readPoints(): unknown param name ="
                          << p.attribute("name");
        }
    }
}

void XmlReader::readGripper(QDomNodeList gripper)
{
    //qDebug() << "gripper amount =" << gripper.count();
    for(int i=0; i<gripper.count(); i++)
    {
        QDomNode stateNode = gripper.at(i);
        if (stateNode.isElement())
        {
            QDomElement state = stateNode.toElement();

            if (state.attribute("name") == "open")
                _vars.fGripperOpened = state.attribute("pwm").toFloat();
            else if (state.attribute("name") == "close")
                _vars.fGripperClosed = state.attribute("pwm").toFloat();
            else qDebug() << "ERROR: XmlReader::readGripper(): unknown state name ="
                          << state.attribute("name");
        }
    }
}

bool XmlReader::isVarsStructInLimits()
{
    //todo: wprowdzić kilka dodatkowych warunków sprawdzających
    if (this->isPieceHeightInLimits(_vars.fPieceHeight) &&
            this->isPointInLimits(_vars.home) &&
            this->isPointInLimits(_vars.homeToMiddleAbove) &&
            this->isPointInLimits(_vars.A1) &&
            this->isPointInLimits(_vars.A8) &&
            this->isPointInLimits(_vars.H1) &&
            this->isPointInLimits(_vars.H8) &&
            this->isPointInLimits(_vars.remWhiteCloserOuter) &&
            this->isPointInLimits(_vars.remWhiteFurtherInner) &&
            this->isPointInLimits(_vars.remBlackCloserOuter) &&
            this->isPointInLimits(_vars.remBlackFurtherInner) &&
            this->isGripperParamInLimits(_vars.fGripperOpened) &&
            this->isGripperParamInLimits(_vars.fGripperClosed))
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

bool XmlReader::isPointInLimits(Point3D point)
{
    Point3D minErrorCorner(100.f, -200.f, -30.f);
    Point3D maxErrorCorner(320.f, 200.f, 70.f);

    if (point.x > minErrorCorner.x && point.x < maxErrorCorner.x &&
            point.y > minErrorCorner.y && point.y < maxErrorCorner.y &&
            point.z > minErrorCorner.z && point.z < maxErrorCorner.z)
        return true;
    else
    {
        qDebug() << "ERROR: XmlReader::isPointInLimits(): one of the points  vals is "
                    " out of scope range. point =" << point.x << point.y << point.z;
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
