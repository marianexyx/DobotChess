#include "xml_reader.h"

XmlReader::XmlReader(QString QStrFileName):
    xmlFile(QStrFileName)
{
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: XmlReader::XmlReader(): failed to load file";
        return;
    }
    else
    {
        if(xmlDoc.setContent(&xmlFile))
        {
            qDebug() << "ERROR: XmlReader::XmlReader(): failed to load document";
            return;
        }
        xmlFile.close();
    }

    QDomElement root  = xmlDoc.firstChildElement(); //get root element of document

    this->ListElements(root, "point", "name"); //???? x,y,z,????

    //test it
    QDomNodeList points = root.elementsByTagName("point");
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

void XmlReader::ListElements(QDomElement root, QString QStrTagName, QString QStrAttribute)
{
    QDomNodeList items = root.elementsByTagName(QStrTagName);

    qDebug() << "XmlReader::ListElements(): total items =" << items.count();

    for (int i=0; i<items.count(); i++)
    {
        QDomNode itemNode = items.at(i); //node can be anything in document (elements are in...
        //...tags propably

        //convert to element
        if (itemNode.isElement())
        {
            QDomElement itemEl = itemNode.toElement();
            qDebug() << "XmlReader::ListElements(): itemEl.attribute(QStrAttribute) ="
                     << itemEl.attribute(QStrAttribute);
        }
    }
}







