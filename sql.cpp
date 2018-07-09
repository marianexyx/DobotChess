#include "sql.h"

/*static*/ void Sql::setDbConnectionData()
{
    qDebug() << "Sql::setDbConnectionData()";

    XmlReader xmlDB(QDir::currentPath() +
                    "/../DobotChessDatabase/mysqlDatabaseConnectionData.xml",
                    XFT_DATABASE);
    DatabaseVars DBVars = xmlDB.getDatabaseVars();

    qDebug() << "Sql::setDbConnectionData(): vars ="
             << DBVars.QStrHostName << DBVars.QStrDatabaseName
             << DBVars.QStrUserName << DBVars.QStrPassword;

    sqlDB = QSqlDatabase::addDatabase("QMYSQL");
    sqlDB.setHostName(DBVars.QStrHostName);
    sqlDB.setDatabaseName(DBVars.QStrDatabaseName);
    sqlDB.setUserName(DBVars.QStrUserName);
    sqlDB.setPassword(DBVars.QStrPassword);
}

/*static*/ bool Sql::isClientHashOk(int64_t n64sqlId, QString QStrHash)
{
    if (sqlDB.open())
    {
        QStringList tables = sqlDB.tables();
        for(int i=0; i<tables.size(); i++)
            qDebug() << "Sql::test(): tables =" << tables.at(i);

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

/* static */ QString Sql::getClientName(int64_t n64sqlId)
{
    QStringList tables = sqlDB.tables();
    for(int i=0; i<tables.size(); i++)
        qDebug() << "Sql::test(): tables =" << tables.at(i);

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

        sqlDB.close();
        return query.value("login").toString();
    }
    else
    {
        qDebug() << "ERROR: Sql::test(): Failed to connect with SQL database. "
                     "error =" << sqlDB.lastError().text();
        return "";
    }
}
