#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Chessboard ChessboardDobot001;
    Dobot DobotArm001(&ChessboardDobot001);
    WebTable WebTable001;
    Websockets Websockety(&WebTable001, 1234); // 1234- na którym porcie websockety mają nasłuchiwać
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;
    ArduinoUsb ArduinoUsbEnemysUI;
    Chess ChessCore(&DobotArm001, &ChessboardDobot001, &TCPCommunication, &Websockety, &WebTable001);

    MainWindow mainwindow(&WebTable001, &Websockety, &ChessboardDobot001,
                      &TCPCommunication, &ArduinoUsbEnemysUI, &DobotArm001, &ChessCore);
    mainwindow.show();

    return a.exec();
}

