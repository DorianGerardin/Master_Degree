/****************************************************************************
** Meta object code from reading C++ file 'TextureDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../TextureDockWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextureDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureDockWidget_t {
    QByteArrayData data[14];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureDockWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureDockWidget_t qt_meta_stringdata_TextureDockWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TextureDockWidget"
QT_MOC_LITERAL(1, 18, 15), // "setMaxCutPlanes"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "_xMax"
QT_MOC_LITERAL(4, 41, 5), // "_yMax"
QT_MOC_LITERAL(5, 47, 5), // "_zMax"
QT_MOC_LITERAL(6, 53, 14), // "setIVisibility"
QT_MOC_LITERAL(7, 68, 1), // "i"
QT_MOC_LITERAL(8, 70, 10), // "selectIAll"
QT_MOC_LITERAL(9, 81, 11), // "discardIAll"
QT_MOC_LITERAL(10, 93, 14), // "setImageLabels"
QT_MOC_LITERAL(11, 108, 14), // "addImageLabels"
QT_MOC_LITERAL(12, 123, 30), // "std::map<unsigned char,QColor>"
QT_MOC_LITERAL(13, 154, 6) // "colors"

    },
    "TextureDockWidget\0setMaxCutPlanes\0\0"
    "_xMax\0_yMax\0_zMax\0setIVisibility\0i\0"
    "selectIAll\0discardIAll\0setImageLabels\0"
    "addImageLabels\0std::map<unsigned char,QColor>\0"
    "colors"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureDockWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x0a /* Public */,
       6,    1,   51,    2, 0x0a /* Public */,
       8,    0,   54,    2, 0x0a /* Public */,
       9,    0,   55,    2, 0x0a /* Public */,
      10,    0,   56,    2, 0x0a /* Public */,
      11,    1,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void TextureDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureDockWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setMaxCutPlanes((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setIVisibility((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->selectIAll(); break;
        case 3: _t->discardIAll(); break;
        case 4: _t->setImageLabels(); break;
        case 5: _t->addImageLabels((*reinterpret_cast< const std::map<unsigned char,QColor>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TextureDockWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QDockWidget::staticMetaObject>(),
    qt_meta_stringdata_TextureDockWidget.data,
    qt_meta_data_TextureDockWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TextureDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureDockWidget.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int TextureDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
