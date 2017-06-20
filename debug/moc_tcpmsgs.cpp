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
    QByteArrayData data[18];
    char stringdata0[209];
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
QT_MOC_LITERAL(3, 26, 3), // "LOG"
QT_MOC_LITERAL(4, 30, 15), // "msgFromTcpToWeb"
QT_MOC_LITERAL(5, 46, 15), // "msgFromTcpToArd"
QT_MOC_LITERAL(6, 62, 16), // "msgFromTcpToCore"
QT_MOC_LITERAL(7, 79, 12), // "displayError"
QT_MOC_LITERAL(8, 92, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(9, 121, 11), // "socketError"
QT_MOC_LITERAL(10, 133, 9), // "connected"
QT_MOC_LITERAL(11, 143, 12), // "disconnected"
QT_MOC_LITERAL(12, 156, 12), // "bytesWritten"
QT_MOC_LITERAL(13, 169, 5), // "bytes"
QT_MOC_LITERAL(14, 175, 9), // "readyRead"
QT_MOC_LITERAL(15, 185, 11), // "TcpQueueMsg"
QT_MOC_LITERAL(16, 197, 7), // "nSender"
QT_MOC_LITERAL(17, 205, 3) // "msg"

    },
    "TCPMsgs\0addTextToConsole\0\0LOG\0"
    "msgFromTcpToWeb\0msgFromTcpToArd\0"
    "msgFromTcpToCore\0displayError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "connected\0disconnected\0bytesWritten\0"
    "bytes\0readyRead\0TcpQueueMsg\0nSender\0"
    "msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPMsgs[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       4,    2,   69,    2, 0x06 /* Public */,
       5,    2,   74,    2, 0x06 /* Public */,
       6,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   84,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    1,   89,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    2,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   16,   17,

       0        // eod
};

void TCPMsgs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TCPMsgs *_t = static_cast<TCPMsgs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->msgFromTcpToWeb((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->msgFromTcpToArd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->msgFromTcpToCore((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->connected(); break;
        case 6: _t->disconnected(); break;
        case 7: _t->bytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->readyRead(); break;
        case 9: _t->TcpQueueMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TCPMsgs::*_t)(QString , LOG );
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
        {
            typedef void (TCPMsgs::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPMsgs::msgFromTcpToCore)) {
                *result = 3;
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void TCPMsgs::addTextToConsole(QString _t1, LOG _t2)
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

// SIGNAL 3
void TCPMsgs::msgFromTcpToCore(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
