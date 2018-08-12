#include "errors.h"

Chess *Errors::pChess = nullptr; //static pointer

void Errors::overloadDebugOutput(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg)
{
    //QMutexLocker locker(&mutex); //todo: need to be done with threads in TCPs

    QDateTime dateTime(QDateTime::currentDateTime());
    QString QStrTime(dateTime.toString("HH:mm:ss:z"));
    QByteArray QBaTime = QStrTime.toLocal8Bit();

    QByteArray localMsg = msg.toLocal8Bit();

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
        fprintf(stderr, C_PURPLE "WARNING: [%s %s:%u] %s" C_BLACK "\n",
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        if (Errors::pChess != nullptr)
            Errors::saveErrorInFile(type, context, msg, QStrTime);
        break;
    case QtCriticalMsg:
        fprintf(stderr, C_RED "ERROR: [%s %s:%u] %s" C_BLACK "\n",
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        if (Errors::pChess != nullptr)
            Errors::saveErrorInFile(type, context, msg, QStrTime);
        break;
    case QtFatalMsg:
        fprintf(stderr, CB_RED "FATAL ERROR: [%s %s:%u] %s" C_BLACK "\n",
                QBaTime.constData(), context.function, context.line, localMsg.constData());
        if (Errors::pChess != nullptr)
            Errors::saveErrorInFile(type, context, msg, QStrTime);
        abort();
    }
    fflush(stderr);
}

/*static*/ QString Errors::errorMsg(QtMsgType msgType, const QMessageLogContext &context,
                                    const QString &QStrErrorMsg, QString QStrTime)
{
    QString QStrFunction = QString::fromUtf8(context.function);

    QString QStrFullErrorMsg = "<ERROR TYPE>: " + Errors::errorTypeAsQStr(msgType) + "\n"
            + "<TIME>: " + QStrTime + "\n"
            + "<FUNCTION>: " + QStrFunction + ":" + QString::number(context.line) + "\n"
            + "<ERROR MSG>: " + QStrErrorMsg + "\n"
            + "<APPLICATION ALL DUMPED DATA>:\n" + Errors::getAppDumpData();

    return QStrFullErrorMsg;
}

/*static*/ QString Errors::errorTypeAsQStr(QtMsgType msgType)
{
    QString QStrErrorType = "ERROR: UNKNOWN_ERROR_TYPE";
    if (msgType == QtWarningMsg) QStrErrorType = "WARNING";
    else if (msgType == QtCriticalMsg) QStrErrorType = "ERROR";
    else if (msgType == QtFatalMsg) QStrErrorType = "FATAL ERROR";
    return QStrErrorType;
}

/*static*/ QString Errors::getAppDumpData()
{
    QString QStrAppAllData;
    if (Errors::pChess == nullptr) return "ERROR: Errors::pChess == nullptr";

    QStrAppAllData = Errors::pChess->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getBoardMainPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getBoardRemovedPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getClientsPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getDobotPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getDobotPointer()
            ->getIntermediatePoints().dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getPieceControllerPointer()->dumpAllData() + "\n\n";
    QStrAppAllData += Errors::pChess->getPieceControllerPointer()
            ->getPieceSetPointer()->dumpAllData() + "\n\n";

    return QStrAppAllData;
}

/*static*/ void Errors::saveErrorInFile(QtMsgType msgType, const QMessageLogContext &context,
                                        const QString &QStrErrorMsg, QString QStrTime)
{
    QString QStrFullErrorMsg = Errors::errorMsg(msgType, context, QStrErrorMsg, QStrTime);

    QDateTime dateTime(QDateTime::currentDateTime());
    QString QStrFileTime(dateTime.toString("dd.MM.yyyy HH:mm:ss"));
    QStrFileTime.replace(".", "_"); //dots can't be in file name
    QStrFileTime.replace(QStrFileTime.indexOf(":"), 1, "h");
    QStrFileTime.replace(QStrFileTime.indexOf(":"), 1, "m");
    QStrFileTime += "s";

    QString QStrFileName = QStrFileTime + " " + Errors::errorTypeAsQStr(msgType);
    QFile errorLogFile("errorLogs/" + QStrFileName + ".txt");
    errorLogFile.open(QFile::Append | QFile::Text);
    QTextStream stream(&errorLogFile);
    stream << QStrFullErrorMsg;
    errorLogFile.close();
}
