#ifndef SQL_H
#define SQL_H

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>

//todo: zastanowić sę gdzie umieścić tą klasęm zanim na dobre zakotwiczę ją w programie

class sql
{

private:
    QSqlDatabase db;

public:
    sql();

    void test();
    void connect();

};

#endif // SQL_H
