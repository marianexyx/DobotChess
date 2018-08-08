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
    chess/chess_conditions.cpp \
    chess/chess_movements.cpp \
    chess/chess_status.cpp \
    chess/chess_timers.cpp \
    chess.cpp \
    client.cpp \
    dobot.cpp \
    errors.cpp \
    mainwindow.cpp \
    piece_controller.cpp \
    sql.cpp \
    tcpmsgs.cpp \
    websockets.cpp \
    xml_reader.cpp \
    chessboard/vars/position_on_board.cpp \
    chessboard/chessboard.cpp \
    chessboard/field.cpp \
    chessboard/piece.cpp \
    dobot/dobot_gripper.cpp \
    dobot/dobot_queue.cpp

HEADERS  += \
    chess/vars/action_types.h \
    chess/vars/chenard_io_msgs.h \
    chess/vars/end_of_game_types.h \
    chess/vars/game_status.h \
    chess/vars/request_types.h \
    chess/vars/sequence_types.h \
    chess/vars/turn_types.h \
    chess/chess_conditions.h \
    chess/chess_movements.h \
    chess/chess_status.h \
    chess/chess_timers.h \
    vars/basic_vars.h \
    vars/board_data_labels.h \
    vars/log.h \
    vars/players_types.h \
    vars/table_data_types.h \
    chess.h \
    client.h \
    dobot.h \
    errors.h \
    mainwindow.h \
    piece_set.h \
    piece_controller.h \
    sql.h \
    tcpmsgs.h \
    ui_mainwindow.h \
    websockets.h \
    xml_reader.h \
    chessboard/vars/board_axis.h \
    chessboard/vars/board_types.h \
    chessboard/vars/position_on_board.h \
    chessboard/chessboard.h \
    chessboard/field.h \
    chessboard/piece.h \
    dobot/vars/dobot_move_types.h \
    dobot/dobot_gripper.h \
    dobot/dobot_queue.h \
    DobotDll/DobotDll.h \
    DobotDll/DobotType.h

FORMS    += mainwindow.ui

DESTDIR = ./

INCLUDEPATH += ./DobotDll

CONFIG(debug, debug | release) {
    LIBS += -L$$INCLUDEPATH/ -lDobotDll
} else {
    LIBS += -L$$INCLUDEPATH/ -lDobotDll
}

DISTFILES += \
    DobotDll/DobotDll.lib \
    DobotDll/DobotDemoForQt.exe \
    DobotDll/DobotDll.dll
