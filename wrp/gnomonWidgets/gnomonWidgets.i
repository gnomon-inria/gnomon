// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

%module gnomonwidgets

%include <dtkBase/dtkBase.i>

%import <dtkCore/dtkCore.i>
%import <dtkImagingCore/dtkImagingCore.i>
// %import <gnomonCore.i>

%{

#include <dtkCore>
#include <dtkWidgets>
#include <dtkImagingCore>
#include <gnomonCore>

#include <gnomonWidgets/gnomonColor/gnomonCoreParameterColor.h>
#include <gnomonWidgets/gnomonColor/gnomonLookupTable.h>

#include <gnomonWidgets/gnomonInterpreterJupyter.h>

%}

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONWIDGETS_EXPORT
#define GNOMONWIDGETS_EXPORT

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

%inline %{
void foo(PyObject *widget, PyObject *console)
{
    qDebug()<<"OhYeh";
    qDebug()<<(QWidget *)(widget);
    QWidget *w;
    int status = SWIG_ConvertPtr(widget, (void **) &w, SWIGTYPE_p_QWidget, SWIG_POINTER_EXCEPTION | SWIG_POINTER_DISOWN);
    qDebug()<<status;
    return;
    //qDebug()<<w;
    //qDebug()<<(QWidget *)(console);
    //qDebug()<<"OhNo";
}
%}


// /////////////////////////////////////////////////////////////////
// Typemaps
// /////////////////////////////////////////////////////////////////

// QVariantMap
/*
%typemap(in) QVariantMap {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QVariant v;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            if (PyUnicode_Check(value)) {
                v = QVariant::fromValue(QString(PyUnicode_AsUTF8(value)));
            } else if (PyBool_Check(value)) {
                bool b = (value == Py_True);
                v = QVariant::fromValue(b);
            } else if (PyLong_Check(value)) {
                v = QVariant::fromValue(PyLong_AsLong(value));
            } else if (PyFloat_Check(value)) {
                v = QVariant::fromValue(PyFloat_AsDouble(value));
            } else {
                qDebug("Value type is not handled. Empty QVariant is set.");
            }
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QVariantMap is returned.");
    }
}

%typemap(in) const QVariantMap& {
    if (PyDict_Check($input)) {
        $1 = new QVariantMap;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QVariant v;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            if (PyUnicode_Check(value)) {
                v = QVariant::fromValue(QString(PyUnicode_AsUTF8(value)));
            } else if (PyBool_Check(value)) {
                bool b = (value == Py_True);
                v = QVariant::fromValue(b);
            } else if (PyLong_Check(value)) {
                v = QVariant::fromValue(PyLong_AsLong(value));
            } else if (PyFloat_Check(value)) {
                v = QVariant::fromValue(PyFloat_AsDouble(value));
            } else {
                qDebug("Value type is not handled. Empty QVariant is set.");
            }
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QVariantMap is returned.");
    }
}

%typemap(freearg) const QVariantMap& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QVariantMap {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QVariant v;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            if (PyUnicode_Check(value)) {
                v = QVariant::fromValue(QString(PyUnicode_AsUTF8(value)));
            } else if (PyBool_Check(value)) {
                bool b = (value == Py_True);
                v = QVariant::fromValue(b);
            } else if (PyLong_Check(value) || PyLong_Check(value)) {
                v = QVariant::fromValue(PyLong_AsLong(value));
            } else if (PyFloat_Check(value)) {
                v = QVariant::fromValue(PyFloat_AsDouble(value));
            } else {
                qDebug("Value type is not handled. Empty QVariant is set.");
            }
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QVariantMap is returned.");
    }
}
*/
// /////////////////////////////////////////////////////////////////
// QMap of colors
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<double, QColor> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QColor v;
        while (PyDict_Next($input, &pos, &key, &value)) {
            double k = double(PyFloat_AsDouble(key));
            if (PyList_Check(value)) {
                int r, g, b;
                r = PyLong_AsLong(PyList_GET_ITEM(value, 0));
                g = PyLong_AsLong(PyList_GET_ITEM(value, 1));
                b = PyLong_AsLong(PyList_GET_ITEM(value, 2));
                v = QColor::fromRgb(r,g,b);
            } else {
                qDebug("Value type is not handled. Empty QColor is set.");
            }
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<double, QColor> is returned.");
    }
}

