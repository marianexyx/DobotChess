#ifndef LOG_H
#define LOG_H

#pragma once
#include <QString>

enum LOG
{
    LOG_NOTHING,
    LOG_CORE,
    LOG_DOBOT,
    LOG_TCP,
    LOG_WEBSOCKET,
    LOG_MAINWINDOW,
    LOG_USB,
    LOG_USB_SENT,
    LOG_USB_RECEIVED
};

inline QString logAsQstr(LOG Log)
{
    QString QStrLog;
    switch(Log)
    {
    case LOG_NOTHING: QStrLog = "nothing";
    case LOG_CORE: QStrLog = "core";
    case LOG_DOBOT: QStrLog = "dobot";
    case LOG_TCP: QStrLog = "tcp";
    case LOG_WEBSOCKET: QStrLog = "websocket";
    case LOG_MAINWINDOW: QStrLog = "mainwindow";
    case LOG_USB: QStrLog = "usb";
    case LOG_USB_SENT: QStrLog = "usbSent";
    case LOG_USB_RECEIVED: QStrLog = "usbReceived";
    default:
        qDebug() << "ERROR: logAsQstr(): wrong arg = " << Log;
        return "";
    }
    return "<" + QStrLog + ">: ";
}

#endif /* LOG_H */



