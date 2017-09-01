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
    chessboard.cpp \
    websockets.cpp \
    arduinousb.cpp \
    igorbot.cpp \
    webchess.cpp

HEADERS  +=  \
    websockets.h \
    tcpmsgs.h \
    chessboard.h \
    dobot.h \
    arduinousb.h \
    chess.h \
    ui_mainwindow.h \
    DobotDll/DobotDll.h \
    DobotDll/DobotType.h \
    mainwindow.h \
    igorbot.h \
    webchess.h \
    vars/log.h \
    vars/basic_vars.h \
    vars/board_axis.h \
    vars/dobot_moves.h \
    vars/board_data_labels.h \
    vars/players_types.h \
    vars/sequence_types.h \
    vars/end_of_game_types.h

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

