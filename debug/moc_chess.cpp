/****************************************************************************
** Meta object code from reading C++ file 'chess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Chess_t {
    QByteArrayData data[17];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Chess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Chess_t qt_meta_stringdata_Chess = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Chess"
QT_MOC_LITERAL(1, 6, 15), // "addTextToLogPTE"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "LOG"
QT_MOC_LITERAL(4, 27, 17), // "setBoardDataLabel"
QT_MOC_LITERAL(5, 45, 16), // "BOARD_DATA_LABEL"
QT_MOC_LITERAL(6, 62, 18), // "showLegalMovesInUI"
QT_MOC_LITERAL(7, 81, 20), // "showHistoryMovesInUI"
QT_MOC_LITERAL(8, 102, 22), // "checkMsgFromWebsockets"
QT_MOC_LITERAL(9, 125, 7), // "QStrMsg"
QT_MOC_LITERAL(10, 133, 7), // "Client&"
QT_MOC_LITERAL(11, 141, 6), // "client"
QT_MOC_LITERAL(12, 148, 19), // "checkMsgFromChenard"
QT_MOC_LITERAL(13, 168, 14), // "QStrTcpMsgType"
QT_MOC_LITERAL(14, 183, 14), // "QStrTcpRespond"
QT_MOC_LITERAL(15, 198, 15), // "checkMsgFromUsb"
QT_MOC_LITERAL(16, 214, 12) // "getTableData"

    },
    "Chess\0addTextToLogPTE\0\0LOG\0setBoardDataLabel\0"
    "BOARD_DATA_LABEL\0showLegalMovesInUI\0"
    "showHistoryMovesInUI\0checkMsgFromWebsockets\0"
    "QStrMsg\0Client&\0client\0checkMsgFromChenard\0"
    "QStrTcpMsgType\0QStrTcpRespond\0"
    "checkMsgFromUsb\0getTableData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       4,    2,   59,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,
       7,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   70,    2, 0x0a /* Public */,
      12,    2,   75,    2, 0x0a /* Public */,
      15,    1,   80,    2, 0x0a /* Public */,
      16,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5,    2,    2,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::QStringList,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10,    9,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::QString,

       0        // eod
};

void Chess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chess *_t = static_cast<Chess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToLogPTE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->setBoardDataLabel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 2: _t->showLegalMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: _t->showHistoryMovesInUI((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->checkMsgFromWebsockets((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Client(*)>(_a[2]))); break;
        case 5: _t->checkMsgFromChenard((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->checkMsgFromUsb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { QString _r = _t->getTableData();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Chess::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::addTextToLogPTE)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QString , BOARD_DATA_LABEL );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::setBoardDataLabel)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::showLegalMovesInUI)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::showHistoryMovesInUI)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Chess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chess.data,
      qt_meta_data_Chess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Chess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Chess.stringdata0))
        return static_cast<void*>(const_cast< Chess*>(this));
    return QObject::qt_metacast(_clname);
}

int Chess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Chess::addTextToLogPTE(QString _t1, LOG _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chess::setBoardDataLabel(QString _t1, BOARD_DATA_LABEL _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chess::showLegalMovesInUI(QStringList _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Chess::showHistoryMovesInUI(QStringList _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
