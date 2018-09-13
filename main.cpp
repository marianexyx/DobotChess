#include <QApplication>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "mainwindow.h"

typedef unsigned short ushort;
typedef unsigned long ulong;

int main(int argc, char *argv[])
{
    qInstallMessageHandler(Errors::overloadDebugOutput);

    QApplication app(argc, argv);

    XmlReader xmlReader001;
    Sql::setDbConnectionData(xmlReader001.getDatabaseVars());
    const RealVars gameConfigVars001(xmlReader001.getRealVars());
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001.boards);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001.boards);
    const IntermediatePoints intermediatePoints001 =
            IntermediatePointsCalc::calculate(gameConfigVars001, &BoardRemoved001);
    Dobot DobotArm001(gameConfigVars001.dobot, intermediatePoints001);
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Chess Chess001(&PieceController001);
    MainWindow mainwindow(&Chess001);
    Errors::pChess = &Chess001;
    Errors errors;

    mainwindow.show();

    Chess001.getWebsocketsPointer()->listenOnPort(1234);

    return app.exec();
}
