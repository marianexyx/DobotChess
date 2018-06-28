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
    QByteArrayData data[82];
    char stringdata0[1541];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "onChangedMode"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 19), // "onJOGCtrlBtnPressed"
QT_MOC_LITERAL(4, 46, 3), // "nID"
QT_MOC_LITERAL(5, 50, 20), // "onJOGCtrlBtnReleased"
QT_MOC_LITERAL(6, 71, 14), // "writeInConsole"
QT_MOC_LITERAL(7, 86, 7), // "QStrMsg"
QT_MOC_LITERAL(8, 94, 3), // "LOG"
QT_MOC_LITERAL(9, 98, 9), // "TypeOfMsg"
QT_MOC_LITERAL(10, 108, 17), // "setBoardDataLabel"
QT_MOC_LITERAL(11, 126, 9), // "QStrLabel"
QT_MOC_LITERAL(12, 136, 16), // "BOARD_DATA_LABEL"
QT_MOC_LITERAL(13, 153, 9), // "LabelType"
QT_MOC_LITERAL(14, 163, 13), // "showBoardInUI"
QT_MOC_LITERAL(15, 177, 9), // "QStrBoard"
QT_MOC_LITERAL(16, 187, 17), // "showRealBoardInUI"
QT_MOC_LITERAL(17, 205, 14), // "clearBoardInUI"
QT_MOC_LITERAL(18, 220, 18), // "showLegalMovesInUI"
QT_MOC_LITERAL(19, 239, 10), // "legalMoves"
QT_MOC_LITERAL(20, 250, 20), // "showHistoryMovesInUI"
QT_MOC_LITERAL(21, 271, 12), // "historyMoves"
QT_MOC_LITERAL(22, 284, 17), // "changeWindowTitle"
QT_MOC_LITERAL(23, 302, 25), // "setDobotPTEValidatorsInUI"
QT_MOC_LITERAL(24, 328, 17), // "setJointLabelText"
QT_MOC_LITERAL(25, 346, 18), // "QStrJointLabelText"
QT_MOC_LITERAL(26, 365, 6), // "sJoint"
QT_MOC_LITERAL(27, 372, 16), // "setAxisLabelText"
QT_MOC_LITERAL(28, 389, 17), // "QStrAxisLabelText"
QT_MOC_LITERAL(29, 407, 6), // "chAxis"
QT_MOC_LITERAL(30, 414, 21), // "setDobotButtonsStates"
QT_MOC_LITERAL(31, 436, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(32, 456, 15), // "setDeviceLabels"
QT_MOC_LITERAL(33, 472, 12), // "QStrDeviceSN"
QT_MOC_LITERAL(34, 485, 14), // "QStrDeviceName"
QT_MOC_LITERAL(35, 500, 17), // "QStrDeviceVersion"
QT_MOC_LITERAL(36, 518, 20), // "showDobotErrorMsgBox"
QT_MOC_LITERAL(37, 539, 39), // "on_emulatePlayerMsgLineEdit_t..."
QT_MOC_LITERAL(38, 579, 15), // "QStrTextChanged"
QT_MOC_LITERAL(39, 595, 30), // "on_sendSimulatedMsgBtn_clicked"
QT_MOC_LITERAL(40, 626, 14), // "setQueueLabels"
QT_MOC_LITERAL(41, 641, 10), // "nSpaceLeft"
QT_MOC_LITERAL(42, 652, 8), // "nDobotId"
QT_MOC_LITERAL(43, 661, 10), // "nCoreMaxId"
QT_MOC_LITERAL(44, 672, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(45, 684, 11), // "nCoreNextId"
QT_MOC_LITERAL(46, 696, 30), // "showActualDobotQueuedCmdIDList"
QT_MOC_LITERAL(47, 727, 16), // "QList<DobotMove>"
QT_MOC_LITERAL(48, 744, 4), // "list"
QT_MOC_LITERAL(49, 749, 25), // "showOnDobotQueuedCmdsList"
QT_MOC_LITERAL(50, 775, 27), // "showArduinoGripperStateList"
QT_MOC_LITERAL(51, 803, 19), // "QList<ServoArduino>"
QT_MOC_LITERAL(52, 823, 23), // "on_calibrateBtn_clicked"
QT_MOC_LITERAL(53, 847, 16), // "on_upBtn_clicked"
QT_MOC_LITERAL(54, 864, 18), // "on_downBtn_clicked"
QT_MOC_LITERAL(55, 883, 29), // "on_resetDobotIndexBtn_clicked"
QT_MOC_LITERAL(56, 913, 16), // "on_AIBtn_clicked"
QT_MOC_LITERAL(57, 930, 25), // "on_AIEnemySendBtn_clicked"
QT_MOC_LITERAL(58, 956, 15), // "showClientsList"
QT_MOC_LITERAL(59, 972, 13), // "QList<Client>"
QT_MOC_LITERAL(60, 986, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(61, 1006, 9), // "nUsbPorst"
QT_MOC_LITERAL(62, 1016, 36), // "on_portsComboBox_currentIndex..."
QT_MOC_LITERAL(63, 1053, 25), // "on_reloadPortsBtn_clicked"
QT_MOC_LITERAL(64, 1079, 21), // "on_sendUsbBtn_clicked"
QT_MOC_LITERAL(65, 1101, 25), // "on_openGripperBtn_clicked"
QT_MOC_LITERAL(66, 1127, 26), // "on_closeGripperBtn_clicked"
QT_MOC_LITERAL(67, 1154, 24), // "on_startGmPosBtn_clicked"
QT_MOC_LITERAL(68, 1179, 24), // "on_startDtPosBtn_clicked"
QT_MOC_LITERAL(69, 1204, 29), // "on_SimulateFromUsbBtn_clicked"
QT_MOC_LITERAL(70, 1234, 21), // "on_sendTcpBtn_clicked"
QT_MOC_LITERAL(71, 1256, 30), // "on_sendTcpLineEdit_textChanged"
QT_MOC_LITERAL(72, 1287, 38), // "on_SimulateFromUsbLineEdit_te..."
QT_MOC_LITERAL(73, 1326, 18), // "on_homeBtn_clicked"
QT_MOC_LITERAL(74, 1345, 25), // "on_usbCmdLine_textChanged"
QT_MOC_LITERAL(75, 1371, 25), // "on_middleAboveBtn_clicked"
QT_MOC_LITERAL(76, 1397, 18), // "on_sendBtn_clicked"
QT_MOC_LITERAL(77, 1416, 23), // "on_xPTPEdit_textChanged"
QT_MOC_LITERAL(78, 1440, 23), // "on_yPTPEdit_textChanged"
QT_MOC_LITERAL(79, 1464, 23), // "on_zPTPEdit_textChanged"
QT_MOC_LITERAL(80, 1488, 25), // "on_retreatLeftBtn_clicked"
QT_MOC_LITERAL(81, 1514, 26) // "on_retreatRightBtn_clicked"

    },
    "MainWindow\0onChangedMode\0\0onJOGCtrlBtnPressed\0"
    "nID\0onJOGCtrlBtnReleased\0writeInConsole\0"
    "QStrMsg\0LOG\0TypeOfMsg\0setBoardDataLabel\0"
    "QStrLabel\0BOARD_DATA_LABEL\0LabelType\0"
    "showBoardInUI\0QStrBoard\0showRealBoardInUI\0"
    "clearBoardInUI\0showLegalMovesInUI\0"
    "legalMoves\0showHistoryMovesInUI\0"
    "historyMoves\0changeWindowTitle\0"
    "setDobotPTEValidatorsInUI\0setJointLabelText\0"
    "QStrJointLabelText\0sJoint\0setAxisLabelText\0"
    "QStrAxisLabelText\0chAxis\0setDobotButtonsStates\0"
    "bDobotButtonsStates\0setDeviceLabels\0"
    "QStrDeviceSN\0QStrDeviceName\0"
    "QStrDeviceVersion\0showDobotErrorMsgBox\0"
    "on_emulatePlayerMsgLineEdit_textChanged\0"
    "QStrTextChanged\0on_sendSimulatedMsgBtn_clicked\0"
    "setQueueLabels\0nSpaceLeft\0nDobotId\0"
    "nCoreMaxId\0nCoreIdLeft\0nCoreNextId\0"
    "showActualDobotQueuedCmdIDList\0"
    "QList<DobotMove>\0list\0showOnDobotQueuedCmdsList\0"
    "showArduinoGripperStateList\0"
    "QList<ServoArduino>\0on_calibrateBtn_clicked\0"
    "on_upBtn_clicked\0on_downBtn_clicked\0"
    "on_resetDobotIndexBtn_clicked\0"
    "on_AIBtn_clicked\0on_AIEnemySendBtn_clicked\0"
    "showClientsList\0QList<Client>\0"
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
    "on_homeBtn_clicked\0on_usbCmdLine_textChanged\0"
    "on_middleAboveBtn_clicked\0on_sendBtn_clicked\0"
    "on_xPTPEdit_textChanged\0on_yPTPEdit_textChanged\0"
    "on_zPTPEdit_textChanged\0"
    "on_retreatLeftBtn_clicked\0"
    "on_retreatRightBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      51,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  269,    2, 0x08 /* Private */,
       3,    1,  270,    2, 0x08 /* Private */,
       5,    0,  273,    2, 0x08 /* Private */,
       6,    2,  274,    2, 0x08 /* Private */,
      10,    2,  279,    2, 0x08 /* Private */,
      14,    1,  284,    2, 0x08 /* Private */,
      16,    0,  287,    2, 0x08 /* Private */,
      17,    0,  288,    2, 0x08 /* Private */,
      18,    1,  289,    2, 0x08 /* Private */,
      20,    1,  292,    2, 0x08 /* Private */,
      22,    0,  295,    2, 0x08 /* Private */,
      23,    0,  296,    2, 0x08 /* Private */,
      24,    2,  297,    2, 0x08 /* Private */,
      27,    2,  302,    2, 0x08 /* Private */,
      30,    1,  307,    2, 0x08 /* Private */,
      32,    3,  310,    2, 0x08 /* Private */,
      36,    0,  317,    2, 0x08 /* Private */,
      37,    1,  318,    2, 0x08 /* Private */,
      39,    0,  321,    2, 0x08 /* Private */,
      40,    5,  322,    2, 0x08 /* Private */,
      46,    1,  333,    2, 0x08 /* Private */,
      49,    1,  336,    2, 0x08 /* Private */,
      50,    1,  339,    2, 0x08 /* Private */,
      52,    0,  342,    2, 0x08 /* Private */,
      53,    0,  343,    2, 0x08 /* Private */,
      54,    0,  344,    2, 0x08 /* Private */,
      55,    0,  345,    2, 0x08 /* Private */,
      56,    0,  346,    2, 0x08 /* Private */,
      57,    0,  347,    2, 0x08 /* Private */,
      58,    1,  348,    2, 0x08 /* Private */,
      60,    1,  351,    2, 0x08 /* Private */,
      62,    1,  354,    2, 0x08 /* Private */,
      63,    0,  357,    2, 0x08 /* Private */,
      64,    0,  358,    2, 0x08 /* Private */,
      65,    0,  359,    2, 0x08 /* Private */,
      66,    0,  360,    2, 0x08 /* Private */,
      67,    0,  361,    2, 0x08 /* Private */,
      68,    0,  362,    2, 0x08 /* Private */,
      69,    0,  363,    2, 0x08 /* Private */,
      70,    0,  364,    2, 0x08 /* Private */,
      71,    1,  365,    2, 0x08 /* Private */,
      72,    1,  368,    2, 0x08 /* Private */,
      73,    0,  371,    2, 0x08 /* Private */,
      74,    1,  372,    2, 0x08 /* Private */,
      75,    0,  375,    2, 0x08 /* Private */,
      76,    0,  376,    2, 0x08 /* Private */,
      77,    1,  377,    2, 0x08 /* Private */,
      78,    1,  380,    2, 0x08 /* Private */,
      79,    1,  383,    2, 0x08 /* Private */,
      80,    0,  386,    2, 0x08 /* Private */,
      81,    0,  387,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   19,
    QMetaType::Void, QMetaType::QStringList,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,   25,   26,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   28,   29,
    QMetaType::Void, QMetaType::Bool,   31,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   33,   34,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   41,   42,   43,   44,   45,
    QMetaType::Void, 0x80000000 | 47,   48,
    QMetaType::Void, 0x80000000 | 47,   48,
    QMetaType::Void, 0x80000000 | 51,   48,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 59,   48,
    QMetaType::Void, QMetaType::Int,   61,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   38,
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
        case 0: _t->onChangedMode(); break;
        case 1: _t->onJOGCtrlBtnPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onJOGCtrlBtnReleased(); break;
        case 3: _t->writeInConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 4: _t->setBoardDataLabel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 5: _t->showBoardInUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->showRealBoardInUI(); break;
        case 7: _t->clearBoardInUI(); break;
        case 8: _t->showLegalMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 9: _t->showHistoryMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->changeWindowTitle(); break;
        case 11: _t->setDobotPTEValidatorsInUI(); break;
        case 12: _t->setJointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 13: _t->setAxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 14: _t->setDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setDeviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 16: _t->showDobotErrorMsgBox(); break;
        case 17: _t->on_emulatePlayerMsgLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->on_sendSimulatedMsgBtn_clicked(); break;
        case 19: _t->setQueueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 20: _t->showActualDobotQueuedCmdIDList((*reinterpret_cast< QList<DobotMove>(*)>(_a[1]))); break;
        case 21: _t->showOnDobotQueuedCmdsList((*reinterpret_cast< QList<DobotMove>(*)>(_a[1]))); break;
        case 22: _t->showArduinoGripperStateList((*reinterpret_cast< QList<ServoArduino>(*)>(_a[1]))); break;
        case 23: _t->on_calibrateBtn_clicked(); break;
        case 24: _t->on_upBtn_clicked(); break;
        case 25: _t->on_downBtn_clicked(); break;
        case 26: _t->on_resetDobotIndexBtn_clicked(); break;
        case 27: _t->on_AIBtn_clicked(); break;
        case 28: _t->on_AIEnemySendBtn_clicked(); break;
        case 29: _t->showClientsList((*reinterpret_cast< QList<Client>(*)>(_a[1]))); break;
        case 30: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->on_portsComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->on_reloadPortsBtn_clicked(); break;
        case 33: _t->on_sendUsbBtn_clicked(); break;
        case 34: _t->on_openGripperBtn_clicked(); break;
        case 35: _t->on_closeGripperBtn_clicked(); break;
        case 36: _t->on_startGmPosBtn_clicked(); break;
        case 37: _t->on_startDtPosBtn_clicked(); break;
        case 38: _t->on_SimulateFromUsbBtn_clicked(); break;
        case 39: _t->on_sendTcpBtn_clicked(); break;
        case 40: _t->on_sendTcpLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 41: _t->on_SimulateFromUsbLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 42: _t->on_homeBtn_clicked(); break;
        case 43: _t->on_usbCmdLine_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 44: _t->on_middleAboveBtn_clicked(); break;
        case 45: _t->on_sendBtn_clicked(); break;
        case 46: _t->on_xPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 47: _t->on_yPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 48: _t->on_zPTPEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 49: _t->on_retreatLeftBtn_clicked(); break;
        case 50: _t->on_retreatRightBtn_clicked(); break;
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
        if (_id < 51)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 51;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 51)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 51;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
