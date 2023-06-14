/****************************************************************************
** Meta object code from reading C++ file 'ShipCreator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../gui/ShipCreator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ShipCreator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ShipInitForm_t {
    QByteArrayData data[9];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShipInitForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShipInitForm_t qt_meta_stringdata_ShipInitForm = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ShipInitForm"
QT_MOC_LITERAL(1, 13, 13), // "formSubmitted"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11), // "std::string"
QT_MOC_LITERAL(4, 40, 1), // "a"
QT_MOC_LITERAL(5, 42, 1), // "b"
QT_MOC_LITERAL(6, 44, 9), // "ShipList*"
QT_MOC_LITERAL(7, 54, 8), // "shipList"
QT_MOC_LITERAL(8, 63, 18) // "handleEnterClicked"

    },
    "ShipInitForm\0formSubmitted\0\0std::string\0"
    "a\0b\0ShipList*\0shipList\0handleEnterClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShipInitForm[] = {

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
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Double, 0x80000000 | 6,    4,    5,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ShipInitForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ShipInitForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->formSubmitted((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< ShipList*(*)>(_a[3]))); break;
        case 1: _t->handleEnterClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ShipList* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ShipInitForm::*)(std::string , double , ShipList * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ShipInitForm::formSubmitted)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ShipInitForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ShipInitForm.data,
    qt_meta_data_ShipInitForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ShipInitForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShipInitForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ShipInitForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ShipInitForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ShipInitForm::formSubmitted(std::string _t1, double _t2, ShipList * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
