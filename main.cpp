#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebTable WebTable001;
    Websockets Websockety(&WebTable001, 1234); // 1234- na którym porcie websockety mają nasłuchiwać
    MainWindow server(&WebTable001, &Websockety);
    server.show();

    return a.exec();
}

