#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Clients ClientsList;
    ArduinoUsb ArduinoUsbEnemysUI;
    Chessboard ChessboardDobot001;
    Dobot DobotArm001(&ChessboardDobot001, &ArduinoUsbEnemysUI);
    Websockets Websockety(&ChessboardDobot001, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;
    Chess Chess001(&ClientsList, &DobotArm001, &ChessboardDobot001, &TCPCommunication,
                   &ArduinoUsbEnemysUI, &Websockety, WEBSITE); //TODO
    MainWindow mainwindow(&Websockety, &ChessboardDobot001, &TCPCommunication,
                      &ArduinoUsbEnemysUI, &DobotArm001, &Chess001);
    mainwindow.show();

    return app.exec();
}

