/****************************************************************************
** Meta object code from reading C++ file 'dobot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
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
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Dobot_t {
    QByteArrayData data[32];
    char stringdata0[484];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dobot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dobot_t qt_meta_stringdata_Dobot = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Dobot"
QT_MOC_LITERAL(1, 6, 16), // "addTextToConsole"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 3), // "LOG"
QT_MOC_LITERAL(4, 28, 14), // "JointLabelText"
QT_MOC_LITERAL(5, 43, 11), // "QSLabelText"
QT_MOC_LITERAL(6, 55, 6), // "sJoint"
QT_MOC_LITERAL(7, 62, 13), // "AxisLabelText"
QT_MOC_LITERAL(8, 76, 15), // "QSAxisLabelText"
QT_MOC_LITERAL(9, 92, 6), // "chAxis"
QT_MOC_LITERAL(10, 99, 25), // "RefreshDobotButtonsStates"
QT_MOC_LITERAL(11, 125, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(12, 145, 12), // "deviceLabels"
QT_MOC_LITERAL(13, 158, 10), // "QSdeviceSN"
QT_MOC_LITERAL(14, 169, 12), // "QSdeviceName"
QT_MOC_LITERAL(15, 182, 15), // "QSdeviceVersion"
QT_MOC_LITERAL(16, 198, 16), // "DobotErrorMsgBox"
QT_MOC_LITERAL(17, 215, 11), // "QueueLabels"
QT_MOC_LITERAL(18, 227, 6), // "nSpace"
QT_MOC_LITERAL(19, 234, 8), // "nDobotId"
QT_MOC_LITERAL(20, 243, 10), // "nCoreMaxId"
QT_MOC_LITERAL(21, 254, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(22, 266, 10), // "CoreNextId"
QT_MOC_LITERAL(23, 277, 18), // "setBoardDataLabels"
QT_MOC_LITERAL(24, 296, 17), // "BOARD_DATA_LABELS"
QT_MOC_LITERAL(25, 314, 33), // "showActualDobotQueuedCmdIndex..."
QT_MOC_LITERAL(26, 348, 37), // "QList<ArmPosForCurrentCmdQueu..."
QT_MOC_LITERAL(27, 386, 27), // "showArduinoGripperStateList"
QT_MOC_LITERAL(28, 414, 19), // "QList<ServoArduino>"
QT_MOC_LITERAL(29, 434, 14), // "onConnectDobot"
QT_MOC_LITERAL(30, 449, 19), // "onPeriodicTaskTimer"
QT_MOC_LITERAL(31, 469, 14) // "onGetPoseTimer"

    },
    "Dobot\0addTextToConsole\0\0LOG\0JointLabelText\0"
    "QSLabelText\0sJoint\0AxisLabelText\0"
    "QSAxisLabelText\0chAxis\0RefreshDobotButtonsStates\0"
    "bDobotButtonsStates\0deviceLabels\0"
    "QSdeviceSN\0QSdeviceName\0QSdeviceVersion\0"
    "DobotErrorMsgBox\0QueueLabels\0nSpace\0"
    "nDobotId\0nCoreMaxId\0nCoreIdLeft\0"
    "CoreNextId\0setBoardDataLabels\0"
    "BOARD_DATA_LABELS\0showActualDobotQueuedCmdIndexList\0"
    "QList<ArmPosForCurrentCmdQueuedIndex>\0"
    "showArduinoGripperStateList\0"
    "QList<ServoArduino>\0onConnectDobot\0"
    "onPeriodicTaskTimer\0onGetPoseTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dobot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       4,    2,   84,    2, 0x06 /* Public */,
       7,    2,   89,    2, 0x06 /* Public */,
      10,    1,   94,    2, 0x06 /* Public */,
      12,    3,   97,    2, 0x06 /* Public */,
      16,    0,  104,    2, 0x06 /* Public */,
      17,    5,  105,    2, 0x06 /* Public */,
      23,    2,  116,    2, 0x06 /* Public */,
      25,    1,  121,    2, 0x06 /* Public */,
      27,    1,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      29,    0,  127,    2, 0x0a /* Public */,
      30,    0,  128,    2, 0x0a /* Public */,
      31,    0,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   13,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   18,   19,   20,   21,   22,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 24,    2,    2,
    QMetaType::Void, 0x80000000 | 26,    2,
    QMetaType::Void, 0x80000000 | 28,    2,

 // slots: parameters
    QMetaType::Void,
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
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->JointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 2: _t->AxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 3: _t->RefreshDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->deviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->DobotErrorMsgBox(); break;
        case 6: _t->QueueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->setBoardDataLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABELS(*)>(_a[2]))); break;
        case 8: _t->showActualDobotQueuedCmdIndexList((*reinterpret_cast< QList<ArmPosForCurrentCmdQueuedIndex>(*)>(_a[1]))); break;
        case 9: _t->showArduinoGripperStateList((*reinterpret_cast< QList<ServoArduino>(*)>(_a[1]))); break;
        case 10: _t->onConnectDobot(); break;
        case 11: _t->onPeriodicTaskTimer(); break;
        case 12: _t->onGetPoseTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Dobot::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::addTextToConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , short );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::JointLabelText)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::AxisLabelText)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::RefreshDobotButtonsStates)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::deviceLabels)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::DobotErrorMsgBox)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(int , int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::QueueLabels)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QString , BOARD_DATA_LABELS );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::setBoardDataLabels)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QList<ArmPosForCurrentCmdQueuedIndex> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::showActualDobotQueuedCmdIndexList)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Dobot::*_t)(QList<ServoArduino> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::showArduinoGripperStateList)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject Dobot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Dobot.data,
      qt_meta_data_Dobot,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Dobot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dobot::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Dobot.stringdata0))
        return static_cast<void*>(const_cast< Dobot*>(this));
    return QObject::qt_metacast(_clname);
}

int Dobot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Dobot::addTextToConsole(QString _t1, LOG _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Dobot::JointLabelText(QString _t1, short _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Dobot::AxisLabelText(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Dobot::RefreshDobotButtonsStates(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Dobot::deviceLabels(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Dobot::DobotErrorMsgBox()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void Dobot::QueueLabels(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Dobot::setBoardDataLabels(QString _t1, BOARD_DATA_LABELS _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Dobot::showActualDobotQueuedCmdIndexList(QList<ArmPosForCurrentCmdQueuedIndex> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Dobot::showArduinoGripperStateList(QList<ServoArduino> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
