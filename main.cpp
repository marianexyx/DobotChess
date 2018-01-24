#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //future: niektóre obiekty są pośrednie i mogę być tworzone wewnątrz innych, a potem...
    //...przekazywane dalej
    Clients ClientsList;
    ArduinoUsb ArduinoUsbEnemysUI;
    Chessboard BoardMain001;
    Chessboard BoardRemoved001;
    Dobot DobotArm001(&ArduinoUsbEnemysUI);
    Websockets Websockety(&ClientsList, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPCommunication;
    Chess Chess001(&ClientsList, &DobotArm001, &BoardMain001, &BoardRemoved001, &TCPCommunication,
                   &ArduinoUsbEnemysUI, &Websockety, &TCPCommunication, WEBSITE);
    MainWindow mainwindow(&Websockety, &BoardMain001, &BoardRemoved001, &TCPCommunication,
                      &ArduinoUsbEnemysUI, &DobotArm001, &Chess001, ClientsList);
    mainwindow.show();

    return app.exec();
}

