#include "sql.h"

/*static*/ QSqlDatabase Sql::sqlDB;

/*static*/ void Sql::setDbConnectionData(DatabaseVars DBV)
{
    qInfo() << "vars =" << DBV.QStrHostName << DBV.QStrDatabaseName
            << DBV.QStrUserName << DBV.QStrPassword;

    sqlDB = QSqlDatabase::addDatabase("QMYSQL");
    sqlDB.setHostName(DBV.QStrHostName);
    sqlDB.setDatabaseName(DBV.QStrDatabaseName);
    sqlDB.setUserName(DBV.QStrUserName);
    sqlDB.setPassword(DBV.QStrPassword);
}

/*static*/ QString Sql::getClientNameFromDB(int64_t n64sqlId)
{
    if (n64sqlId <= 0)
    {
        qCritical() << "given sqlID <= 0. its =" << QString::number(n64sqlId);
        return "";
    }

    if (sqlDB.open())
    {
        QString QStrQuery = "SELECT * FROM users WHERE id = "
                + QString::number(n64sqlId);

        QSqlQuery query;
        if (!query.exec(QStrQuery)) //exec is making query for database
        {
            qCritical() << "no records in users table";
            return "";
        }

        if (query.size() != 1)
        {
            qCritical() << "query.size() != 1. it's =" << QString::number(query.size());
            return "";
        }

        QString QStrDbLogin;
        while(query.next())
            QStrDbLogin = query.value("login").toString();

        sqlDB.close();

        return QStrDbLogin;
    }
    else
    {
        qCritical() << "failed to connect with SQL DT. err =" << sqlDB.lastError().text();
        return "";
    }
}

/*static*/ bool Sql::isLoginAndPassExistInDB(QString QStrLogin, QString QStrPassword)
{
    if (QStrLogin.length() < 3 || QStrLogin.length() > 25 ||
            QStrPassword.length() < 1 || QStrPassword.length() > 20)
        return false;

    if (sqlDB.open())
    {
        //todo: "Using SELECT * is not recommended because the order of...
        //...the fields in the query is undefined."
        QString QStrQuery = "SELECT * FROM users WHERE login = :login AND password = :password";
        QSqlQuery query;
        query.prepare(QStrQuery);
        query.bindValue(":login", QStrLogin);
        query.bindValue(":password", QStrPassword);
        if (query.exec())
            return true;
        else return false;
    }
    else
    {
        qCritical() << "failed to connect with SQL DT. err =" << sqlDB.lastError().text();
        return false;
    }
}

/*static*/ int64_t Sql::getClientIdFromDB(QString QStrLogin)
{
    if (QStrLogin.length() < 3 || QStrLogin.length() > 25)
    {
        qCritical() << "wrong login length =" << QString::number(QStrLogin.length());
        return 0;
    }

    if (sqlDB.open())
    {
        QString QStrQuery = "SELECT * FROM users WHERE login = :login";
        QSqlQuery query;
        query.prepare(QStrQuery);
        query.bindValue(":login", QStrLogin);
        if (!query.exec()) //exec is making query for database
        {
            qCritical() << "no records in users table";
            return 0;
        }

        if (query.size() != 1)
        {
            qCritical() << "query.size() != 1. it's =" << QString::number(query.size());
            return 0;
        }

        int64_t n64Id;
        while(query.next())
            n64Id = query.value("id").toLongLong();

        sqlDB.close();

        return n64Id;
    }
    else
    {
        qCritical() << "failed to connect with SQL DT. err =" << sqlDB.lastError().text();
        return 0;
    }
}
