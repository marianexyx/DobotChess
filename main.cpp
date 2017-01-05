#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dobot DobotArm001;
    /*DobotArm001.setPeriodicTaskTimer();
    DobotArm001.getPoseTimer();*/
    Chessboard ChessboardDobot001;
    WebTable WebTable001;
    Websockets Websockety(&WebTable001, 1234); // 1234- na którym porcie websockety mają nasłuchiwać
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;
    Chess ChessCore(&DobotArm001, &ChessboardDobot001, &TCPCommunication, &Websockety, &WebTable001);

    MainWindow mainwindow(&WebTable001, &Websockety, &ChessboardDobot001,
                      &TCPCommunication, &DobotArm001, &ChessCore);
    mainwindow.show();

    return a.exec();
}

