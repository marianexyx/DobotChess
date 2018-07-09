#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    XmlReader xmlReader001(QDir::currentPath() + "/xmlGameConfig.xml",
                           XFT_GAME_CONFIG);
    Sql::setDbConnectionData();
    RealVars gameConfigVars001 = xmlReader001.getRealVars();
    Clients ClientsList;
    ArduinoUsb USB001;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001);
    Dobot DobotArm001(&USB001, gameConfigVars001,
                      BoardMain001.getBoardPoint3D(BP_RETREAT_LEFT),
                      BoardMain001.getBoardPoint3D(BP_RETREAT_RIGHT));
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Websockets ChessWebsockets001(&ClientsList);
    TCPMsgs TCPMsgs001;
    Chess Chess001(&PieceController001, &BoardChenard001, &ChessWebsockets001,
                   &TCPMsgs001, WEBSITE);
    MainWindow mainwindow(&Chess001);
    mainwindow.show();

    return app.exec();
}
