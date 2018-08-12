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

/*static*/ bool Sql::isClientHashOk(int64_t n64sqlId, QString QStrHash)
{    
    if (n64sqlId < 1)
    {
        qCritical() << "ID param cannot be below 1. it's ==" << QString::number(n64sqlId);
        return false;
    }
    qInfo() << "n64sqlId =" << QString::number(n64sqlId);

    if (sqlDB.open())
    {
        QString QStrQuery = "SELECT * FROM users WHERE id = "
                + QString::number(n64sqlId);

        QSqlQuery query;
        if (!query.exec(QStrQuery)) //exec is making query for database
        {
            qCritical() << "no records in users table for ID =" << QString::number(n64sqlId);
            return false;
        }

        if (query.size() != 1)
        {
            qCritical() << "query.size() != 1. it's =" << QString::number(query.size());
            return false;
        }

        while(query.next())
        {
            QString QStrDbHash = query.value("hash").toString();
            if (QStrDbHash != QStrHash)
            {
                qCritical() << "hash parameter (" + QStrHash +
                               ") is diffrent then hash from DB:" << QStrDbHash;
                return false;
            }
        }

        sqlDB.close();
        return true;
    }
    else
    {
        qCritical() << "failed to connect with SQL DB. err =" << sqlDB.lastError().text();
        return false;
    }
}

/*static*/ bool Sql::isClientHashOk(QString QStrIDandHash)
{
    int64_t n64ID = QStrIDandHash.left(QStrIDandHash.indexOf("&")).toInt();
    QString QStrHash = QStrIDandHash.mid(QStrIDandHash.indexOf("&")+1);

    return Sql::isClientHashOk(n64ID, QStrHash);
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
        if(!query.exec(QStrQuery)) //exec is making query for database
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
