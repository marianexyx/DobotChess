#ifndef ERRORS_H
#define ERRORS_H

#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>
#include <QFile>
#include <QTimer>
#include "chess/chess.h"
#include "intermediate_points_calculator.h"

//colors
#define C_BLACK "\e[0;30m"
#define C_RED "\e[0;31m"
#define C_BLUE "\e[0;34m"
#define C_PURPLE "\e[0;35m"
#define C_CYAN "\e[0;36m"
#define CB_BLACK "\e[1;30m"
#define CB_RED "\e[1;31m"

class Errors: public QObject
{
    Q_OBJECT

private:
    static const QDir m_errorDir;

public:
    static uint newErrors;
    static uint newWarnings;
    Errors() {}

    static Chess* pChess; //overloadDebugOutput() can work only on statics

    static void overloadDebugOutput(QtMsgType type, const QMessageLogContext &context,
                                    const QString &msg);
    static QString errorMsg(QtMsgType msgType, const QMessageLogContext &context,
                            const QString &QStrMsg, QString QStrTime);
    static QString errorTypeAsQStr(QtMsgType msgType);
    static void saveErrorInFile(QtMsgType msgType, const QMessageLogContext &context,
                                QString QStrErrorMsg, QString QStrTime);
    static QString getAppDumpData();
};

#endif // ERRORS_H
