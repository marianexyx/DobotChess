#ifndef SQL_H
#define SQL_H

#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <xml_reader.h>

static QSqlDatabase sqlDB;

struct Sql
{
    static void setDbConnectionData();
    static bool isClientHashOk(int64_t n64sqlId, QString QStrHash);
    static bool isClientHashOk(QString QStrIDandHash);
    static QString getClientName(int64_t n64sqlId);
};

#endif // SQL_H
