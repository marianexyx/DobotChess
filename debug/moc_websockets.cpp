/****************************************************************************
** Meta object code from reading C++ file 'websockets.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../websockets.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'websockets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Websockets_t {
    QByteArrayData data[11];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Websockets_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Websockets_t qt_meta_stringdata_Websockets = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Websockets"
QT_MOC_LITERAL(1, 11, 25), // "addTextToWebsocketConsole"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 7), // "QStrMsg"
QT_MOC_LITERAL(4, 46, 24), // "MsgFromWebsocketsToChess"
QT_MOC_LITERAL(5, 71, 21), // "QStrMsgFromWebsockets"
QT_MOC_LITERAL(6, 93, 27), // "MsgFromWebsocketsToWebtable"
QT_MOC_LITERAL(7, 121, 19), // "processWebsocketMsg"
QT_MOC_LITERAL(8, 141, 19), // "QS_WbstMsgToProcess"
QT_MOC_LITERAL(9, 161, 15), // "onNewConnection"
QT_MOC_LITERAL(10, 177, 18) // "socketDisconnected"

    },
    "Websockets\0addTextToWebsocketConsole\0"
    "\0QStrMsg\0MsgFromWebsocketsToChess\0"
    "QStrMsgFromWebsockets\0MsgFromWebsocketsToWebtable\0"
    "processWebsocketMsg\0QS_WbstMsgToProcess\0"
    "onNewConnection\0socketDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Websockets[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   53,    2, 0x0a /* Public */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Websockets::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Websockets *_t = static_cast<Websockets *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToWebsocketConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->MsgFromWebsocketsToChess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->MsgFromWebsocketsToWebtable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->processWebsocketMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onNewConnection(); break;
        case 5: _t->socketDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Websockets::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::addTextToWebsocketConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Websockets::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::MsgFromWebsocketsToChess)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Websockets::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::MsgFromWebsocketsToWebtable)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Websockets::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Websockets.data,
      qt_meta_data_Websockets,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Websockets::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Websockets::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Websockets.stringdata0))
        return static_cast<void*>(const_cast< Websockets*>(this));
    return QObject::qt_metacast(_clname);
}

int Websockets::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Websockets::addTextToWebsocketConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Websockets::MsgFromWebsocketsToChess(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Websockets::MsgFromWebsocketsToWebtable(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
