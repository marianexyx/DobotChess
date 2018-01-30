#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T09:59:06
#
#-------------------------------------------------

QT       += core gui 
QT       += network 
QT       += xml 
QT       += sql
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
    sql.cpp \
    piece.cpp \
    field.cpp \
    chess/chess_timers.cpp \
    chess/chess_movements.cpp \
    chess/chess_bot.cpp \
    chess/chess_status.cpp \
    client.cpp \
    chess/chess_resets.cpp \
    xml_reader.cpp \
    dobot/dobot_queue.cpp \
    dobot/dobot_servo.cpp \
    chess/chess_conditions.cpp

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
    vars/log.h \
    vars/basic_vars.h \
    vars/board_axis.h \
    vars/dobot_moves.h \
    vars/board_data_labels.h \
    vars/players_types.h \
    vars/sequence_types.h \
    vars/end_of_game_types.h \
    vars/board_types.h \
    vars/turn_types.h \
    sql.h \
    vars/piece_type.h \
    piece.h \
    vars/posotion_on_board.h \
    field.h \
    chess/chess_timers.h \
    chess/chess_movements.h \
    chess/chess_bot.h \
    chess/chess_status.h \
    vars/game_status.h \
    client.h \
    chess/chess_resets.h \
    xml_reader.h \
    dobot/dobot_queue.h \
    dobot/dobot_servo.h \
    chess/chess_conditions.h \
    vars/request_types.h \
    vars/chenard_io_msgs.h

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

