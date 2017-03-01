/****************************************************************************
** Meta object code from reading C++ file 'tcpmsgs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcpmsgs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpmsgs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCPMsgs_t {
    QByteArrayData data[9];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPMsgs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPMsgs_t qt_meta_stringdata_TCPMsgs = {
    {
QT_MOC_LITERAL(0, 0, 7), // "TCPMsgs"
QT_MOC_LITERAL(1, 8, 19), // "addTextToTcpConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "MsgFromChenard"
QT_MOC_LITERAL(4, 44, 9), // "connected"
QT_MOC_LITERAL(5, 54, 12), // "disconnected"
QT_MOC_LITERAL(6, 67, 12), // "bytesWritten"
QT_MOC_LITERAL(7, 80, 5), // "bytes"
QT_MOC_LITERAL(8, 86, 9) // "readyRead"

    },
    "TCPMsgs\0addTextToTcpConsole\0\0"
    "MsgFromChenard\0connected\0disconnected\0"
    "bytesWritten\0bytes\0readyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPMsgs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       3,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   52,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    7,
    QMetaType::Void,

       0        // eod
};

void TCPMsgs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TCPMsgs *_t = static_cast<TCPMsgs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToTcpConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 1: _t->MsgFromChenard((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->connected(); break;
        case 3: _t->disconnected(); break;
        case 4: _t->bytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 5: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TCPMsgs::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::addTextToTcpConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TCPMsgs::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::MsgFromChenard)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TCPMsgs::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TCPMsgs.data,
      qt_meta_data_TCPMsgs,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TCPMsgs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPMsgs::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TCPMsgs.stringdata0))
        return static_cast<void*>(const_cast< TCPMsgs*>(this));
    return QObject::qt_metacast(_clname);
}

int TCPMsgs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TCPMsgs::addTextToTcpConsole(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCPMsgs::MsgFromChenard(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
