/****************************************************************************
** Meta object code from reading C++ file 'dobot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dobot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dobot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Dobot_t {
    QByteArrayData data[39];
    char stringdata0[562];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dobot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dobot_t qt_meta_stringdata_Dobot = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Dobot"
QT_MOC_LITERAL(1, 6, 15), // "addTextToLogPTE"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "LOG"
QT_MOC_LITERAL(4, 27, 14), // "JointLabelText"
QT_MOC_LITERAL(5, 42, 13), // "QStrLabelText"
QT_MOC_LITERAL(6, 56, 6), // "sJoint"
QT_MOC_LITERAL(7, 63, 13), // "AxisLabelText"
QT_MOC_LITERAL(8, 77, 17), // "QStrAxisLabelText"
QT_MOC_LITERAL(9, 95, 6), // "chAxis"
QT_MOC_LITERAL(10, 102, 21), // "setDobotButtonsStates"
QT_MOC_LITERAL(11, 124, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(12, 144, 12), // "deviceLabels"
QT_MOC_LITERAL(13, 157, 12), // "QStrDeviceSN"
QT_MOC_LITERAL(14, 170, 14), // "QStrDeviceName"
QT_MOC_LITERAL(15, 185, 17), // "QStrDeviceVersion"
QT_MOC_LITERAL(16, 203, 16), // "DobotErrorMsgBox"
QT_MOC_LITERAL(17, 220, 11), // "queueLabels"
QT_MOC_LITERAL(18, 232, 6), // "nSpace"
QT_MOC_LITERAL(19, 239, 8), // "nDobotId"
QT_MOC_LITERAL(20, 248, 10), // "nCoreMaxId"
QT_MOC_LITERAL(21, 259, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(22, 271, 11), // "nCoreNextId"
QT_MOC_LITERAL(23, 283, 27), // "showArduinoGripperStateList"
QT_MOC_LITERAL(24, 311, 19), // "QList<ServoArduino>"
QT_MOC_LITERAL(25, 331, 23), // "showQueuedArmCmdsOnCore"
QT_MOC_LITERAL(26, 355, 22), // "showSentArmCmdsToDobot"
QT_MOC_LITERAL(27, 378, 13), // "sendMoveToArm"
QT_MOC_LITERAL(28, 392, 9), // "DobotMove"
QT_MOC_LITERAL(29, 402, 4), // "move"
QT_MOC_LITERAL(30, 407, 14), // "onConnectDobot"
QT_MOC_LITERAL(31, 422, 19), // "onPeriodicTaskTimer"
QT_MOC_LITERAL(32, 442, 14), // "onGetPoseTimer"
QT_MOC_LITERAL(33, 457, 19), // "showQueueLabelsInUI"
QT_MOC_LITERAL(34, 477, 19), // "addTextToLogPTEInUI"
QT_MOC_LITERAL(35, 497, 7), // "QStrTxt"
QT_MOC_LITERAL(36, 505, 3), // "log"
QT_MOC_LITERAL(37, 509, 26), // "showQueuedArmCmdsOnCorePTE"
QT_MOC_LITERAL(38, 536, 25) // "showSentArmCmdsToDobotPTE"

    },
    "Dobot\0addTextToLogPTE\0\0LOG\0JointLabelText\0"
    "QStrLabelText\0sJoint\0AxisLabelText\0"
    "QStrAxisLabelText\0chAxis\0setDobotButtonsStates\0"
    "bDobotButtonsStates\0deviceLabels\0"
    "QStrDeviceSN\0QStrDeviceName\0"
    "QStrDeviceVersion\0DobotErrorMsgBox\0"
    "queueLabels\0nSpace\0nDobotId\0nCoreMaxId\0"
    "nCoreIdLeft\0nCoreNextId\0"
    "showArduinoGripperStateList\0"
    "QList<ServoArduino>\0showQueuedArmCmdsOnCore\0"
    "showSentArmCmdsToDobot\0sendMoveToArm\0"
    "DobotMove\0move\0onConnectDobot\0"
    "onPeriodicTaskTimer\0onGetPoseTimer\0"
    "showQueueLabelsInUI\0addTextToLogPTEInUI\0"
    "QStrTxt\0log\0showQueuedArmCmdsOnCorePTE\0"
    "showSentArmCmdsToDobotPTE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dobot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  104,    2, 0x06 /* Public */,
       4,    2,  109,    2, 0x06 /* Public */,
       7,    2,  114,    2, 0x06 /* Public */,
      10,    1,  119,    2, 0x06 /* Public */,
      12,    3,  122,    2, 0x06 /* Public */,
      16,    0,  129,    2, 0x06 /* Public */,
      17,    5,  130,    2, 0x06 /* Public */,
      23,    1,  141,    2, 0x06 /* Public */,
      25,    0,  144,    2, 0x06 /* Public */,
      26,    0,  145,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    1,  146,    2, 0x0a /* Public */,
      30,    0,  149,    2, 0x0a /* Public */,
      31,    0,  150,    2, 0x0a /* Public */,
      32,    0,  151,    2, 0x0a /* Public */,
      33,    5,  152,    2, 0x0a /* Public */,
      34,    2,  163,    2, 0x0a /* Public */,
      37,    0,  168,    2, 0x0a /* Public */,
      38,    0,  169,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   13,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   18,   19,   20,   21,   22,
    QMetaType::Void, 0x80000000 | 24,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   18,   19,   20,   21,   22,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,   35,   36,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Dobot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dobot *_t = static_cast<Dobot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToLogPTE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->JointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 2: _t->AxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 3: _t->setDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->deviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->DobotErrorMsgBox(); break;
        case 6: _t->queueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->showArduinoGripperStateList((*reinterpret_cast< QList<ServoArduino>(*)>(_a[1]))); break;
        case 8: _t->showQueuedArmCmdsOnCore(); break;
        case 9: _t->showSentArmCmdsToDobot(); break;
        case 10: _t->sendMoveToArm((*reinterpret_cast< DobotMove(*)>(_a[1]))); break;
        case 11: _t->onConnectDobot(); break;
        case 12: _t->onPeriodicTaskTimer(); break;
        case 13: _t->onGetPoseTimer(); break;
        case 14: _t->showQueueLabelsInUI((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 15: _t->addTextToLogPTEInUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 16: _t->showQueuedArmCmdsOnCorePTE(); break;
        case 17: _t->showSentArmCmdsToDobotPTE(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Dobot::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::addTextToLogPTE)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::JointLabelText)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::AxisLabelText)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::setDobotButtonsStates)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::deviceLabels)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::DobotErrorMsgBox)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(int , int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::queueLabels)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QList<ServoArduino> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::showArduinoGripperStateList)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::showQueuedArmCmdsOnCore)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Dobot::showSentArmCmdsToDobot)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Dobot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Dobot.data,
      qt_meta_data_Dobot,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Dobot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dobot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Dobot.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Dobot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void Dobot::addTextToLogPTE(QString _t1, LOG _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Dobot::JointLabelText(QString _t1, short _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Dobot::AxisLabelText(QString _t1, char _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Dobot::setDobotButtonsStates(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Dobot::deviceLabels(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Dobot::DobotErrorMsgBox()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Dobot::queueLabels(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Dobot::showArduinoGripperStateList(QList<ServoArduino> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Dobot::showQueuedArmCmdsOnCore()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Dobot::showSentArmCmdsToDobot()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
