#ifndef LOG_H
#define LOG_H

enum LOG //todo: we wszystkich wyliczanakch dodać prefix, np LOG_NOTHING
{
    NOTHING,
    CORE,
    DOBOT,
    TCP,
    WEBSOCKET,
    MAINWINDOW,
    USB,
    USB_SENT,
    USB_RECEIVED
};

#endif /* LOG_H */

