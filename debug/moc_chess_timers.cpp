/****************************************************************************
** Meta object code from reading C++ file 'chess_timers.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess/chess_timers.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess_timers.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChessTimers_t {
    QByteArrayData data[11];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChessTimers_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChessTimers_t qt_meta_stringdata_ChessTimers = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ChessTimers"
QT_MOC_LITERAL(1, 12, 17), // "setBoardDataLabel"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 16), // "BOARD_DATA_LABEL"
QT_MOC_LITERAL(4, 48, 12), // "timeOutStart"
QT_MOC_LITERAL(5, 61, 13), // "timeOutPlayer"
QT_MOC_LITERAL(6, 75, 11), // "PLAYER_TYPE"
QT_MOC_LITERAL(7, 87, 13), // "playerTimeOut"
QT_MOC_LITERAL(8, 101, 6), // "Player"
QT_MOC_LITERAL(9, 108, 16), // "updateTimeLabels"
QT_MOC_LITERAL(10, 125, 12) // "startTimeOut"

    },
    "ChessTimers\0setBoardDataLabel\0\0"
    "BOARD_DATA_LABEL\0timeOutStart\0"
    "timeOutPlayer\0PLAYER_TYPE\0playerTimeOut\0"
    "Player\0updateTimeLabels\0startTimeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChessTimers[] = {

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
       1,    2,   44,    2, 0x06 /* Public */,
       4,    0,   49,    2, 0x06 /* Public */,
       5,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   53,    2, 0x08 /* Private */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChessTimers::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChessTimers *_t = static_cast<ChessTimers *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setBoardDataLabel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 1: _t->timeOutStart(); break;
        case 2: _t->timeOutPlayer((*reinterpret_cast< PLAYER_TYPE(*)>(_a[1]))); break;
        case 3: _t->playerTimeOut((*reinterpret_cast< PLAYER_TYPE(*)>(_a[1]))); break;
        case 4: _t->updateTimeLabels(); break;
        case 5: _t->startTimeOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ChessTimers::*_t)(QString , BOARD_DATA_LABEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChessTimers::setBoardDataLabel)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ChessTimers::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChessTimers::timeOutStart)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ChessTimers::*_t)(PLAYER_TYPE );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChessTimers::timeOutPlayer)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChessTimers::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChessTimers.data,
      qt_meta_data_ChessTimers,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ChessTimers::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChessTimers::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChessTimers.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ChessTimers::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ChessTimers::setBoardDataLabel(QString _t1, BOARD_DATA_LABEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChessTimers::timeOutStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ChessTimers::timeOutPlayer(PLAYER_TYPE _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
