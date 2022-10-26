/****************************************************************************
** Meta object code from reading C++ file 'TextureViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../TextureViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextureViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureViewer_t {
    QByteArrayData data[22];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureViewer_t qt_meta_stringdata_TextureViewer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TextureViewer"
QT_MOC_LITERAL(1, 14, 15), // "setMaxCutPlanes"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "_xMax"
QT_MOC_LITERAL(4, 37, 5), // "_yMax"
QT_MOC_LITERAL(5, 43, 5), // "_zMax"
QT_MOC_LITERAL(6, 49, 14), // "setImageLabels"
QT_MOC_LITERAL(7, 64, 7), // "setXCut"
QT_MOC_LITERAL(8, 72, 2), // "_x"
QT_MOC_LITERAL(9, 75, 7), // "setYCut"
QT_MOC_LITERAL(10, 83, 2), // "_y"
QT_MOC_LITERAL(11, 86, 7), // "setZCut"
QT_MOC_LITERAL(12, 94, 2), // "_z"
QT_MOC_LITERAL(13, 97, 10), // "invertXCut"
QT_MOC_LITERAL(14, 108, 10), // "invertYCut"
QT_MOC_LITERAL(15, 119, 10), // "invertZCut"
QT_MOC_LITERAL(16, 130, 14), // "setXCutDisplay"
QT_MOC_LITERAL(17, 145, 12), // "_xCutDisplay"
QT_MOC_LITERAL(18, 158, 14), // "setYCutDisplay"
QT_MOC_LITERAL(19, 173, 12), // "_yCutDisplay"
QT_MOC_LITERAL(20, 186, 14), // "setZCutDisplay"
QT_MOC_LITERAL(21, 201, 12) // "_zCutDisplay"

    },
    "TextureViewer\0setMaxCutPlanes\0\0_xMax\0"
    "_yMax\0_zMax\0setImageLabels\0setXCut\0"
    "_x\0setYCut\0_y\0setZCut\0_z\0invertXCut\0"
    "invertYCut\0invertZCut\0setXCutDisplay\0"
    "_xCutDisplay\0setYCutDisplay\0_yCutDisplay\0"
    "setZCutDisplay\0_zCutDisplay"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   69,    2, 0x06 /* Public */,
       6,    0,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   77,    2, 0x0a /* Public */,
       9,    1,   80,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,
      13,    0,   86,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x0a /* Public */,
      15,    0,   88,    2, 0x0a /* Public */,
      16,    1,   89,    2, 0x0a /* Public */,
      18,    1,   92,    2, 0x0a /* Public */,
      20,    1,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Bool,   21,

       0        // eod
};

void TextureViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureViewer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setMaxCutPlanes((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setImageLabels(); break;
        case 2: _t->setXCut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setYCut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setZCut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->invertXCut(); break;
        case 6: _t->invertYCut(); break;
        case 7: _t->invertZCut(); break;
        case 8: _t->setXCutDisplay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setYCutDisplay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->setZCutDisplay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TextureViewer::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureViewer::setMaxCutPlanes)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TextureViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureViewer::setImageLabels)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TextureViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QGLViewer::staticMetaObject>(),
    qt_meta_stringdata_TextureViewer.data,
    qt_meta_data_TextureViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TextureViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureViewer.stringdata0))
        return static_cast<void*>(this);
    return QGLViewer::qt_metacast(_clname);
}

int TextureViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TextureViewer::setMaxCutPlanes(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextureViewer::setImageLabels()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
