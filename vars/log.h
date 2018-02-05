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
    switch(Log)
    {
    case LOG_NOTHING: return "nothing";
    case LOG_CORE: return "core";
    case LOG_DOBOT: return "dobot";
    case LOG_TCP: return "tcp";
    case LOG_WEBSOCKET: return "websocket";
    case LOG_MAINWINDOW: return "mainwindow";
    case LOG_USB: return "usb";
    case LOG_USB_SENT: return "usbSent";
    case LOG_USB_RECEIVED: return "usbReceived";
    default:
        qDebug() << "ERROR: logAsQstr(): wrong arg = " << Log;
        return "";
    }
}

#endif /* LOG_H */



