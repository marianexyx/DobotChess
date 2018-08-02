/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[70];
    char stringdata0[1246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 14), // "writeInConsole"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "QStrMsg"
QT_MOC_LITERAL(4, 35, 3), // "LOG"
QT_MOC_LITERAL(5, 39, 9), // "TypeOfMsg"
QT_MOC_LITERAL(6, 49, 17), // "setBoardDataLabel"
QT_MOC_LITERAL(7, 67, 9), // "QStrLabel"
QT_MOC_LITERAL(8, 77, 16), // "BOARD_DATA_LABEL"
QT_MOC_LITERAL(9, 94, 9), // "LabelType"
QT_MOC_LITERAL(10, 104, 22), // "showImaginaryBoardInUI"
QT_MOC_LITERAL(11, 127, 9), // "QStrBoard"
QT_MOC_LITERAL(12, 137, 17), // "showRealBoardInUI"
QT_MOC_LITERAL(13, 155, 14), // "clearBoardInUI"
QT_MOC_LITERAL(14, 170, 18), // "showLegalMovesInUI"
QT_MOC_LITERAL(15, 189, 10), // "legalMoves"
QT_MOC_LITERAL(16, 200, 20), // "showHistoryMovesInUI"
QT_MOC_LITERAL(17, 221, 12), // "historyMoves"
QT_MOC_LITERAL(18, 234, 17), // "changeWindowTitle"
QT_MOC_LITERAL(19, 252, 13), // "onChangedMode"
QT_MOC_LITERAL(20, 266, 19), // "onJOGCtrlBtnPressed"
QT_MOC_LITERAL(21, 286, 3), // "nID"
QT_MOC_LITERAL(22, 290, 20), // "onJOGCtrlBtnReleased"
QT_MOC_LITERAL(23, 311, 25), // "setDobotPTEValidatorsInUI"
QT_MOC_LITERAL(24, 337, 17), // "setJointLabelText"
QT_MOC_LITERAL(25, 355, 18), // "QStrJointLabelText"
QT_MOC_LITERAL(26, 374, 6), // "sJoint"
QT_MOC_LITERAL(27, 381, 16), // "setAxisLabelText"
QT_MOC_LITERAL(28, 398, 17), // "QStrAxisLabelText"
QT_MOC_LITERAL(29, 416, 6), // "chAxis"
QT_MOC_LITERAL(30, 423, 21), // "setDobotButtonsStates"
QT_MOC_LITERAL(31, 445, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(32, 465, 15), // "setDeviceLabels"
QT_MOC_LITERAL(33, 481, 12), // "QStrDeviceSN"
QT_MOC_LITERAL(34, 494, 14), // "QStrDeviceName"
QT_MOC_LITERAL(35, 509, 17), // "QStrDeviceVersion"
QT_MOC_LITERAL(36, 527, 20), // "showDobotErrorMsgBox"
QT_MOC_LITERAL(37, 548, 39), // "on_emulatePlayerMsgLineEdit_t..."
QT_MOC_LITERAL(38, 588, 15), // "QStrTextChanged"
QT_MOC_LITERAL(39, 604, 30), // "on_sendSimulatedMsgBtn_clicked"
QT_MOC_LITERAL(40, 635, 14), // "setQueueLabels"
QT_MOC_LITERAL(41, 650, 7), // "unSpace"
QT_MOC_LITERAL(42, 658, 8), // "uint64_t"
QT_MOC_LITERAL(43, 667, 11), // "un64DobotId"
QT_MOC_LITERAL(44, 679, 13), // "un64CoreMaxId"
QT_MOC_LITERAL(45, 693, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(46, 705, 14), // "un64CoreNextId"
QT_MOC_LITERAL(47, 720, 23), // "showQueuedArmCmdsOnCore"
QT_MOC_LITERAL(48, 744, 22), // "showSentArmCmdsToDobot"
QT_MOC_LITERAL(49, 767, 23), // "on_calibrateBtn_clicked"
QT_MOC_LITERAL(50, 791, 16), // "on_upBtn_clicked"
QT_MOC_LITERAL(51, 808, 18), // "on_downBtn_clicked"
QT_MOC_LITERAL(52, 827, 29), // "on_resetDobotIndexBtn_clicked"
QT_MOC_LITERAL(53, 857, 19), // "showClientsListInUI"
QT_MOC_LITERAL(54, 877, 13), // "QList<Client>"
QT_MOC_LITERAL(55, 891, 4), // "list"
QT_MOC_LITERAL(56, 896, 25), // "on_openGripperBtn_clicked"
QT_MOC_LITERAL(57, 922, 26), // "on_closeGripperBtn_clicked"
QT_MOC_LITERAL(58, 949, 24), // "on_startGmPosBtn_clicked"
QT_MOC_LITERAL(59, 974, 24), // "on_startDtPosBtn_clicked"
QT_MOC_LITERAL(60, 999, 21), // "on_sendTcpBtn_clicked"
QT_MOC_LITERAL(61, 1021, 30), // "on_sendTcpLineEdit_textChanged"
QT_MOC_LITERAL(62, 1052, 18), // "on_homeBtn_clicked"
QT_MOC_LITERAL(63, 1071, 25), // "on_middleAboveBtn_clicked"
QT_MOC_LITERAL(64, 1097, 23), // "on_sendPointBtn_clicked"
QT_MOC_LITERAL(65, 1121, 23), // "on_xPTPEdit_textChanged"
QT_MOC_LITERAL(66, 1145, 23), // "on_yPTPEdit_textChanged"
QT_MOC_LITERAL(67, 1169, 23), // "on_zPTPEdit_textChanged"
QT_MOC_LITERAL(68, 1193, 25), // "on_retreatLeftBtn_clicked"
QT_MOC_LITERAL(69, 1219, 26) // "on_retreatRightBtn_clicked"

    },
    "MainWindow\0writeInConsole\0\0QStrMsg\0"
    "LOG\0TypeOfMsg\0setBoardDataLabel\0"
    "QStrLabel\0BOARD_DATA_LABEL\0LabelType\0"
    "showImaginaryBoardInUI\0QStrBoard\0"
    "showRealBoardInUI\0clearBoardInUI\0"
    "showLegalMovesInUI\0legalMoves\0"
    "showHistoryMovesInUI\0historyMoves\0"
    "changeWindowTitle\0onChangedMode\0"
    "onJOGCtrlBtnPressed\0nID\0onJOGCtrlBtnReleased\0"
    "setDobotPTEValidatorsInUI\0setJointLabelText\0"
    "QStrJointLabelText\0sJoint\0setAxisLabelText\0"
    "QStrAxisLabelText\0chAxis\0setDobotButtonsStates\0"
    "bDobotButtonsStates\0setDeviceLabels\0"
    "QStrDeviceSN\0QStrDeviceName\0"
    "QStrDeviceVersion\0showDobotErrorMsgBox\0"
    "on_emulatePlayerMsgLineEdit_textChanged\0"
    "QStrTextChanged\0on_sendSimulatedMsgBtn_clicked\0"
    "setQueueLabels\0unSpace\0uint64_t\0"
    "un64DobotId\0un64CoreMaxId\0nCoreIdLeft\0"
    "un64CoreNextId\0showQueuedArmCmdsOnCore\0"
    "showSentArmCmdsToDobot\0on_calibrateBtn_clicked\0"
    "on_upBtn_clicked\0on_downBtn_clicked\0"
    "on_resetDobotIndexBtn_clicked\0"
    "showClientsListInUI\0QList<Client>\0"
    "list\0on_openGripperBtn_clicked\0"
    "on_closeGripperBtn_clicked\0"
    "on_startGmPosBtn_clicked\0"
    "on_startDtPosBtn_clicked\0on_sendTcpBtn_clicked\0"
    "on_sendTcpLineEdit_textChanged\0"
    "on_homeBtn_clicked\0on_middleAboveBtn_clicked\0"
    "on_sendPointBtn_clicked\0on_xPTPEdit_textChanged\0"
    "on_yPTPEdit_textChanged\0on_zPTPEdit_textChanged\0"
    "on_retreatLeftBtn_clicked\0"
    "on_retreatRightBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  219,    2, 0x08 /* Private */,
       6,    2,  224,    2, 0x08 /* Private */,
      10,    1,  229,    2, 0x08 /* Private */,
      12,    0,  232,    2, 0x08 /* Private */,
      13,    0,  233,    2, 0x08 /* Private */,
      14,    1,  234,    2, 0x08 /* Private */,
      16,    1,  237,    2, 0x08 /* Private */,
      18,    0,  240,    2, 0x08 /* Private */,
      19,    0,  241,    2, 0x08 /* Private */,
      20,    1,  242,    2, 0x08 /* Private */,
      22,    0,  245,    2, 0x08 /* Private */,
      23,    0,  246,    2, 0x08 /* Private */,
      24,    2,  247,    2, 0x08 /* Private */,
      27,    2,  252,    2, 0x08 /* Private */,
      30,    1,  257,    2, 0x08 /* Private */,
      32,    3,  260,    2, 0x08 /* Private */,
      36,    0,  267,    2, 0x08 /* Private */,
      37,    1,  268,    2, 0x08 /* Private */,
      39,    0,  271,    2, 0x08 /* Private */,
      40,    5,  272,    2, 0x08 /* Private */,
      47,    0,  283,    2, 0x08 /* Private */,
      48,    0,  284,    2, 0x08 /* Private */,
      49,    0,  285,    2, 0x08 /* Private */,
      50,    0,  286,    2, 0x08 /* Private */,
      51,    0,  287,    2, 0x08 /* Private */,
      52,    0,  288,    2, 0x08 /* Private */,
      53,    1,  289,    2, 0x08 /* Private */,
      56,    0,  292,    2, 0x08 /* Private */,
      57,    0,  293,    2, 0x08 /* Private */,
      58,    0,  294,    2, 0x08 /* Private */,
      59,    0,  295,    2, 0x08 /* Private */,
      60,    0,  296,    2, 0x08 /* Private */,
      61,    1,  297,    2, 0x08 /* Private */,
      62,    0,  300,    2, 0x08 /* Private */,
      63,    0,  301,    2, 0x08 /* Private */,
      64,    0,  302,    2, 0x08 /* Private */,
      65,    1,  303,    2, 0x08 /* Private */,
      66,    1,  306,    2, 0x08 /* Private */,
      67,    1,  309,    2, 0x08 /* Private */,
      68,    0,  312,    2, 0x08 /* Private */,
      69,    0,  313,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   15,
    QMetaType::Void, QMetaType::QStringList,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,   25,   26,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   28,   29,
    QMetaType::Void, QMetaType::Bool,   31,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   33,   34,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 42, 0x80000000 | 42, QMetaType::Int, 0x80000000 | 42,   41,   43,   44,   45,   46,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 54,   55,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->writeInConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->setBoardDataLabel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 2: _t->showImaginaryBoardInUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->showRealBoardInUI(); break;
        case 4: _t->clearBoardInUI(); break;
        case 5: _t->showLegalMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 6: _t->showHistoryMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 7: _t->changeWindowTitle(); break;
        case 8: _t->onChangedMode(); break;
        case 9: _t->onJOGCtrlBtnPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onJOGCtrlBtnReleased(); break;
        case 11: _t->setDobotPTEValidatorsInUI(); break;
        case 12: _t->setJointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 13: _t->setAxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 14: _t->setDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setDeviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 16: _t->showDobotErrorMsgBox(); break;
        case 17: _t->on_emulatePlayerMsgLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->on_sendSimulatedMsgBtn_clicked(); break;
        case 19: _t->setQueueLabels((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint64_t(*)>(_a[2])),(*reinterpret_cast< uint64_t(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< uint64_t(*)>(_a[5]))); break;
        case 20: _t->showQueuedArmCmdsOnCore(); break;
        case 21: _t->showSentArmCmdsToDobot(); break;
        case 22: _t->on_calibrateBtn_clicked(); break;
        case 23: _t->on_upBtn_clicked(); break;
        case 24: _t->on_downBtn_clicked(); break;
        case 25: _t->on_resetDobotIndexBtn_clicked(); break;
        case 26: _t->showClientsListInUI((*reinterpret_cast< QList<Client>(*)>(_a[1]))); break;
        case 27: _t->on_openGripperBtn_clicked(); break;
        case 28: _t->on_closeGripperBtn_clicked(); break;
        case 29: _t->on_startGmPosBtn_clicked(); break;
        case 30: _t->on_startDtPosBtn_clicked(); break;
        case 31: _t->on_sendTcpBtn_clicked(); break;
        case 32: _t->on_sendTcpLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->on_homeBtn_clicked(); break;
        case 34: _t->on_middleAboveBtn_clicked(); break;
        case 35: _t->on_sendPointBtn_clicked(); break;
        case 36: _t->on_xPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 37: _t->on_yPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 38: _t->on_zPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->on_retreatLeftBtn_clicked(); break;
        case 40: _t->on_retreatRightBtn_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 41)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 41;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
