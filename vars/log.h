#ifndef LOG_H
#define LOG_H

#pragma once
#include <QString>
#include <QDebug>

enum LOG
{
    LOG_NOTHING,
    LOG_CORE,
    LOG_DOBOT,
    LOG_TCP,
    LOG_WEBSOCKET,
    LOG_MAINWINDOW
};

inline QString logAsQstr(LOG Log)
{
    QString QStrLog;
    switch(Log)
    {
    case LOG_NOTHING: QStrLog = "nothing"; break;
    case LOG_CORE: QStrLog = "core"; break;
    case LOG_DOBOT: QStrLog = "dobot"; break;
    case LOG_TCP: QStrLog = "TCP"; break;
    case LOG_WEBSOCKET: QStrLog = "websocket"; break;
    case LOG_MAINWINDOW: QStrLog = "mainwindow"; break;
    default:
        qCritical() << "wrong arg = " << Log;
        return "";
    }
    return "<" + QStrLog + ">: ";
}

#endif /* LOG_H */



