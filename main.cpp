#include "mainwindow.h"
#include <QApplication>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>

//future: change all private members names from "_" to "m_"?

// COLORS
#define C_NORMAL  "\e[0m"
#define C_BLACK   "\e[0;30m"
#define C_GREEN   "\e[0;32m"
#define CB_GREEN  "\e[1;32m"
#define C_CYAN    "\e[0;36m"
#define CB_CYAN   "\e[1;36m"
#define C_RED     "\e[0;31m"
#define CB_RED    "\e[1;31m"

//todo: end it
//todo: move it to error class
void myMessageOutput(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString timeStr(dateTime.toString("HH:mm:ss:z"));
    QByteArray QBaTime = timeStr.toLocal8Bit();

    QByteArray localMsg = msg.toLocal8Bit();


    //todo: opening the file (do it only once!):
    //https://stackoverflow.com/questions/29449237/qt5-how-to-use-qdebug-to-log-in-a-file-multi-thread-application

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
                context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, C_GREEN "Info:" C_CYAN " %s" C_BLACK "(%s:%u, %s) time:%s\n", localMsg.constData(),
                context.file, context.line, context.function, QBaTime.constData());
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
    fflush(stderr);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);

    //todo: tests
    qDebug() << "debug output";
    qInfo() << "info output";
    qWarning() << "warning output";
    qCritical() << "critical output";
    //qFatal("fatal output");

    XmlReader xmlReader001;
    Sql::setDbConnectionData(xmlReader001.getDatabaseVars());
    RealVars gameConfigVars001(xmlReader001.getRealVars());
    Clients ClientsList;
    Chessboard BoardChenard001(B_MAIN, IMAGINARY);
    Chessboard BoardMain001(B_MAIN, REAL, gameConfigVars001);
    Chessboard BoardRemoved001(B_REMOVED, REAL, gameConfigVars001);
    IntermediatePointsCalc IntermediatePointsCalc001(gameConfigVars001, &BoardRemoved001);
    IntermediatePoints intermediatePoints001 = IntermediatePointsCalc001.getIntermediatePoints();
    Dobot DobotArm001(gameConfigVars001, &intermediatePoints001);
    PieceController PieceController001(&DobotArm001, &BoardMain001, &BoardRemoved001);
    Websockets ChessWebsockets001(&ClientsList);
    TCPMsgs TCPMsgs001;
    Chess Chess001(&PieceController001, &BoardChenard001, &ChessWebsockets001,
                   &TCPMsgs001);
    MainWindow mainwindow(&Chess001);
    mainwindow.show();

    return app.exec();
}
