#include "mainwindow.h"
#include <QApplication>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>

//todo:
//stackoverflow.com/questions/4954140/how-to-redirect-qdebug-qwarning-qcritical-etc-output
/*void myMessageOutput(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        abort();
    }
}*/

//todo: change all private members names from "_" to "m_"

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput); // Install the handler
    qSetMessagePattern("%{function}(%{line}): %{message}");
    QApplication app(argc, argv);

    XmlReader xmlReader001;
    Sql::setDbConnectionData(xmlReader001.getDatabaseVars());
    RealVars gameConfigVars001(xmlReader001.getRealVars());
    Clients ClientsList;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001);
    Dobot DobotArm001(gameConfigVars001,
                      BoardMain001.getBoardPoint3D(BP_RETREAT_LEFT),
                      BoardMain001.getBoardPoint3D(BP_RETREAT_RIGHT));
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Websockets ChessWebsockets001(&ClientsList);
    TCPMsgs TCPMsgs001;
    Chess Chess001(&PieceController001, &BoardChenard001, &ChessWebsockets001,
                   &TCPMsgs001);
    MainWindow mainwindow(&Chess001);
    mainwindow.show();

    return app.exec();
}
