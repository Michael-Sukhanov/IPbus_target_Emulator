/****************************************************************************
** Meta object code from reading C++ file 'EventHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "EventHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EventHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CRU_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CRU_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CRU_t qt_meta_stringdata_CRU = {
    {
QT_MOC_LITERAL(0, 0, 3), // "CRU"
QT_MOC_LITERAL(1, 4, 9), // "Send_data"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 17) // "BC_ORBIT_checking"

    },
    "CRU\0Send_data\0\0BC_ORBIT_checking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CRU[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CRU::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CRU *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Send_data((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 1: _t->BC_ORBIT_checking(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CRU::*)(quint32 , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CRU::Send_data)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CRU::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CRU.data,
    qt_meta_data_CRU,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CRU::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CRU::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CRU.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CRU::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CRU::Send_data(quint32 _t1, quint16 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Laser_t {
    QByteArrayData data[3];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Laser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Laser_t qt_meta_stringdata_Laser = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Laser"
QT_MOC_LITERAL(1, 6, 10), // "LaserFlash"
QT_MOC_LITERAL(2, 17, 0) // ""

    },
    "Laser\0LaserFlash\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Laser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void Laser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Laser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LaserFlash(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Laser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Laser::LaserFlash)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Laser::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Laser.data,
    qt_meta_data_Laser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Laser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Laser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Laser.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Laser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Laser::LaserFlash()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_EventHandler_t {
    QByteArrayData data[22];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventHandler_t qt_meta_stringdata_EventHandler = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EventHandler"
QT_MOC_LITERAL(1, 13, 6), // "handle"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 7), // "address"
QT_MOC_LITERAL(4, 29, 11), // "handle_FIFO"
QT_MOC_LITERAL(5, 41, 11), // "ReadHandler"
QT_MOC_LITERAL(6, 53, 12), // "LaserHandler"
QT_MOC_LITERAL(7, 66, 18), // "FIFOCountersFiller"
QT_MOC_LITERAL(8, 85, 16), // "BC_ORBIT_MONITOR"
QT_MOC_LITERAL(9, 102, 9), // "CRU_ORBIT"
QT_MOC_LITERAL(10, 112, 6), // "CRU_BC"
QT_MOC_LITERAL(11, 119, 11), // "init_values"
QT_MOC_LITERAL(12, 131, 21), // "init_TCM_channel_mask"
QT_MOC_LITERAL(13, 153, 12), // "mask_changed"
QT_MOC_LITERAL(14, 166, 4), // "Side"
QT_MOC_LITERAL(15, 171, 2), // "sd"
QT_MOC_LITERAL(16, 174, 12), // "contains_bit"
QT_MOC_LITERAL(17, 187, 4), // "mask"
QT_MOC_LITERAL(18, 192, 3), // "pos"
QT_MOC_LITERAL(19, 196, 12), // "HDMIlinks_ok"
QT_MOC_LITERAL(20, 209, 18), // "write_to_avail_PMs"
QT_MOC_LITERAL(21, 228, 4) // "data"

    },
    "EventHandler\0handle\0\0address\0handle_FIFO\0"
    "ReadHandler\0LaserHandler\0FIFOCountersFiller\0"
    "BC_ORBIT_MONITOR\0CRU_ORBIT\0CRU_BC\0"
    "init_values\0init_TCM_channel_mask\0"
    "mask_changed\0Side\0sd\0contains_bit\0"
    "mask\0pos\0HDMIlinks_ok\0write_to_avail_PMs\0"
    "data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       4,    1,   77,    2, 0x0a /* Public */,
       5,    1,   80,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    2,   85,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x08 /* Private */,
      13,    1,   92,    2, 0x08 /* Private */,
      16,    2,   95,    2, 0x08 /* Private */,
      19,    1,  100,    2, 0x08 /* Private */,
      20,    2,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Bool, QMetaType::UInt, QMetaType::UChar,   17,   18,
    QMetaType::Bool, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,   21,    3,

       0        // eod
};

void EventHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EventHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handle((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 1: _t->handle_FIFO((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 2: _t->ReadHandler((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 3: _t->LaserHandler(); break;
        case 4: _t->FIFOCountersFiller(); break;
        case 5: _t->BC_ORBIT_MONITOR((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 6: _t->init_values(); break;
        case 7: _t->init_TCM_channel_mask(); break;
        case 8: _t->mask_changed((*reinterpret_cast< Side(*)>(_a[1]))); break;
        case 9: { bool _r = _t->contains_bit((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->HDMIlinks_ok((*reinterpret_cast< Side(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->write_to_avail_PMs((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EventHandler::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_EventHandler.data,
    qt_meta_data_EventHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EventHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int EventHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
