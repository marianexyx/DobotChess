/****************************************************************************
** Meta object code from reading C++ file 'tcpmsgs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcpmsgs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpmsgs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCPMsgs_t {
    QByteArrayData data[13];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPMsgs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPMsgs_t qt_meta_stringdata_TCPMsgs = {
    {
QT_MOC_LITERAL(0, 0, 7), // "TCPMsgs"
QT_MOC_LITERAL(1, 8, 16), // "addTextToConsole"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "msgFromTcpToWeb"
QT_MOC_LITERAL(4, 42, 15), // "msgFromTcpToArd"
QT_MOC_LITERAL(5, 58, 9), // "connected"
QT_MOC_LITERAL(6, 68, 12), // "disconnected"
QT_MOC_LITERAL(7, 81, 12), // "bytesWritten"
QT_MOC_LITERAL(8, 94, 5), // "bytes"
QT_MOC_LITERAL(9, 100, 9), // "readyRead"
QT_MOC_LITERAL(10, 110, 11), // "TcpQueueMsg"
QT_MOC_LITERAL(11, 122, 7), // "nSender"
QT_MOC_LITERAL(12, 130, 3) // "msg"

    },
    "TCPMsgs\0addTextToConsole\0\0msgFromTcpToWeb\0"
    "msgFromTcpToArd\0connected\0disconnected\0"
    "bytesWritten\0bytes\0readyRead\0TcpQueueMsg\0"
    "nSender\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPMsgs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       3,    2,   59,    2, 0x06 /* Public */,
       4,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   69,    2, 0x08 /* Private */,
       6,    0,   70,    2, 0x08 /* Private */,
       7,    1,   71,    2, 0x08 /* Private */,
       9,    0,   74,    2, 0x08 /* Private */,
      10,    2,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   11,   12,

       0        // eod
};

void TCPMsgs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TCPMsgs *_t = static_cast<TCPMsgs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 1: _t->msgFromTcpToWeb((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->msgFromTcpToArd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->connected(); break;
        case 4: _t->disconnected(); break;
        case 5: _t->bytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: _t->readyRead(); break;
        case 7: _t->TcpQueueMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TCPMsgs::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::addTextToConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TCPMsgs::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::msgFromTcpToWeb)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TCPMsgs::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::msgFromTcpToArd)) {
                *result = 2;
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TCPMsgs::addTextToConsole(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCPMsgs::msgFromTcpToWeb(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TCPMsgs::msgFromTcpToArd(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
