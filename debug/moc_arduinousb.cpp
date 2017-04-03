/****************************************************************************
** Meta object code from reading C++ file 'arduinousb.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../arduinousb.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arduinousb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ArduinoUsb_t {
    QByteArrayData data[10];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArduinoUsb_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArduinoUsb_t qt_meta_stringdata_ArduinoUsb = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ArduinoUsb"
QT_MOC_LITERAL(1, 11, 16), // "addTextToConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(4, 49, 12), // "AIEnemyStart"
QT_MOC_LITERAL(5, 62, 11), // "AIEnemySend"
QT_MOC_LITERAL(6, 74, 11), // "TcpQueueMsg"
QT_MOC_LITERAL(7, 86, 16), // "portIndexChanged"
QT_MOC_LITERAL(8, 103, 5), // "index"
QT_MOC_LITERAL(9, 109, 11) // "readUsbData"

    },
    "ArduinoUsb\0addTextToConsole\0\0"
    "updatePortsComboBox\0AIEnemyStart\0"
    "AIEnemySend\0TcpQueueMsg\0portIndexChanged\0"
    "index\0readUsbData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArduinoUsb[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       3,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,
       5,    1,   58,    2, 0x06 /* Public */,
       6,    2,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   66,    2, 0x0a /* Public */,
       9,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

void ArduinoUsb::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ArduinoUsb *_t = static_cast<ArduinoUsb *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 1: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->AIEnemyStart(); break;
        case 3: _t->AIEnemySend((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->TcpQueueMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->portIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->readUsbData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ArduinoUsb::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::addTextToConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ArduinoUsb::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::updatePortsComboBox)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ArduinoUsb::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::AIEnemyStart)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ArduinoUsb::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::AIEnemySend)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ArduinoUsb::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::TcpQueueMsg)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject ArduinoUsb::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ArduinoUsb.data,
      qt_meta_data_ArduinoUsb,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ArduinoUsb::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArduinoUsb::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ArduinoUsb.stringdata0))
        return static_cast<void*>(const_cast< ArduinoUsb*>(this));
    return QObject::qt_metacast(_clname);
}

int ArduinoUsb::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ArduinoUsb::addTextToConsole(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArduinoUsb::updatePortsComboBox(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ArduinoUsb::AIEnemyStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ArduinoUsb::AIEnemySend(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ArduinoUsb::TcpQueueMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
