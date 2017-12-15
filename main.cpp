#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ArduinoUsb ArduinoUsbEnemysUI;
    Chessboard ChessboardDobot001;
    Dobot DobotArm001(&ChessboardDobot001, &ArduinoUsbEnemysUI);
    Websockets Websockety(&ChessboardDobot001, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;

    //TODO: IgorBot i webchess tworzyć jako obiekty z parametrem a nie robić z tego oddzielnych klas
    /*IgorBot IgorBotAI(&DobotArm001, &ChessboardDobot001, &TCPCommunication,
                       &ArduinoUsbEnemysUI);
    WebChess WebChess001(&DobotArm001, &ChessboardDobot001, &TCPCommunication,
                         &Websockety);*/
    Chess Chess001(&DobotArm001, &ChessboardDobot001, &TCPCommunication, &ArduinoUsbEnemysUI,
                   &Websockety, WEBSITE); //TODO

    MainWindow mainwindow(&Websockety, &ChessboardDobot001, &TCPCommunication,
                      &ArduinoUsbEnemysUI, &DobotArm001, &IgorBotAI, &WebChess001);
    mainwindow.show();

    return a.exec();
}

