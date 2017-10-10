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
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[78];
    char stringdata0[1388];
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
QT_MOC_LITERAL(50, 706, 33), // "showActualDobotQueuedCmdIndex..."
QT_MOC_LITERAL(51, 740, 37), // "QList<ArmPosForCurrentCmdQueu..."
QT_MOC_LITERAL(52, 778, 4), // "list"
QT_MOC_LITERAL(53, 783, 27), // "showArduinoGripperStateList"
QT_MOC_LITERAL(54, 811, 19), // "QList<ServoArduino>"
QT_MOC_LITERAL(55, 831, 23), // "on_calibrateBtn_clicked"
QT_MOC_LITERAL(56, 855, 16), // "on_upBtn_clicked"
QT_MOC_LITERAL(57, 872, 18), // "on_downBtn_clicked"
QT_MOC_LITERAL(58, 891, 29), // "on_resetDobotIndexBtn_clicked"
QT_MOC_LITERAL(59, 921, 33), // "on_executeDobotComandsBtn_cli..."
QT_MOC_LITERAL(60, 955, 16), // "on_AIBtn_clicked"
QT_MOC_LITERAL(61, 972, 25), // "on_AIEnemySendBtn_clicked"
QT_MOC_LITERAL(62, 998, 15), // "showClientsList"
QT_MOC_LITERAL(63, 1014, 14), // "QList<Clients>"
QT_MOC_LITERAL(64, 1029, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(65, 1049, 9), // "nUsbPorst"
QT_MOC_LITERAL(66, 1059, 36), // "on_portsComboBox_currentIndex..."
QT_MOC_LITERAL(67, 1096, 25), // "on_reloadPortsBtn_clicked"
QT_MOC_LITERAL(68, 1122, 21), // "on_sendUsbBtn_clicked"
QT_MOC_LITERAL(69, 1144, 25), // "on_openGripperBtn_clicked"
QT_MOC_LITERAL(70, 1170, 26), // "on_closeGripperBtn_clicked"
QT_MOC_LITERAL(71, 1197, 24), // "on_startGmPosBtn_clicked"
QT_MOC_LITERAL(72, 1222, 24), // "on_startDtPosBtn_clicked"
QT_MOC_LITERAL(73, 1247, 29), // "on_SimulateFromUsbBtn_clicked"
QT_MOC_LITERAL(74, 1277, 21), // "on_sendTcpBtn_clicked"
QT_MOC_LITERAL(75, 1299, 30), // "on_sendTcpLineEdit_textChanged"
QT_MOC_LITERAL(76, 1330, 38), // "on_SimulateFromUsbLineEdit_te..."
QT_MOC_LITERAL(77, 1369, 18) // "on_homeBtn_clicked"

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
    "showActualDobotQueuedCmdIndexList\0"
    "QList<ArmPosForCurrentCmdQueuedIndex>\0"
    "list\0showArduinoGripperStateList\0"
    "QList<ServoArduino>\0on_calibrateBtn_clicked\0"
    "on_upBtn_clicked\0on_downBtn_clicked\0"
    "on_resetDobotIndexBtn_clicked\0"
    "on_executeDobotComandsBtn_clicked\0"
    "on_AIBtn_clicked\0on_AIEnemySendBtn_clicked\0"
    "showClientsList\0QList<Clients>\0"
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
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  234,    2, 0x0a /* Public */,
       5,    0,  239,    2, 0x08 /* Private */,
       6,    1,  240,    2, 0x08 /* Private */,
       8,    0,  243,    2, 0x08 /* Private */,
       9,    0,  244,    2, 0x08 /* Private */,
      10,    2,  245,    2, 0x08 /* Private */,
      14,    1,  250,    2, 0x08 /* Private */,
      16,    2,  253,    2, 0x08 /* Private */,
      20,    1,  258,    2, 0x08 /* Private */,
      22,    0,  261,    2, 0x08 /* Private */,
      23,    1,  262,    2, 0x08 /* Private */,
      25,    1,  265,    2, 0x08 /* Private */,
      27,    0,  268,    2, 0x08 /* Private */,
      28,    2,  269,    2, 0x08 /* Private */,
      31,    2,  274,    2, 0x08 /* Private */,
      34,    1,  279,    2, 0x08 /* Private */,
      36,    3,  282,    2, 0x08 /* Private */,
      40,    0,  289,    2, 0x08 /* Private */,
      41,    1,  290,    2, 0x08 /* Private */,
      43,    0,  293,    2, 0x08 /* Private */,
      44,    5,  294,    2, 0x08 /* Private */,
      50,    1,  305,    2, 0x08 /* Private */,
      53,    1,  308,    2, 0x08 /* Private */,
      55,    0,  311,    2, 0x08 /* Private */,
      56,    0,  312,    2, 0x08 /* Private */,
      57,    0,  313,    2, 0x08 /* Private */,
      58,    0,  314,    2, 0x08 /* Private */,
      59,    0,  315,    2, 0x08 /* Private */,
      60,    0,  316,    2, 0x08 /* Private */,
      61,    0,  317,    2, 0x08 /* Private */,
      62,    1,  318,    2, 0x08 /* Private */,
      64,    1,  321,    2, 0x08 /* Private */,
      66,    1,  324,    2, 0x08 /* Private */,
      67,    0,  327,    2, 0x08 /* Private */,
      68,    0,  328,    2, 0x08 /* Private */,
      69,    0,  329,    2, 0x08 /* Private */,
      70,    0,  330,    2, 0x08 /* Private */,
      71,    0,  331,    2, 0x08 /* Private */,
      72,    0,  332,    2, 0x08 /* Private */,
      73,    0,  333,    2, 0x08 /* Private */,
      74,    0,  334,    2, 0x08 /* Private */,
      75,    1,  335,    2, 0x08 /* Private */,
      76,    1,  338,    2, 0x08 /* Private */,
      77,    0,  341,    2, 0x08 /* Private */,

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
    QMetaType::Void, 0x80000000 | 51,   52,
    QMetaType::Void, 0x80000000 | 54,   52,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 63,   52,
    QMetaType::Void, QMetaType::Int,   65,
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
        case 21: _t->showActualDobotQueuedCmdIndexList((*reinterpret_cast< QList<ArmPosForCurrentCmdQueuedIndex>(*)>(_a[1]))); break;
        case 22: _t->showArduinoGripperStateList((*reinterpret_cast< QList<ServoArduino>(*)>(_a[1]))); break;
        case 23: _t->on_calibrateBtn_clicked(); break;
        case 24: _t->on_upBtn_clicked(); break;
        case 25: _t->on_downBtn_clicked(); break;
        case 26: _t->on_resetDobotIndexBtn_clicked(); break;
        case 27: _t->on_executeDobotComandsBtn_clicked(); break;
        case 28: _t->on_AIBtn_clicked(); break;
        case 29: _t->on_AIEnemySendBtn_clicked(); break;
        case 30: _t->showClientsList((*reinterpret_cast< QList<Clients>(*)>(_a[1]))); break;
        case 31: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->on_portsComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->on_reloadPortsBtn_clicked(); break;
        case 34: _t->on_sendUsbBtn_clicked(); break;
        case 35: _t->on_openGripperBtn_clicked(); break;
        case 36: _t->on_closeGripperBtn_clicked(); break;
        case 37: _t->on_startGmPosBtn_clicked(); break;
        case 38: _t->on_startDtPosBtn_clicked(); break;
        case 39: _t->on_SimulateFromUsbBtn_clicked(); break;
        case 40: _t->on_sendTcpBtn_clicked(); break;
        case 41: _t->on_sendTcpLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 42: _t->on_SimulateFromUsbLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 43: _t->on_homeBtn_clicked(); break;
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
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 44)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 44;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
