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
    QByteArrayData data[71];
    char stringdata0[1232];
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
QT_MOC_LITERAL(22, 271, 10), // "clearBoard"
QT_MOC_LITERAL(23, 282, 14), // "showLegalMoves"
QT_MOC_LITERAL(24, 297, 10), // "legalMoves"
QT_MOC_LITERAL(25, 308, 16), // "showHistoryMoves"
QT_MOC_LITERAL(26, 325, 12), // "historyMoves"
QT_MOC_LITERAL(27, 338, 18), // "setDobotValidators"
QT_MOC_LITERAL(28, 357, 17), // "setJointLabelText"
QT_MOC_LITERAL(29, 375, 16), // "QSJointLabelText"
QT_MOC_LITERAL(30, 392, 6), // "sJoint"
QT_MOC_LITERAL(31, 399, 16), // "setAxisLabelText"
QT_MOC_LITERAL(32, 416, 15), // "QSAxisLabelText"
QT_MOC_LITERAL(33, 432, 6), // "chAxis"
QT_MOC_LITERAL(34, 439, 21), // "setDobotButtonsStates"
QT_MOC_LITERAL(35, 461, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(36, 481, 15), // "setDeviceLabels"
QT_MOC_LITERAL(37, 497, 10), // "QSdeviceSN"
QT_MOC_LITERAL(38, 508, 12), // "QSdeviceName"
QT_MOC_LITERAL(39, 521, 15), // "QSdeviceVersion"
QT_MOC_LITERAL(40, 537, 20), // "showDobotErrorMsgBox"
QT_MOC_LITERAL(41, 558, 39), // "on_emulatePlayerMsgLineEdit_t..."
QT_MOC_LITERAL(42, 598, 11), // "textChanged"
QT_MOC_LITERAL(43, 610, 30), // "on_sendSimulatedMsgBtn_clicked"
QT_MOC_LITERAL(44, 641, 14), // "setQueueLabels"
QT_MOC_LITERAL(45, 656, 6), // "nSpace"
QT_MOC_LITERAL(46, 663, 8), // "nDobotId"
QT_MOC_LITERAL(47, 672, 10), // "nCoreMaxId"
QT_MOC_LITERAL(48, 683, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(49, 695, 10), // "CoreNextId"
QT_MOC_LITERAL(50, 706, 23), // "on_calibrateBtn_clicked"
QT_MOC_LITERAL(51, 730, 16), // "on_upBtn_clicked"
QT_MOC_LITERAL(52, 747, 18), // "on_downBtn_clicked"
QT_MOC_LITERAL(53, 766, 29), // "on_resetDobotIndexBtn_clicked"
QT_MOC_LITERAL(54, 796, 33), // "on_executeDobotComandsBtn_cli..."
QT_MOC_LITERAL(55, 830, 16), // "on_AIBtn_clicked"
QT_MOC_LITERAL(56, 847, 25), // "on_AIEnemySendBtn_clicked"
QT_MOC_LITERAL(57, 873, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(58, 893, 9), // "nUsbPorst"
QT_MOC_LITERAL(59, 903, 36), // "on_portsComboBox_currentIndex..."
QT_MOC_LITERAL(60, 940, 25), // "on_reloadPortsBtn_clicked"
QT_MOC_LITERAL(61, 966, 21), // "on_sendUsbBtn_clicked"
QT_MOC_LITERAL(62, 988, 25), // "on_openGripperBtn_clicked"
QT_MOC_LITERAL(63, 1014, 26), // "on_closeGripperBtn_clicked"
QT_MOC_LITERAL(64, 1041, 24), // "on_startGmPosBtn_clicked"
QT_MOC_LITERAL(65, 1066, 24), // "on_startDtPosBtn_clicked"
QT_MOC_LITERAL(66, 1091, 29), // "on_SimulateFromUsbBtn_clicked"
QT_MOC_LITERAL(67, 1121, 21), // "on_sendTcpBtn_clicked"
QT_MOC_LITERAL(68, 1143, 30), // "on_sendTcpLineEdit_textChanged"
QT_MOC_LITERAL(69, 1174, 38), // "on_SimulateFromUsbLineEdit_te..."
QT_MOC_LITERAL(70, 1213, 18) // "on_homeBtn_clicked"

    },
    "MainWindow\0checkMsgFromChenard\0\0"
    "tcpMsgType\0tcpRespond\0onChangedMode\0"
    "onJOGCtrlBtnPressed\0index\0"
    "onJOGCtrlBtnReleased\0onPTPsendBtnClicked\0"
    "writeInConsole\0QStrMsg\0LOG\0msgType\0"
    "changeWindowtitle\0title\0setBoardDataLabels\0"
    "QStrLabel\0BOARD_DATA_LABELS\0labelType\0"
    "showBoard\0QStrBoard\0clearBoard\0"
    "showLegalMoves\0legalMoves\0showHistoryMoves\0"
    "historyMoves\0setDobotValidators\0"
    "setJointLabelText\0QSJointLabelText\0"
    "sJoint\0setAxisLabelText\0QSAxisLabelText\0"
    "chAxis\0setDobotButtonsStates\0"
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
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  219,    2, 0x0a /* Public */,
       5,    0,  224,    2, 0x08 /* Private */,
       6,    1,  225,    2, 0x08 /* Private */,
       8,    0,  228,    2, 0x08 /* Private */,
       9,    0,  229,    2, 0x08 /* Private */,
      10,    2,  230,    2, 0x08 /* Private */,
      14,    1,  235,    2, 0x08 /* Private */,
      16,    2,  238,    2, 0x08 /* Private */,
      20,    1,  243,    2, 0x08 /* Private */,
      22,    0,  246,    2, 0x08 /* Private */,
      23,    1,  247,    2, 0x08 /* Private */,
      25,    1,  250,    2, 0x08 /* Private */,
      27,    0,  253,    2, 0x08 /* Private */,
      28,    2,  254,    2, 0x08 /* Private */,
      31,    2,  259,    2, 0x08 /* Private */,
      34,    1,  264,    2, 0x08 /* Private */,
      36,    3,  267,    2, 0x08 /* Private */,
      40,    0,  274,    2, 0x08 /* Private */,
      41,    1,  275,    2, 0x08 /* Private */,
      43,    0,  278,    2, 0x08 /* Private */,
      44,    5,  279,    2, 0x08 /* Private */,
      50,    0,  290,    2, 0x08 /* Private */,
      51,    0,  291,    2, 0x08 /* Private */,
      52,    0,  292,    2, 0x08 /* Private */,
      53,    0,  293,    2, 0x08 /* Private */,
      54,    0,  294,    2, 0x08 /* Private */,
      55,    0,  295,    2, 0x08 /* Private */,
      56,    0,  296,    2, 0x08 /* Private */,
      57,    1,  297,    2, 0x08 /* Private */,
      59,    1,  300,    2, 0x08 /* Private */,
      60,    0,  303,    2, 0x08 /* Private */,
      61,    0,  304,    2, 0x08 /* Private */,
      62,    0,  305,    2, 0x08 /* Private */,
      63,    0,  306,    2, 0x08 /* Private */,
      64,    0,  307,    2, 0x08 /* Private */,
      65,    0,  308,    2, 0x08 /* Private */,
      66,    0,  309,    2, 0x08 /* Private */,
      67,    0,  310,    2, 0x08 /* Private */,
      68,    1,  311,    2, 0x08 /* Private */,
      69,    1,  314,    2, 0x08 /* Private */,
      70,    0,  317,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   24,
    QMetaType::Void, QMetaType::QStringList,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,   29,   30,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   32,   33,
    QMetaType::Void, QMetaType::Bool,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   37,   38,   39,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   45,   46,   47,   48,   49,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   58,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void, QMetaType::QString,   42,
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
        case 9: _t->clearBoard(); break;
        case 10: _t->showLegalMoves((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->showHistoryMoves((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 12: _t->setDobotValidators(); break;
        case 13: _t->setJointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 14: _t->setAxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 15: _t->setDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->setDeviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 17: _t->showDobotErrorMsgBox(); break;
        case 18: _t->on_emulatePlayerMsgLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->on_sendSimulatedMsgBtn_clicked(); break;
        case 20: _t->setQueueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 21: _t->on_calibrateBtn_clicked(); break;
        case 22: _t->on_upBtn_clicked(); break;
        case 23: _t->on_downBtn_clicked(); break;
        case 24: _t->on_resetDobotIndexBtn_clicked(); break;
        case 25: _t->on_executeDobotComandsBtn_clicked(); break;
        case 26: _t->on_AIBtn_clicked(); break;
        case 27: _t->on_AIEnemySendBtn_clicked(); break;
        case 28: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->on_portsComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->on_reloadPortsBtn_clicked(); break;
        case 31: _t->on_sendUsbBtn_clicked(); break;
        case 32: _t->on_openGripperBtn_clicked(); break;
        case 33: _t->on_closeGripperBtn_clicked(); break;
        case 34: _t->on_startGmPosBtn_clicked(); break;
        case 35: _t->on_startDtPosBtn_clicked(); break;
        case 36: _t->on_SimulateFromUsbBtn_clicked(); break;
        case 37: _t->on_sendTcpBtn_clicked(); break;
        case 38: _t->on_sendTcpLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->on_SimulateFromUsbLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 40: _t->on_homeBtn_clicked(); break;
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
QT_END_MOC_NAMESPACE
