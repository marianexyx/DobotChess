/****************************************************************************
** Meta object code from reading C++ file 'dobot_queue.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dobot/dobot_queue.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dobot_queue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DobotQueue_t {
    QByteArrayData data[9];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DobotQueue_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DobotQueue_t qt_meta_stringdata_DobotQueue = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DobotQueue"
QT_MOC_LITERAL(1, 11, 30), // "showActualDobotQueuedCmdIDList"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 16), // "QList<DobotMove>"
QT_MOC_LITERAL(4, 60, 13), // "sendMoveToArm"
QT_MOC_LITERAL(5, 74, 9), // "DobotMove"
QT_MOC_LITERAL(6, 84, 19), // "showQueueLabelsInUI"
QT_MOC_LITERAL(7, 104, 19), // "addTextToLogPTEInUI"
QT_MOC_LITERAL(8, 124, 3) // "LOG"

    },
    "DobotQueue\0showActualDobotQueuedCmdIDList\0"
    "\0QList<DobotMove>\0sendMoveToArm\0"
    "DobotMove\0showQueueLabelsInUI\0"
    "addTextToLogPTEInUI\0LOG"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DobotQueue[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       6,    5,   40,    2, 0x06 /* Public */,
       7,    2,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    2,    2,

       0        // eod
};

void DobotQueue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DobotQueue *_t = static_cast<DobotQueue *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showActualDobotQueuedCmdIDList((*reinterpret_cast< QList<DobotMove>(*)>(_a[1]))); break;
        case 1: _t->sendMoveToArm((*reinterpret_cast< DobotMove(*)>(_a[1]))); break;
        case 2: _t->showQueueLabelsInUI((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: _t->addTextToLogPTEInUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DobotQueue::*_t)(QList<DobotMove> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DobotQueue::showActualDobotQueuedCmdIDList)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DobotQueue::*_t)(DobotMove );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DobotQueue::sendMoveToArm)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DobotQueue::*_t)(int , int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DobotQueue::showQueueLabelsInUI)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (DobotQueue::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DobotQueue::addTextToLogPTEInUI)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DobotQueue::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DobotQueue.data,
      qt_meta_data_DobotQueue,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DobotQueue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DobotQueue::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DobotQueue.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DobotQueue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DobotQueue::showActualDobotQueuedCmdIDList(QList<DobotMove> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DobotQueue::sendMoveToArm(DobotMove _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DobotQueue::showQueueLabelsInUI(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DobotQueue::addTextToLogPTEInUI(QString _t1, LOG _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
