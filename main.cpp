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
    Chessboard BoardRealMain001(B_MAIN, REAL);
    Chessboard BoardRealRemoved001(B_REMOVED, REAL);
    Chessboard BoardCoreMain001(B_MAIN, IMAGINARY);
    Chessboard BoardCoreRemoved001(B_REMOVED, IMAGINARY);
    Dobot DobotArm001(&USB001);
    Websockets Websockety(&ClientsList, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPMsgs001;
    Chess Chess001(&ClientsList, &PieceController001, &DobotArm001, &BoardRealMain001,
                   &BoardRealRemoved001, &BoardCoreMain001, &BoardCoreRemoved001,
                   &BoardChenard001, &Websockety, &TCPMsgs001, WEBSITE);
    MainWindow mainwindow(&Websockety, &BoardRealMain001, &BoardRealRemoved001, &BoardChenard001,
                          &TCPMsgs001, &USB001, &DobotArm001, &Chess001, &ClientsList);
    mainwindow.show();

    return app.exec();
}

