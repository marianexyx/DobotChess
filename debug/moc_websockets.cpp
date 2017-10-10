/****************************************************************************
** Meta object code from reading C++ file 'websockets.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../websockets.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'websockets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Websockets_t {
    QByteArrayData data[17];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Websockets_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Websockets_t qt_meta_stringdata_Websockets = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Websockets"
QT_MOC_LITERAL(1, 11, 16), // "addTextToConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "LOG"
QT_MOC_LITERAL(4, 33, 24), // "MsgFromWebsocketsToChess"
QT_MOC_LITERAL(5, 58, 21), // "QStrMsgFromWebsockets"
QT_MOC_LITERAL(6, 80, 18), // "setBoardDataLabels"
QT_MOC_LITERAL(7, 99, 17), // "BOARD_DATA_LABELS"
QT_MOC_LITERAL(8, 117, 15), // "showClientsList"
QT_MOC_LITERAL(9, 133, 14), // "QList<Clients>"
QT_MOC_LITERAL(10, 148, 18), // "socketDisconnected"
QT_MOC_LITERAL(11, 167, 11), // "receivedMsg"
QT_MOC_LITERAL(12, 179, 19), // "QS_WbstMsgToProcess"
QT_MOC_LITERAL(13, 199, 15), // "onNewConnection"
QT_MOC_LITERAL(14, 215, 29), // "msgFromChessboardToWebsockets"
QT_MOC_LITERAL(15, 245, 9), // "QStrWsMsg"
QT_MOC_LITERAL(16, 255, 18) // "getTableDataAsJSON"

    },
    "Websockets\0addTextToConsole\0\0LOG\0"
    "MsgFromWebsocketsToChess\0QStrMsgFromWebsockets\0"
    "setBoardDataLabels\0BOARD_DATA_LABELS\0"
    "showClientsList\0QList<Clients>\0"
    "socketDisconnected\0receivedMsg\0"
    "QS_WbstMsgToProcess\0onNewConnection\0"
    "msgFromChessboardToWebsockets\0QStrWsMsg\0"
    "getTableDataAsJSON"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Websockets[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       4,    1,   64,    2, 0x06 /* Public */,
       6,    2,   67,    2, 0x06 /* Public */,
       8,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   75,    2, 0x08 /* Private */,
      11,    1,   76,    2, 0x0a /* Public */,
      13,    0,   79,    2, 0x0a /* Public */,
      14,    1,   80,    2, 0x0a /* Public */,
      16,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    2,    2,
    QMetaType::Void, 0x80000000 | 9,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::QString,

       0        // eod
};

void Websockets::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Websockets *_t = static_cast<Websockets *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->MsgFromWebsocketsToChess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setBoardDataLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABELS(*)>(_a[2]))); break;
        case 3: _t->showClientsList((*reinterpret_cast< QList<Clients>(*)>(_a[1]))); break;
        case 4: _t->socketDisconnected(); break;
        case 5: _t->receivedMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onNewConnection(); break;
        case 7: _t->msgFromChessboardToWebsockets((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: { QString _r = _t->getTableDataAsJSON();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Websockets::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::addTextToConsole)) {
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
            typedef void (Websockets::*_t)(QString , BOARD_DATA_LABELS );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::setBoardDataLabels)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Websockets::*_t)(QList<Clients> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Websockets::showClientsList)) {
                *result = 3;
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Websockets::addTextToConsole(QString _t1, LOG _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Websockets::MsgFromWebsocketsToChess(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Websockets::setBoardDataLabels(QString _t1, BOARD_DATA_LABELS _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Websockets::showClientsList(QList<Clients> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
