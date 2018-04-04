#include "xml_reader.h"

XmlReader::XmlReader(QString QStrFileName):
    xmlFile(QStrFileName)
{
    if (!xmlFile.exists())
    {
        qDebug() << "ERROR: XmlReader::XmlReader(): file name:" << QStrFileName
                 << "don't exists";
        return;
    }

    QString errMsg;
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errMsg = xmlFile.errorString();
        qDebug() << "ERROR: XmlReader::XmlReader(): failed to load file:" << errMsg;

        return;
    }
    else
    {
        if(xmlDoc.setContent(&xmlFile))
        {
            qDebug() << "ERROR: XmlReader::XmlReader(): failed to load document";
            xmlFile.close();
            return;
        }
        xmlFile.close();
    }



    /*QDomElement docElem = xmlDoc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            qDebug() << qPrintable(e.tagName()); // the node really is an element.
        }
         qDebug() << "dupa";
        n = n.nextSibling();
    }
    qDebug() << "dupa2";*/



    QDomElement root = xmlDoc.firstChildElement(); //get root element of document = <data>

    this->listElements(root, "points", "point"); //???? x,y,z,????

    QDomNodeList points = root.elementsByTagName("points");
    for (int i=0; i<points.count(); i++)
    {
        QDomNode pointNode = points.at(i);
        //convert to an element
        if (pointNode.isElement())
        {
            //do smtg
        }
    }
}

//roots for listElements are: <piece>, <points>, <gripper>
void XmlReader::listElements(QDomElement root, QString QStrTagName, QString QStrAttribute)
{
    QDomNodeList items = root.elementsByTagName(QStrTagName);

    qDebug() << "XmlReader::listElements(): total items =" << items.count();

    for (int i=0; i<items.count(); ++i)
    {
        QDomNode itemNode = items.at(i); //node can be anything in document (elements
        //...are in tags propably

        //convert to element
        if (itemNode.isElement())
        {
            QDomElement itemEl = itemNode.toElement();
            qDebug() << "XmlReader::listElements(): itemEl.attribute(QStrAttribute) ="
                     << itemEl.attribute(QStrAttribute);
        }
    }
}

RealVars XmlReader::getRealVars()
{
    if (!this->areVarsInLimits())
    {
        qDebug() << "FATAL ERROR: real data out of scope";
        exit(EXIT_FAILURE); //crash app
    }

    return _vars;
}

bool XmlReader::areVarsInLimits()
{
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
    else return false;
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
        qDebug() << "ERROR: XmlReader::isPointInLimits(): one of the points "
                    "vals is out of scope range";
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
