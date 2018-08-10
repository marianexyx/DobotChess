#include "errors.h"

Chess *Errors::pChess = nullptr; //static pointer

Errors::Errors()
{
    //_pIntermediatePointsCalc = pIntermediatePointsCalc; //todo:
}

void Errors::overloadDebugOutput(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg)
{
    //QMutexLocker locker(&mutex);

    QDateTime dateTime(QDateTime::currentDateTime());
    QString timeStr(dateTime.toString("HH:mm:ss:z"));
    QByteArray QBaTime = timeStr.toLocal8Bit();

    QByteArray localMsg = msg.toLocal8Bit();

    //QString QStrErrorMsg = errorMsg(type, context, msg, timeStr, )

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, CB_BLACK "[" C_BLACK "%s " C_CYAN "%s:%u" CB_BLACK "]" C_BLACK " %s\n",
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, CB_BLACK "[" C_BLACK "%s " C_BLUE "%s:%u" CB_BLACK "]" C_BLACK " %s\n",
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, C_PURPLE "WARNING: [%s %s:%u] %s\n" C_BLACK,
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        if (Errors::pChess != nullptr)
            Errors::saveErrorInFile(Errors::errorMsg(type, context, msg, timeStr));
        break;
    case QtCriticalMsg:
        fprintf(stderr, C_RED "ERROR: [%s %s:%u] %s\n" C_BLACK,
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        if (Errors::pChess != nullptr)
            Errors::saveErrorInFile(Errors::errorMsg(type, context, msg, timeStr));
        break;
    case QtFatalMsg:
        fprintf(stderr, CB_RED "FATAL ERROR: [%s %s:%u] %s\n" C_BLACK,
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        abort();
    }
    fflush(stderr);
}

/*static*/ QString Errors::errorMsg(QtMsgType msgType, const QMessageLogContext &context,
                                    const QString &QStrMsg, QString QStrTime)
{
    QString QStrErrorType;
    if (msgType == QtWarningMsg) QStrErrorType = "WARNING";
    else if (msgType == QtCriticalMsg) QStrErrorType = "ERROR";
    else if (msgType == QtFatalMsg) QStrErrorType = "FATAL ERROR";

    QString QStrFunction = QString::fromUtf8(context.function);

    QString QStrErrorMsg = "ERROR TYPE: " + QStrErrorType + "\n"
            + "TIME: " + QStrTime + "\n"
            + "FUNCTION" + QStrFunction + ":" + QString::number(context.line) + "\n"
            + "ERROR MSG: " + QStrMsg + "\n"
            + "APPLICATION ALL DUMPED DATA:\n" + Errors::getAppDumpData();

    return QStrErrorMsg;
}

/*static*/ QString Errors::getAppDumpData()
{
    QString QStrAppAllData;
    if (Errors::pChess == nullptr) return "ERROR: Errors::pChess == nullptr";

    QStrAppAllData = Errors::pChess->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getBoardMainPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getBoardRemovedPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getBoardRemovedPointer()->dumpAllData() + "\n\n";
    //piece
    QStrAppAllData += Errors::pChess->getClientsPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getDobotPointer()->dumpAllData() + "\n\n";
    //QStrAppAllData += _pIntermediatePointsCalc->dumpAllData() + "\n\n"; //todo:
    QStrAppAllData += Errors::pChess->getPieceControllerPointer()
            ->getLastPos().getAsQStr() + "\n\n";
    QStrAppAllData += Errors::pChess->getPieceControllerPointer()
            ->getPieceSetPointer()->dumpAllData() + "\n\n";

    return QStrAppAllData;
}

/*static*/ void Errors::saveErrorInFile(QString QStrErrorMsg)
{
    QFile errorLogFile("logFile.txt");
    errorLogFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream stream(&errorLogFile);
    stream << QStrErrorMsg << endl;
}

