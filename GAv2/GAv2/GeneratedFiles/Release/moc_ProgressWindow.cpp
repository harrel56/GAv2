/****************************************************************************
** Meta object code from reading C++ file 'ProgressWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ProgressWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProgressWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ProgressWindow_t {
    QByteArrayData data[11];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProgressWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProgressWindow_t qt_meta_stringdata_ProgressWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ProgressWindow"
QT_MOC_LITERAL(1, 15, 6), // "cancel"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "setMaxValue"
QT_MOC_LITERAL(4, 35, 3), // "max"
QT_MOC_LITERAL(5, 39, 6), // "update"
QT_MOC_LITERAL(6, 46, 3), // "rep"
QT_MOC_LITERAL(7, 50, 3), // "gen"
QT_MOC_LITERAL(8, 54, 3), // "val"
QT_MOC_LITERAL(9, 58, 9), // "succeeded"
QT_MOC_LITERAL(10, 68, 6) // "failed"

    },
    "ProgressWindow\0cancel\0\0setMaxValue\0"
    "max\0update\0rep\0gen\0val\0succeeded\0"
    "failed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProgressWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   40,    2, 0x0a /* Public */,
       5,    3,   43,    2, 0x0a /* Public */,
       9,    0,   50,    2, 0x0a /* Public */,
      10,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProgressWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProgressWindow *_t = static_cast<ProgressWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cancel(); break;
        case 1: _t->setMaxValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->update((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->succeeded(); break;
        case 4: _t->failed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ProgressWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProgressWindow::cancel)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ProgressWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ProgressWindow.data,
      qt_meta_data_ProgressWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProgressWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProgressWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProgressWindow.stringdata0))
        return static_cast<void*>(const_cast< ProgressWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int ProgressWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ProgressWindow::cancel()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
