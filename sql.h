#ifndef SQL_H
#define SQL_H

#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>

//todo: think where to put this class before i will anchor this in program

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
