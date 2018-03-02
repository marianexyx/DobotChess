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
    chess/chess_bot.cpp \
    chess/chess_conditions.cpp \
    chess/chess_movements.cpp \
    chess/chess_resets.cpp \
    chess/chess_status.cpp \
    chess/chess_timers.cpp \
    dobot/dobot_queue.cpp \
    dobot/dobot_servo.cpp \
    arduinousb.cpp \
    chess.cpp \
    chessboard.cpp \
    client.cpp \
    dobot.cpp \
    field.cpp \
    mainwindow.cpp \
    piece.cpp \
    sql.cpp \
    tcpmsgs.cpp \
    websockets.cpp \
    xml_reader.cpp \
    piece_controller.cpp

HEADERS  +=  \
    chess/chess_bot.h \
    chess/chess_conditions.h \
    chess/chess_movements.h \
    chess/chess_resets.h \
    chess/chess_status.h \
    chess/chess_timers.h \
    chess/game_status.h \
    chess/request_types.h \
    chess/sequence_types.h \
    chess/turn_types.h \
    dobot/dobot_queue.h \
    dobot/dobot_servo.h \
    DobotDll/DobotDll.h \
    DobotDll/DobotType.h \
    vars/basic_vars.h \
    vars/board_axis.h \
    vars/board_data_labels.h \
    vars/board_types.h \
    vars/log.h \
    vars/players_types.h \
    vars/position_on_board.h \
    arduinousb.h \
    chess.h \
    chessboard.h \
    client.h \
    dobot.h \
    field.h \
    mainwindow.h \
    piece.h \
    sql.h \
    tcpmsgs.h \
    ui_mainwindow.h \
    websockets.h \
    xml_reader.h \
    chess/vars/chenard_io_msgs.h \
    chess/vars/end_of_game_types.h \
    chess/vars/game_status.h \
    chess/vars/request_types.h \
    chess/vars/sequence_types.h \
    chess/vars/turn_types.h \
    dobot/vars/dobot_moves.h \
    piece_set.h \
    piece_controller.h

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

