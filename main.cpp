#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ArduinoUsb ArduinoUsbEnemysUI;
    Chessboard ChessboardDobot001;
    Dobot DobotArm001(&ChessboardDobot001, &ArduinoUsbEnemysUI);
    WebTable WebTable001;
    Websockets Websockety(&ChessboardDobot001, &WebTable001, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;

    /*Chess ChessCore(&DobotArm001, &ChessboardDobot001, &TCPCommunication,
                    &Websockety, &WebTable001, &ArduinoUsbEnemysUI);*/
    //TODO: IgorBot i webchess tworzyć jako obiekty z parametrem a nie robić z tego oddzielnych klas
    IgorBot IgorBotAI(&DobotArm001, &ChessboardDobot001, &TCPCommunication,
                       &WebTable001, &ArduinoUsbEnemysUI);
    WebChess WebChess001(&DobotArm001, &ChessboardDobot001, &TCPCommunication,
                         &WebTable001, &Websockety);
    MainWindow mainwindow(&WebTable001, &Websockety, &ChessboardDobot001, &TCPCommunication,
                      &ArduinoUsbEnemysUI, &DobotArm001, &IgorBotAI, &WebChess001);
    mainwindow.show();

    return a.exec();
}

