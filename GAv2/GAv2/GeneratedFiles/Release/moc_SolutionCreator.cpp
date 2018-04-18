/****************************************************************************
** Meta object code from reading C++ file 'SolutionCreator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SolutionCreator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SolutionCreator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SolutionPage_t {
    QByteArrayData data[4];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SolutionPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SolutionPage_t qt_meta_stringdata_SolutionPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SolutionPage"
QT_MOC_LITERAL(1, 13, 11), // "changeIndex"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12) // "solveClicked"

    },
    "SolutionPage\0changeIndex\0\0solveClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SolutionPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       3,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void SolutionPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SolutionPage *_t = static_cast<SolutionPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->solveClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SolutionPage::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SolutionPage::changeIndex)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SolutionPage::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SolutionPage::solveClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SolutionPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SolutionPage.data,
      qt_meta_data_SolutionPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SolutionPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SolutionPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SolutionPage.stringdata0))
        return static_cast<void*>(const_cast< SolutionPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int SolutionPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void SolutionPage::changeIndex(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SolutionPage::solveClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_ProblemPage_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProblemPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProblemPage_t qt_meta_stringdata_ProblemPage = {
    {
QT_MOC_LITERAL(0, 0, 11) // "ProblemPage"

    },
    "ProblemPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProblemPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ProblemPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ProblemPage::staticMetaObject = {
    { &SolutionPage::staticMetaObject, qt_meta_stringdata_ProblemPage.data,
      qt_meta_data_ProblemPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProblemPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProblemPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProblemPage.stringdata0))
        return static_cast<void*>(const_cast< ProblemPage*>(this));
    return SolutionPage::qt_metacast(_clname);
}

int ProblemPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SolutionPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_BasicPage_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BasicPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BasicPage_t qt_meta_stringdata_BasicPage = {
    {
QT_MOC_LITERAL(0, 0, 9) // "BasicPage"

    },
    "BasicPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BasicPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void BasicPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject BasicPage::staticMetaObject = {
    { &SolutionPage::staticMetaObject, qt_meta_stringdata_BasicPage.data,
      qt_meta_data_BasicPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BasicPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BasicPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BasicPage.stringdata0))
        return static_cast<void*>(const_cast< BasicPage*>(this));
    return SolutionPage::qt_metacast(_clname);
}

int BasicPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SolutionPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_SelectionPage_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectionPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectionPage_t qt_meta_stringdata_SelectionPage = {
    {
QT_MOC_LITERAL(0, 0, 13) // "SelectionPage"

    },
    "SelectionPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectionPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SelectionPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SelectionPage::staticMetaObject = {
    { &SolutionPage::staticMetaObject, qt_meta_stringdata_SelectionPage.data,
      qt_meta_data_SelectionPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SelectionPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectionPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SelectionPage.stringdata0))
        return static_cast<void*>(const_cast< SelectionPage*>(this));
    return SolutionPage::qt_metacast(_clname);
}

int SelectionPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SolutionPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_SolutionCreator_t {
    QByteArrayData data[3];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SolutionCreator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SolutionCreator_t qt_meta_stringdata_SolutionCreator = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SolutionCreator"
QT_MOC_LITERAL(1, 16, 7), // "onClose"
QT_MOC_LITERAL(2, 24, 0) // ""

    },
    "SolutionCreator\0onClose\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SolutionCreator[] = {

 // content:
       7,       // revision
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

void SolutionCreator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SolutionCreator *_t = static_cast<SolutionCreator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onClose(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SolutionCreator::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SolutionCreator::onClose)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SolutionCreator::staticMetaObject = {
    { &QStackedWidget::staticMetaObject, qt_meta_stringdata_SolutionCreator.data,
      qt_meta_data_SolutionCreator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SolutionCreator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SolutionCreator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SolutionCreator.stringdata0))
        return static_cast<void*>(const_cast< SolutionCreator*>(this));
    return QStackedWidget::qt_metacast(_clname);
}

int SolutionCreator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStackedWidget::qt_metacall(_c, _id, _a);
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
void SolutionCreator::onClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
