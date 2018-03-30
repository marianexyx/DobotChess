/****************************************************************************
** Meta object code from reading C++ file 'chess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Chess_t {
    QByteArrayData data[28];
    char stringdata0[388];
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
QT_MOC_LITERAL(10, 133, 7), // "int64_t"
QT_MOC_LITERAL(11, 141, 11), // "n64SenderID"
QT_MOC_LITERAL(12, 153, 19), // "checkMsgFromChenard"
QT_MOC_LITERAL(13, 173, 14), // "QStrTcpMsgType"
QT_MOC_LITERAL(14, 188, 14), // "QStrTcpRespond"
QT_MOC_LITERAL(15, 203, 15), // "checkMsgFromUsb"
QT_MOC_LITERAL(16, 219, 12), // "getTableData"
QT_MOC_LITERAL(17, 232, 21), // "setBoardDataLabelInUI"
QT_MOC_LITERAL(18, 254, 9), // "QStrLabel"
QT_MOC_LITERAL(19, 264, 9), // "LabelType"
QT_MOC_LITERAL(20, 274, 20), // "showLegalMovesInForm"
QT_MOC_LITERAL(21, 295, 10), // "legalMoves"
QT_MOC_LITERAL(22, 306, 22), // "showHistoryMovesInForm"
QT_MOC_LITERAL(23, 329, 12), // "historyMoves"
QT_MOC_LITERAL(24, 342, 12), // "timeOutStart"
QT_MOC_LITERAL(25, 355, 13), // "timeOutPlayer"
QT_MOC_LITERAL(26, 369, 11), // "PLAYER_TYPE"
QT_MOC_LITERAL(27, 381, 6) // "Player"

    },
    "Chess\0addTextToLogPTE\0\0LOG\0setBoardDataLabel\0"
    "BOARD_DATA_LABEL\0showLegalMovesInUI\0"
    "showHistoryMovesInUI\0checkMsgFromWebsockets\0"
    "QStrMsg\0int64_t\0n64SenderID\0"
    "checkMsgFromChenard\0QStrTcpMsgType\0"
    "QStrTcpRespond\0checkMsgFromUsb\0"
    "getTableData\0setBoardDataLabelInUI\0"
    "QStrLabel\0LabelType\0showLegalMovesInForm\0"
    "legalMoves\0showHistoryMovesInForm\0"
    "historyMoves\0timeOutStart\0timeOutPlayer\0"
    "PLAYER_TYPE\0Player"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       4,    2,   84,    2, 0x06 /* Public */,
       6,    1,   89,    2, 0x06 /* Public */,
       7,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   95,    2, 0x0a /* Public */,
      12,    2,  100,    2, 0x0a /* Public */,
      15,    1,  105,    2, 0x0a /* Public */,
      16,    0,  108,    2, 0x0a /* Public */,
      17,    2,  109,    2, 0x0a /* Public */,
      20,    1,  114,    2, 0x0a /* Public */,
      22,    1,  117,    2, 0x0a /* Public */,
      24,    0,  120,    2, 0x0a /* Public */,
      25,    1,  121,    2, 0x0a /* Public */,

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
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5,   18,   19,
    QMetaType::Void, QMetaType::QStringList,   21,
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 26,   27,

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
        case 4: _t->checkMsgFromWebsockets((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int64_t(*)>(_a[2]))); break;
        case 5: _t->checkMsgFromChenard((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->checkMsgFromUsb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { QString _r = _t->getTableData();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->setBoardDataLabelInUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 9: _t->showLegalMovesInForm((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->showHistoryMovesInForm((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->timeOutStart(); break;
        case 12: _t->timeOutPlayer((*reinterpret_cast< PLAYER_TYPE(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Chess::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chess::addTextToLogPTE)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QString , BOARD_DATA_LABEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chess::setBoardDataLabel)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chess::showLegalMovesInUI)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chess::showHistoryMovesInUI)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Chess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chess.data,
      qt_meta_data_Chess,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Chess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chess::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Chess.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Chess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Chess::addTextToLogPTE(QString _t1, LOG _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chess::setBoardDataLabel(QString _t1, BOARD_DATA_LABEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chess::showLegalMovesInUI(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Chess::showHistoryMovesInUI(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
