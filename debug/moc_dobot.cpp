/****************************************************************************
** Meta object code from reading C++ file 'dobot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dobot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dobot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Dobot_t {
    QByteArrayData data[26];
    char stringdata0[335];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dobot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dobot_t qt_meta_stringdata_Dobot = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Dobot"
QT_MOC_LITERAL(1, 6, 21), // "addTextToDobotConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "QS_msg"
QT_MOC_LITERAL(4, 36, 14), // "JointLabelText"
QT_MOC_LITERAL(5, 51, 11), // "QSLabelText"
QT_MOC_LITERAL(6, 63, 6), // "sJoint"
QT_MOC_LITERAL(7, 70, 13), // "AxisLabelText"
QT_MOC_LITERAL(8, 84, 15), // "QSAxisLabelText"
QT_MOC_LITERAL(9, 100, 6), // "chAxis"
QT_MOC_LITERAL(10, 107, 25), // "RefreshDobotButtonsStates"
QT_MOC_LITERAL(11, 133, 19), // "bDobotButtonsStates"
QT_MOC_LITERAL(12, 153, 12), // "deviceLabels"
QT_MOC_LITERAL(13, 166, 10), // "QSdeviceSN"
QT_MOC_LITERAL(14, 177, 12), // "QSdeviceName"
QT_MOC_LITERAL(15, 190, 15), // "QSdeviceVersion"
QT_MOC_LITERAL(16, 206, 16), // "DobotErrorMsgBox"
QT_MOC_LITERAL(17, 223, 11), // "QueueLabels"
QT_MOC_LITERAL(18, 235, 6), // "nSpace"
QT_MOC_LITERAL(19, 242, 8), // "nDobotId"
QT_MOC_LITERAL(20, 251, 10), // "nCoreMaxId"
QT_MOC_LITERAL(21, 262, 11), // "nCoreIdLeft"
QT_MOC_LITERAL(22, 274, 10), // "CoreNextId"
QT_MOC_LITERAL(23, 285, 14), // "onConnectDobot"
QT_MOC_LITERAL(24, 300, 19), // "onPeriodicTaskTimer"
QT_MOC_LITERAL(25, 320, 14) // "onGetPoseTimer"

    },
    "Dobot\0addTextToDobotConsole\0\0QS_msg\0"
    "JointLabelText\0QSLabelText\0sJoint\0"
    "AxisLabelText\0QSAxisLabelText\0chAxis\0"
    "RefreshDobotButtonsStates\0bDobotButtonsStates\0"
    "deviceLabels\0QSdeviceSN\0QSdeviceName\0"
    "QSdeviceVersion\0DobotErrorMsgBox\0"
    "QueueLabels\0nSpace\0nDobotId\0nCoreMaxId\0"
    "nCoreIdLeft\0CoreNextId\0onConnectDobot\0"
    "onPeriodicTaskTimer\0onGetPoseTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dobot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    2,   67,    2, 0x06 /* Public */,
       7,    2,   72,    2, 0x06 /* Public */,
      10,    1,   77,    2, 0x06 /* Public */,
      12,    3,   80,    2, 0x06 /* Public */,
      16,    0,   87,    2, 0x06 /* Public */,
      17,    5,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      23,    0,   99,    2, 0x0a /* Public */,
      24,    0,  100,    2, 0x0a /* Public */,
      25,    0,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   13,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   18,   19,   20,   21,   22,

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
        case 0: _t->addTextToDobotConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->JointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 2: _t->AxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 3: _t->RefreshDobotButtonsStates((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->deviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->DobotErrorMsgBox(); break;
        case 6: _t->QueueLabels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->onConnectDobot(); break;
        case 8: _t->onPeriodicTaskTimer(); break;
        case 9: _t->onGetPoseTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Dobot::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dobot::addTextToDobotConsole)) {
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Dobot::addTextToDobotConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
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
QT_END_MOC_NAMESPACE
