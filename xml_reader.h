#ifndef XML_READER_H
#define XML_READER_H

#pragma once
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <QFile>

//https://www.youtube.com/watch?v=NzQwJdcdRKE&index=95&list=PL2D1942A4688E9D63

//todo: nic nie testowane
class XmlReader
{
private:
    QDomDocument xmlDoc;
    QFile xmlFile;

public:
    XmlReader(QString QStrFileName);

    void ListElements(QDomElement root, QString QStrTagName, QString QStrAttribute);
};

#endif // XML_READER_H
