/****************************************************************************
** Meta object code from reading C++ file 'AdminWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../AdminWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdminWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AdminWidget_t {
    QByteArrayData data[10];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AdminWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AdminWidget_t qt_meta_stringdata_AdminWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AdminWidget"
QT_MOC_LITERAL(1, 12, 8), // "addMovie"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 15), // "addMovieToTable"
QT_MOC_LITERAL(4, 38, 5), // "Movie"
QT_MOC_LITERAL(5, 44, 5), // "movie"
QT_MOC_LITERAL(6, 50, 11), // "deleteMovie"
QT_MOC_LITERAL(7, 62, 11), // "updateMovie"
QT_MOC_LITERAL(8, 74, 27), // "displayCurrentSelectedMovie"
QT_MOC_LITERAL(9, 102, 11) // "displayPlot"

    },
    "AdminWidget\0addMovie\0\0addMovieToTable\0"
    "Movie\0movie\0deleteMovie\0updateMovie\0"
    "displayCurrentSelectedMovie\0displayPlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AdminWidget[] = {

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
       1,    0,   44,    2, 0x0a /* Public */,
       3,    1,   45,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,
       8,    0,   50,    2, 0x0a /* Public */,
       9,    0,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AdminWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdminWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addMovie(); break;
        case 1: _t->addMovieToTable((*reinterpret_cast< const Movie(*)>(_a[1]))); break;
        case 2: _t->deleteMovie(); break;
        case 3: _t->updateMovie(); break;
        case 4: _t->displayCurrentSelectedMovie(); break;
        case 5: _t->displayPlot(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AdminWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AdminWidget.data,
    qt_meta_data_AdminWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AdminWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AdminWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AdminWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
