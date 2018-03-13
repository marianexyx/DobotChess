/****************************************************************************
** Meta object code from reading C++ file 'websockets.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../websockets.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'websockets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Websockets_t {
    QByteArrayData data[14];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Websockets_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Websockets_t qt_meta_stringdata_Websockets = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Websockets"
QT_MOC_LITERAL(1, 11, 15), // "addTextToLogPTE"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "LOG"
QT_MOC_LITERAL(4, 32, 24), // "msgFromWebsocketsToChess"
QT_MOC_LITERAL(5, 57, 7), // "QStrMsg"
QT_MOC_LITERAL(6, 65, 7), // "Client&"
QT_MOC_LITERAL(7, 73, 7), // "pClient"
QT_MOC_LITERAL(8, 81, 18), // "socketDisconnected"
QT_MOC_LITERAL(9, 100, 11), // "receivedMsg"
QT_MOC_LITERAL(10, 112, 15), // "onNewConnection"
QT_MOC_LITERAL(11, 128, 15), // "sendMsgToClient"
QT_MOC_LITERAL(12, 144, 7), // "Client*"
QT_MOC_LITERAL(13, 152, 19) // "sendMsgToAllClients"

    },
    "Websockets\0addTextToLogPTE\0\0LOG\0"
    "msgFromWebsocketsToChess\0QStrMsg\0"
    "Client&\0pClient\0socketDisconnected\0"
    "receivedMsg\0onNewConnection\0sendMsgToClient\0"
    "Client*\0sendMsgToAllClients"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Websockets[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       4,    2,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   59,    2, 0x08 /* Private */,
       9,    1,   60,    2, 0x0a /* Public */,
      10,    0,   63,    2, 0x0a /* Public */,
      11,    2,   64,    2, 0x0a /* Public */,
      13,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    5,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,    5,    7,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void Websockets::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Websockets *_t = static_cast<Websockets *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToLogPTE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->msgFromWebsocketsToChess((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Client(*)>(_a[2]))); break;
        case 2: _t->socketDisconnected(); break;
        case 3: _t->receivedMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onNewConnection(); break;
        case 5: _t->sendMsgToClient((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Client*(*)>(_a[2]))); break;
        case 6: _t->sendMsgToAllClients((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Websockets::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Websockets::addTextToLogPTE)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Websockets::*_t)(QString , Client & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Websockets::msgFromWebsocketsToChess)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Websockets::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Websockets.data,
      qt_meta_data_Websockets,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Websockets::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Websockets::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Websockets.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Websockets::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Websockets::addTextToLogPTE(QString _t1, LOG _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Websockets::msgFromWebsocketsToChess(QString _t1, Client & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