%typemap(in) const QMap<double, QColor>& {
    if (PyDict_Check($input)) {
        $1 = new QMap<double, QColor>;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QColor v;
        while (PyDict_Next($input, &pos, &key, &value)) {
            double k = double(PyFloat_AsDouble(key));
            if (PyList_Check(value)) {
                int r, g, b;
                r = PyLong_AsLong(PyList_GET_ITEM(value, 0));
                g = PyLong_AsLong(PyList_GET_ITEM(value, 1));
                b = PyLong_AsLong(PyList_GET_ITEM(value, 2));
                v = QColor::fromRgb(r,g,b);
            } else {
                qDebug("Value type is not handled. Empty QColor is set.");
            }
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<double, QColor> is returned.");
    }
}

%typemap(freearg) const QMap<double, QColor>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<double, QColor> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        QColor v;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            double k = double(PyFloat_AsDouble(key));
            if (PyList_Check(value)) {
                int r, g, b;
                r = PyLong_AsLong(PyList_GET_ITEM(value, 0));
                g = PyLong_AsLong(PyList_GET_ITEM(value, 1));
                b = PyLong_AsLong(PyList_GET_ITEM(value, 2));
                v = QColor::fromRgb(r,g,b);
            } else {
                qDebug("Value type is not handled. Empty QColor is set.");
            }
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<double, QColor> is returned.");
    }
}


%typemap(out) QMap<double, QColor> {
  $result = PyDict_New();
  QColor c;
  double k;

  QList<double> keys = $1.keys();
  for (auto it = keys.begin(); it != keys.end(); ++it) {
    k = *it;
    c = $1[k];

    PyObject *value = PyList_New(3);
    PyList_SET_ITEM(value, 0, PyLong_FromLong(c.red()));
    PyList_SET_ITEM(value, 1, PyLong_FromLong(c.green()));
    PyList_SET_ITEM(value, 2, PyLong_FromLong(c.blue()));
    PyDict_SetItem($result, PyFloat_FromDouble(k), value);
  }
}

%typemap(directorin) QMap<double, QColor> {
  PyObject *dict = PyDict_New();
  QColor c;
  double k;

  QList<double> keys = $1.keys();
  for (auto it = keys.begin(); it != keys.end(); ++it) {
    k = *it;
    c = $1[k];
    PyObject *value = PyList_New(3);
    PyList_SET_ITEM(value, 0, PyLong_FromLong(c.red()));
    PyList_SET_ITEM(value, 1, PyLong_FromLong(c.green()));
    PyList_SET_ITEM(value, 2, PyLong_FromLong(c.blue()));
    PyDict_SetItem($result, PyFloat_FromDouble(k), value);
  }
  $input = dict;
}


// /////////////////////////////////////////////////////////////////
// SIP/SWIG connection
// /////////////////////////////////////////////////////////////////

%inline
%{

void setupConsole(qlonglong console_address)
{
    QWidget *widget = reinterpret_cast<QWidget *>(console_address);

    foreach(QWidget *top, qApp->topLevelWidgets()) {
        foreach(gnomonInterpreterJupyter *interpreter, top->findChildren<gnomonInterpreterJupyter *>()) {
            qDebug() << Q_FUNC_INFO << "interp" << interpreter;
            interpreter->addWidget(widget);
        }
    }
}

%}

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

WRAP_DTKCORE_PARAMETER_NO_TEMPLATE(gnomonCoreParameterColorMap, ParameterColorMap)
%include <gnomonWidgets/gnomonColor/gnomonCoreParameterColor.h>

%ignore dtkCoreParameterSimple<gnomonLookupTable>::__str__;
%include <gnomonWidgets/gnomonColor/gnomonLookupTable.h>
WRAP_DTKCORE_PARAMETER(dtkCoreParameterSimple<gnomonLookupTable>, ParameterLookupTable)

%include <gnomonWidgets/gnomonInterpreterJupyter.h>

//
// gnomonWidgets.i.in ends here
