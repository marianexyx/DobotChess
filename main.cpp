#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //todo: niektóre obiekty są pośrednie i mogę być tworzone wewnątrz innych, a potem...
    //...przekazywane dalej
    XmlReader xmlReader001("C:/DobotChess/xmlGameConfig.xml");
    RealVars gameConfigVars001 = xmlReader001.getRealVars();
    Clients ClientsList;
    ArduinoUsb USB001;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001);
    Dobot DobotArm001(&USB001, gameConfigVars001);
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Websockets Websockety(&ClientsList, 1234);
    //TODO: odpalać server websocket po konstruktorze mainwindow
    TCPMsgs TCPMsgs001;
    //todo: jak wrzucam poniżej PieceController001, to masę wskaźników mogę z niego wyciągać
    Chess Chess001(&ClientsList, &DobotArm001, &PieceController001, &BoardMain001,
                   &BoardRemoved001, &BoardChenard001, &Websockety, &TCPMsgs001, WEBSITE);
    MainWindow mainwindow(&Websockety, &PieceController001, &BoardMain001, &BoardRemoved001,
                          &BoardChenard001, &TCPMsgs001, &USB001, &DobotArm001, &Chess001,
                          &ClientsList);
    mainwindow.show();

    return app.exec();
}
