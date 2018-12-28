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

struct Sql
{
    static QSqlDatabase sqlDB;

    static void setDbConnectionData(DatabaseVars DBV);
    static bool isClientHashOk(QString QStrIDandHash);
    static bool isClientHashOk(int64_t n64sqlId, QString QStrHash);
    static QString getClientNameFromDB(int64_t n64sqlId);
    static bool isLoginAndPassExistInDB(QString QStrLogin, QString QStrPassword);
    static int64_t getClientIdFromDB(QString QStrLogin);
};

#endif // SQL_H
