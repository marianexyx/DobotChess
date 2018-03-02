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
    QByteArrayData data[9];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArduinoUsb_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArduinoUsb_t qt_meta_stringdata_ArduinoUsb = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ArduinoUsb"
QT_MOC_LITERAL(1, 11, 15), // "addTextToLogPTE"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "LOG"
QT_MOC_LITERAL(4, 32, 19), // "updatePortsComboBox"
QT_MOC_LITERAL(5, 52, 17), // "msgFromUsbToChess"
QT_MOC_LITERAL(6, 70, 16), // "portIndexChanged"
QT_MOC_LITERAL(7, 87, 6), // "nIndex"
QT_MOC_LITERAL(8, 94, 11) // "readUsbData"

    },
    "ArduinoUsb\0addTextToLogPTE\0\0LOG\0"
    "updatePortsComboBox\0msgFromUsbToChess\0"
    "portIndexChanged\0nIndex\0readUsbData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArduinoUsb[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       4,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   50,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,

       0        // eod
};

void ArduinoUsb::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ArduinoUsb *_t = static_cast<ArduinoUsb *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToLogPTE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->updatePortsComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->msgFromUsbToChess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->portIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->readUsbData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ArduinoUsb::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::addTextToLogPTE)) {
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
            typedef void (ArduinoUsb::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArduinoUsb::msgFromUsbToChess)) {
                *result = 2;
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
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ArduinoUsb::addTextToLogPTE(QString _t1, LOG _t2)
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
void ArduinoUsb::msgFromUsbToChess(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
