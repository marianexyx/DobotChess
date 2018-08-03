#include "sql.h"

/*static*/ QSqlDatabase Sql::sqlDB;

/*static*/ void Sql::setDbConnectionData(DatabaseVars DBV)
{
    qDebug() << "Sql::setDbConnectionData(): vars =" << DBV.QStrHostName
             << DBV.QStrDatabaseName << DBV.QStrUserName << DBV.QStrPassword;

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
        qDebug() << "ERROR: Sql::isClientHashOk(): ID param cannot be below 1. "
                    "it's ==" << n64sqlId;
        return false;
    }
    qDebug() << "Sql::isClientHashOk(): n64sqlId =" << n64sqlId;

    if (sqlDB.open())
    {
        QString QStrQuery = "SELECT * FROM users WHERE id = "
                + QString::number(n64sqlId);

        QSqlQuery query;
        if (!query.exec(QStrQuery)) //exec is making query for database
        {
            qDebug() << "ERROR: Sql::isClientHashOk(): No records in users table "
                        "for ID =" << QString::number(n64sqlId);
            return false;
        }

        if (query.size() != 1)
        {
            qDebug() << "ERROR: Sql::isClientHashOk(): query.size() != 1. it's ="
                     << query.size();
            return false;
        }

        while(query.next())
        {
            QString QStrDbHash = query.value("hash").toString();
            if (QStrDbHash != QStrHash)
            {
                qDebug() << "ERROR: Sql::isClientHashOk(): hash parameter ("
                            + QStrHash + ") is diffrent then hash from DB ("
                         << QStrDbHash + ")";
                return false;
            }
        }

        sqlDB.close();
        return true;
    }
    else
    {
        qDebug() << "ERROR: Sql::isClientHashOk(): Failed to connect with "
                    "SQL database. Error =" << sqlDB.lastError().text();
        return false;
    }
}

/*static*/ bool Sql::isClientHashOk(QString QStrIDandHash)
{
    int64_t n64ID = QStrIDandHash.left(QStrIDandHash.indexOf("&")).toInt();
    QString QStrHash = QStrIDandHash.mid(QStrIDandHash.indexOf("&")+1);

    return Sql::isClientHashOk(n64ID, QStrHash);
}

/*static*/ QString Sql::getClientName(int64_t n64sqlId)
{
    if (n64sqlId <= 0)
    {
        qDebug() << "ERROR: Sql::getClientName(): given ID < 1. its =" << n64sqlId;
        return "";
    }

    if (sqlDB.open())
    {
        QString QStrQuery = "SELECT * FROM users WHERE id = "
                + QString::number(n64sqlId);

        QSqlQuery query;
        if(!query.exec(QStrQuery)) //exec is making query for database
        {
            qDebug() << "ERROR: Sql::getClientName(): No records in users table";
            return "";
        }

        if (query.size() != 1)
        {
            qDebug() << "ERROR: Sql::getClientName(): query.size() != 1. it's ="
                     << query.size();
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
        qDebug() << "ERROR: Sql::getClientName(): Failed to connect with SQL database. "
                     "error =" << sqlDB.lastError().text();
        return "";
    }
}
