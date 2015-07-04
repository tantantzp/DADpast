/****************************************************************************
** Meta object code from reading C++ file 'dadpast.h'
**
** Created: Wed May 14 21:49:25 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dadpast.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dadpast.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DADpast[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      19,    8,    8,    8, 0x0a,
      45,   40,   32,    8, 0x0a,
      74,   70,   62,    8, 0x0a,
     119,  112,  102,    8, 0x0a,
     145,    8,    8,    8, 0x0a,
     158,    8,    8,    8, 0x0a,
     169,    8,    8,    8, 0x0a,
     180,    8,    8,    8, 0x0a,
     192,  190,    8,    8, 0x0a,
     211,  190,    8,    8, 0x0a,
     231,  190,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DADpast[] = {
    "DADpast\0\0readSrc()\0readTarget()\0QString\0"
    "info\0getFile(QString)\0QImage*\0img\0"
    "IplImagetoQImage(IplImage*)\0IplImage*\0"
    "qImage\0QImagetoIplImage(QImage*)\0"
    "ImageBlend()\0initDraw()\0DragDrop()\0"
    "showImg()\0p\0mouseMove(QPoint&)\0"
    "mousePress(QPoint&)\0mouseRelease(QPoint&)\0"
};

void DADpast::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DADpast *_t = static_cast<DADpast *>(_o);
        switch (_id) {
        case 0: _t->readSrc(); break;
        case 1: _t->readTarget(); break;
        case 2: { QString _r = _t->getFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QImage* _r = _t->IplImagetoQImage((*reinterpret_cast< IplImage*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QImage**>(_a[0]) = _r; }  break;
        case 4: { IplImage* _r = _t->QImagetoIplImage((*reinterpret_cast< QImage*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< IplImage**>(_a[0]) = _r; }  break;
        case 5: _t->ImageBlend(); break;
        case 6: _t->initDraw(); break;
        case 7: _t->DragDrop(); break;
        case 8: _t->showImg(); break;
        case 9: _t->mouseMove((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 10: _t->mousePress((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 11: _t->mouseRelease((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DADpast::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DADpast::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DADpast,
      qt_meta_data_DADpast, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DADpast::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DADpast::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DADpast::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DADpast))
        return static_cast<void*>(const_cast< DADpast*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DADpast::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
