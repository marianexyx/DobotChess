#include "sql.h"

sql::sql()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("mysql.cba.pl");
    db.setDatabaseName("budgames_cba_pl");
    db.setUserName("marianexyx");
    db.setPassword("1qazse4rfvgy7SX");


}

void sql::test()
{
    //http://szymonsiarkiewicz.pl/poradniki/kurs-qt/kurs-qt-komunikacja-z-baza-danych/
    //https://www.youtube.com/watch?v=LiHHm7cd5Bs&list=PL2D1942A4688E9D63&t=26

    /*if(!db.open())
        qDebug() << "Nieudane polaczenie z baza danychn";
    else
    {
        QStringList tables = db.tables();
        for(int i=0;i<tables.size();i++)
            qDebug() << tables.at(i);

        QSqlQuery pobieranie;
        if(!pobieranie.exec("SELECT * FROM Stats")) //exec wykonuje zapytanie do bazy
            qDebug() << "Brak rekordów w tabeli Stats";

        while(pobieranie.next())
        {
            int d = pobieranie.value("Deaths").toInt(); //nazwa kolumny
            int k = pobieranie.value(2).toInt(); //nr kolumny licząc od 0

            qDebug() << k << " / " << d;
        }

        db.close();
    }*/
}

void sql::connect()
{

}


