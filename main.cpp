#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //todo: niektóre obiekty są pośrednie i mogę być tworzone wewnątrz innych, a potem...
    //...przekazywane dalej
    Clients ClientsList;
    ArduinoUsb USB001;
    PieceController PieceController001;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, IMAGINARY);
    Chessboard BoardRemoved001(B_REMOVED, IMAGINARY);
    Dobot DobotArm001(&USB001);
    Websockets Websockety(&ClientsList, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPMsgs001;
    Chess Chess001(&ClientsList, &PieceController001, &DobotArm001, &BoardMain001,
                   &BoardRemoved001, &BoardChenard001, &Websockety, &TCPMsgs001, WEBSITE);
    MainWindow mainwindow(&Websockety, &BoardMain001, &BoardRemoved001, &BoardChenard001,
                          &TCPMsgs001, &USB001, &DobotArm001, &Chess001, &ClientsList);
    mainwindow.show();

    return app.exec();
}

