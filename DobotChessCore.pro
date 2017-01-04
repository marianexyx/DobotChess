#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T09:59:06
#
#-------------------------------------------------

QT       += core gui 
QT       += network 
QT       += xml 
QT       += serialport 
QT       += printsupport
QT       += core websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotChessCore
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    chess.cpp \
    dobot.cpp \
    tcpmsgs.cpp \
    #debug/moc_mainwindow.cpp \
    #debug/moc_Thread.cpp \
    webtable.cpp \
    chessboard.cpp \
    websockets.cpp \
    workerThread.cpp

HEADERS  +=  webtable.h \
    websockets.h \
    tcpmsgs.h \
    chessboard.h \
    dobot.h \
    chess.h \
    DobotDll/DobotDll.h \
    DobotDll/DobotType.h \
    mainwindow.h \
    ui_mainwindow.h \
    workerThread.h


FORMS    += mainwindow.ui

DESTDIR = ./DobotDll

INCLUDEPATH += $$DESTDIR

CONFIG(debug, debug | release) {
    LIBS += -L$$DESTDIR/ -lDobotDll
} else {
    LIBS += -L$$DESTDIR/ -lDobotDll
}

DISTFILES += \
    DobotDll/DobotDll.lib \
    DobotDll/DobotDemoForQt.exe \
    DobotDll/DobotDll.dll \
    DobotDll/msvcp120.dll \
    DobotDll/msvcr120.dll \
    DobotDll/Qt5Core.dll \
    DobotDll/Qt5Network.dll \
    DobotDll/Qt5SerialPort.dll

