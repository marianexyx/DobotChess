#include <QApplication>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "mainwindow.h"

//future: change all private members names from "_" to "m_"?

int main(int argc, char *argv[])
{
    //todo: can it be below QApplication? should it be?
    qInstallMessageHandler(Errors::overloadDebugOutput);

    QApplication app(argc, argv);

    //todo: object creations should be realised by classes that owns target class?
    XmlReader xmlReader001;
    Sql::setDbConnectionData(xmlReader001.getDatabaseVars());
    RealVars gameConfigVars001(xmlReader001.getRealVars());
    Clients ClientsList;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001);
    //todo: chess class should own IntermediatePointsCalc?
    IntermediatePointsCalc IntermediatePointsCalc001(gameConfigVars001, &BoardRemoved001);
    IntermediatePoints intermediatePoints001 = IntermediatePointsCalc001.getIntermediatePoints();
    Dobot DobotArm001(gameConfigVars001, &intermediatePoints001);
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Websockets ChessWebsockets001(&ClientsList);
    TCPMsgs TCPMsgs001;
    Chess Chess001(&PieceController001, &BoardChenard001, &ChessWebsockets001, &TCPMsgs001);
    MainWindow mainwindow(&Chess001);

    Errors::pChess = &Chess001;
    //qInstallMessageHandler(Errors::overloadDebugOutput);

    mainwindow.show();

    qCritical() << "random crit err msg";

    return app.exec();
}
