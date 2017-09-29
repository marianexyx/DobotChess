/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[70];
    char stringdata0[1221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "checkMsgFromChenard"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 10), // "tcpMsgType"
QT_MOC_LITERAL(4, 43, 10), // "tcpRespond"
QT_MOC_LITERAL(5, 54, 13), // "onChangedMode"
QT_MOC_LITERAL(6, 68, 19), // "onJOGCtrlBtnPressed"
QT_MOC_LITERAL(7, 88, 5), // "index"
QT_MOC_LITERAL(8, 94, 20), // "onJOGCtrlBtnReleased"
QT_MOC_LITERAL(9, 115, 19), // "onPTPsendBtnClicked"
QT_MOC_LITERAL(10, 135, 14), // "writeInConsole"
QT_MOC_LITERAL(11, 150, 7), // "QStrMsg"
QT_MOC_LITERAL(12, 158, 3), // "LOG"
QT_MOC_LITERAL(13, 162, 7), // "msgType"
QT_MOC_LITERAL(14, 170, 17), // "changeWindowtitle"
QT_MOC_LITERAL(15, 188, 5), // "title"
QT_MOC_LITERAL(16, 194, 18), // "setBoardDataLabels"
QT_MOC_LITERAL(17, 213, 9), // "QStrLabel"
QT_MOC_LITERAL(18, 223, 17), // "BOARD_DATA_LABELS"
QT_MOC_LITERAL(19, 241, 9), // "labelType"
QT_MOC_LITERAL(20, 251, 9), // "showBoard"
QT_MOC_LITERAL(21, 261, 9), // "QStrBoard"
QT_MOC_LITERAL(22, 271, 14), // "showLegalMoves"
QT_MOC_LITERAL(23, 286, 10), // "legalMoves"
QT_MOC_LITERAL(24, 297, 16), // "showHistoryMoves"
QT_MOC_LITERAL(25, 314, 12), // "historyMoves"
QT_MOC_LITERAL(26, 327, 18), // "setDobotValidators"
QT_MOC_LITERAL(27, 346, 17), // "setJointLabelText"
QT_MOC_LITERAL(28, 364, 16), // "QSJointLabelText"
QT_MOC_LITERAL(29, 381, 6), // "sJoint"
QT_MOC_LITERAL(30, 388, 16), // "setAxisLabelText"
QT_MOC_LITERAL(31, 405, 15), // "QSAxisLabelText"
QT_MOC_LITERAL(32, 421, 6), // "chAxis"
QT_MOC_LITERAL(33, 428, 21), // "setDobotButtonsStates"
QT_MOC_LITERAL(34, 450, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(35, 470, 15), // "setDeviceLabels"
QT_MOC_LITERAL(36, 486, 10), // "QSdeviceSN"
QT_MOC_LITERAL(37, 497, 12), // "QSdeviceName"
QT_MOC_LITERAL(38, 510, 15), // "QSdeviceVersion"
QT_MOC_LITERAL(39, 526, 20), // "showDobotErrorMsgBox"
QT_MOC_LITERAL(40, 547, 39), // "on_emulatePlayerMsgLineEdit_t..."
QT_MOC_LITERAL(41, 587, 11), // "textChanged"
QT_MOC_LITERAL(42, 599, 30), // "on_sendSimulatedMsgBtn_clicked"
QT_MOC_LITERAL(43, 630, 14), // "setQueueLabels"
QT_MOC_LITERAL(44, 645, 6), // "nSpace"
QT_MOC_LITERAL(45, 652, 8), // "nDobotId"
QT_MOC_LITERAL(46, 661, 10), // "nCoreMaxId"
QT_MOC_LITERAL(47, 672, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(48, 684, 10), // "CoreNextId"
QT_MOC_LITERAL(49, 695, 23), // "on_calibrateBtn_clicked"
QT_MOC_LITERAL(50, 719, 16), // "on_upBtn_clicked"
QT_MOC_LITERAL(51, 736, 18), // "on_downBtn_clicked"
QT_MOC_LITERAL(52, 755, 29), // "on_resetDobotIndexBtn_clicked"
QT_MOC_LITERAL(53, 785, 33), // "on_executeDobotComandsBtn_cli..."
QT_MOC_LITERAL(54, 819, 16), // "on_AIBtn_clicked"
QT_MOC_LITERAL(55, 836, 25), // "on_AIEnemySendBtn_clicked"
QT_MOC_LITERAL(56, 862, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(57, 882, 9), // "nUsbPorst"
QT_MOC_LITERAL(58, 892, 36), // "on_portsComboBox_currentIndex..."
QT_MOC_LITERAL(59, 929, 25), // "on_reloadPortsBtn_clicked"
QT_MOC_LITERAL(60, 955, 21), // "on_sendUsbBtn_clicked"
QT_MOC_LITERAL(61, 977, 25), // "on_openGripperBtn_clicked"
QT_MOC_LITERAL(62, 1003, 26), // "on_closeGripperBtn_clicked"
QT_MOC_LITERAL(63, 1030, 24), // "on_startGmPosBtn_clicked"
QT_MOC_LITERAL(64, 1055, 24), // "on_startDtPosBtn_clicked"
QT_MOC_LITERAL(65, 1080, 29), // "on_SimulateFromUsbBtn_clicked"
QT_MOC_LITERAL(66, 1110, 21), // "on_sendTcpBtn_clicked"
QT_MOC_LITERAL(67, 1132, 30), // "on_sendTcpLineEdit_textChanged"
QT_MOC_LITERAL(68, 1163, 38), // "on_SimulateFromUsbLineEdit_te..."
QT_MOC_LITERAL(69, 1202, 18) // "on_homeBtn_clicked"

    },
    "MainWindow\0checkMsgFromChenard\0\0"
    "tcpMsgType\0tcpRespond\0onChangedMode\0"
    "onJOGCtrlBtnPressed\0index\0"
    "onJOGCtrlBtnReleased\0onPTPsendBtnClicked\0"
    "writeInConsole\0QStrMsg\0LOG\0msgType\0"
    "changeWindowtitle\0title\0setBoardDataLabels\0"
    "QStrLabel\0BOARD_DATA_LABELS\0labelType\0"
    "showBoard\0QStrBoard\0showLegalMoves\0"
    "legalMoves\0showHistoryMoves\0historyMoves\0"
    "setDobotValidators\0setJointLabelText\0"
    "QSJointLabelText\0sJoint\0setAxisLabelText\0"
    "QSAxisLabelText\0chAxis\0setDobotButtonsStates\0"
    "bDobotButtonsStates\0setDeviceLabels\0"
    "QSdeviceSN\0QSdeviceName\0QSdeviceVersion\0"
    "showDobotErrorMsgBox\0"
    "on_emulatePlayerMsgLineEdit_textChanged\0"
    "textChanged\0on_sendSimulatedMsgBtn_clicked\0"
    "setQueueLabels\0nSpace\0nDobotId\0"
    "nCoreMaxId\0nCoreIdLeft\0CoreNextId\0"
    "on_calibrateBtn_clicked\0on_upBtn_clicked\0"
    "on_downBtn_clicked\0on_resetDobotIndexBtn_clicked\0"
    "on_executeDobotComandsBtn_clicked\0"
    "on_AIBtn_clicked\0on_AIEnemySendBtn_clicked\0"
    "updatePortsComboBox\0nUsbPorst\0"
    "on_portsComboBox_currentIndexChanged\0"
    "on_reloadPortsBtn_clicked\0"
    "on_sendUsbBtn_clicked\0on_openGripperBtn_clicked\0"
    "on_closeGripperBtn_clicked\0"
    "on_startGmPosBtn_clicked\0"
    "on_startDtPosBtn_clicked\0"
    "on_SimulateFromUsbBtn_clicked\0"
    "on_sendTcpBtn_clicked\0"
    "on_sendTcpLineEdit_textChanged\0"
    "on_SimulateFromUsbLineEdit_textChanged\0"
    "on_homeBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  214,    2, 0x0a /* Public */,
       5,    0,  219,    2, 0x08 /* Private */,
       6,    1,  220,    2, 0x08 /* Private */,
       8,    0,  223,    2, 0x08 /* Private */,
       9,    0,  224,    2, 0x08 /* Private */,
      10,    2,  225,    2, 0x08 /* Private */,
      14,    1,  230,    2, 0x08 /* Private */,
      16,    2,  233,    2, 0x08 /* Private */,
      20,    1,  238,    2, 0x08 /* Private */,
      22,    1,  241,    2, 0x08 /* Private */,
      24,    1,  244,    2, 0x08 /* Private */,
      26,    0,  247,    2, 0x08 /* Private */,
      27,    2,  248,    2, 0x08 /* Private */,
      30,    2,  253,    2, 0x08 /* Private */,
      33,    1,  258,    2, 0x08 /* Private */,
      35,    3,  261,    2, 0x08 /* Private */,
      39,    0,  268,    2, 0x08 /* Private */,
      40,    1,  269,    2, 0x08 /* Private */,
      42,    0,  272,    2, 0x08 /* Private */,
      43,    5,  273,    2, 0x08 /* Private */,
      49,    0,  284,    2, 0x08 /* Private */,
      50,    0,  285,    2, 0x08 /* Private */,
      51,    0,  286,    2, 0x08 /* Private */,
      52,    0,  287,    2, 0x08 /* Private */,
      53,    0,  288,    2, 0x08 /* Private */,
      54,    0,  289,    2, 0x08 /* Private */,
      55,    0,  290,    2, 0x08 /* Private */,
      56,    1,  291,    2, 0x08 /* Private */,
      58,    1,  294,    2, 0x08 /* Private */,
      59,    0,  297,    2, 0x08 /* Private */,
      60,    0,  298,    2, 0x08 /* Private */,
      61,    0,  299,    2, 0x08 /* Private */,
      62,    0,  300,    2, 0x08 /* Private */,
      63,    0,  301,    2, 0x08 /* Private */,
      64,    0,  302,    2, 0x08 /* Private */,
      65,    0,  303,    2, 0x08 /* Private */,
      66,    0,  304,    2, 0x08 /* Private */,
      67,    1,  305,    2, 0x08 /* Private */,
      68,    1,  308,    2, 0x08 /* Private */,
      69,    0,  311,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 18,   17,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::Void, QMetaType::QStringList,   25,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,   28,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   31,   32,
    QMetaType::Void, QMetaType::Bool,   34,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   36,   37,   38,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   44,   45,   46,   47,   48,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   57,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->checkMsgFromChenard((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->onChangedMode(); break;
        case 2: _t->onJOGCtrlBtnPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onJOGCtrlBtnReleased(); break;
        case 4: _t->onPTPsendBtnClicked(); break;
        case 5: _t->writeInConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 6: _t->changeWindowtitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setBoardDataLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABELS(*)>(_a[2]))); break;
        case 8: _t->showBoard((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->showLegalMoves((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->showHistoryMoves((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->setDobotValidators(); break;
        case 12: _t->setJointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 13: _t->setAxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 14: _t->setDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setDeviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 16: _t->showDobotErrorMsgBox(); break;
        case 17: _t->on_emulatePlayerMsgLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->on_sendSimulatedMsgBtn_clicked(); break;
        case 19: _t->setQueueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 20: _t->on_calibrateBtn_clicked(); break;
        case 21: _t->on_upBtn_clicked(); break;
        case 22: _t->on_downBtn_clicked(); break;
        case 23: _t->on_resetDobotIndexBtn_clicked(); break;
        case 24: _t->on_executeDobotComandsBtn_clicked(); break;
        case 25: _t->on_AIBtn_clicked(); break;
        case 26: _t->on_AIEnemySendBtn_clicked(); break;
        case 27: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->on_portsComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->on_reloadPortsBtn_clicked(); break;
        case 30: _t->on_sendUsbBtn_clicked(); break;
        case 31: _t->on_openGripperBtn_clicked(); break;
        case 32: _t->on_closeGripperBtn_clicked(); break;
        case 33: _t->on_startGmPosBtn_clicked(); break;
        case 34: _t->on_startDtPosBtn_clicked(); break;
        case 35: _t->on_SimulateFromUsbBtn_clicked(); break;
        case 36: _t->on_sendTcpBtn_clicked(); break;
        case 37: _t->on_sendTcpLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 38: _t->on_SimulateFromUsbLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->on_homeBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
