#pragma once

%module(directors="1", package="gnomon.visualization", moduleimport="import _gnomonvisualization") gnomonvisualization

%include <gnomonCore/numpyWrapper.i>

%include "std_array.i"
%include "std_vector.i"
%include "carrays.i"
%array_class(double, doubleArray);

%include "std_shared_ptr.i"

/*
%shared_ptr(gnomonAbstractForm)
%shared_ptr(gnomonBinaryImage)
%shared_ptr(gnomonCellComplex)
%shared_ptr(gnomonCellGraph)
%shared_ptr(gnomonCellImage)
%shared_ptr(gnomonDataDict)
%shared_ptr(gnomonDataFrame)
%shared_ptr(gnomonImage)
%shared_ptr(gnomonLString)
%shared_ptr(gnomonMesh)
%shared_ptr(gnomonPointCloud)
%shared_ptr(gnomonTree)
%shared_ptr(gnomonSphereForm)
%shared_ptr(gnomonWallForm)

%shared_ptr(gnomonAbstractDynamicForm)
%shared_ptr(gnomonBinaryImageSeries)
%shared_ptr(gnomonCellComplexSeries)
%shared_ptr(gnomonCellGraphSeries)
%shared_ptr(gnomonCellImageSeries)
%shared_ptr(gnomonDataDictSeries)
%shared_ptr(gnomonDataFrameSeries)
%shared_ptr(gnomonImageSeries)
%shared_ptr(gnomonLStringSeries)
%shared_ptr(gnomonMeshSeries)
%shared_ptr(gnomonPointCloudSeries)
%shared_ptr(gnomonTreeSeries)
*/

%import <dtkBase/dtkBase.i>
%import <dtkCore/dtkCore.i>
%import <dtkImagingCore/dtkImagingCore.i>

%{
 
#include <dtkCore>
#include <dtkImagingCore>

#include <gnomonVisualization/gnomonActor/gnomonActor.h>
#include <gnomonVisualization/gnomonInteractorStyle/gnomonInteractorStyle.h>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
//#include <gnomonVisualization/gnomonView/gnomonViewManager.h>
#include <gnomonVisualization/gnomonView/gnomonViewForm.h>
#include <gnomonVisualization/gnomonView/gnomonViewMatplotlib.h>
#include <gnomonVisualization/gnomonView/gnomonViewData.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonBinaryImage/gnomonAbstractVisualizationBinaryImage.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonCellComplex/gnomonAbstractVisualizationCellComplex.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonDataFrame/gnomonAbstractMatplotlibVisualizationDataFrame.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractVisualizationLString.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h>
#include <gnomonVisualization/gnomonVisualizations/gnomonTree/gnomonAbstractMatplotlibVisualizationTree.h>

#include <gnomonVisualization/gnomonCoreParameterColor.h>
#include <gnomonVisualization/gnomonLookupTable.h>
#include <gnomonVisualization/gnomonCoreParameterLookupTable.h>
#include <gnomonVisualization/gnomonCoreParameterGraphical.h>

%}

%include <gnomonCore/gnomonCore.i>
%include <gnomonCore/gnomonForm.i>

%{
// VTK also includes a Py_hash_t typedef definition for Python 2 that clashes
// with SWIG's preprocessor macro
#if PY_VERSION_HEX < 0x3020000
#ifdef Py_hash_t
#undef Py_hash_t
#endif
#endif


// /////////////////////////////////////////////////////////////////
// VTK
// /////////////////////////////////////////////////////////////////

#include <vtkPythonUtil.h>

%}

%feature("autodoc","1");

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONVISUALIZATION_EXPORT
#define GNOMONVISUALIZATION_EXPORT
#undef  Q_INVOKABLE
#define Q_INVOKABLE

// /////////////////////////////////////////////////////////////////
// typemaps
// /////////////////////////////////////////////////////////////////


%typemap(out) vtkRenderer* {

    PyImport_ImportModule("vtk");

    $result = vtkPythonUtil::GetObjectFromPointer((vtkObjectBase*)$1);
}

%typemap(in) vtkRenderer* {

    $1 = (vtkRenderer *)vtkPythonUtil::GetPointerFromObject($input, "vtkRenderer");

    if ($1 == NULL) {
        SWIG_fail;
    }
}

%typemap(out) vtkRenderWindowInteractor* {

    PyImport_ImportModule("vtk");

    $result = vtkPythonUtil::GetObjectFromPointer((vtkObjectBase*)$1);
}

%typemap(in) vtkRenderWindowInteractor* {

    $1 = (vtkRenderWindowInteractor *)vtkPythonUtil::GetPointerFromObject($input, "vtkRenderWindowInteractor");

    if ($1 == NULL) {
        SWIG_fail;
    }
}

// /////////////////////////////////////////////////////////////////
// QImage <-> list of lists
// /////////////////////////////////////////////////////////////////

%typemap(in) QImage {
    if (PyList_Check($input)) {
        int rows = PyList_Size($input);
        if (PyList_Check(PyList_GET_ITEM($input, 0))) {
            int cols = PyList_Size(PyList_GET_ITEM($input, 0));
            $1 = new QImage(cols, rows, QImage::Format_RGB32);
            QRgb *rgbPtr = reinterpret_cast<QRgb *>($1->bits());
            for(int row=0; row<rows; ++row) {
                for(int col=0;col<cols; ++col) {
                    double r, g, b;
                    r = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 0));
                    g = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 1));
                    b = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 2));
                    *(rgbPtr) = QColor(r,g,b).rgb();
                    ++rgbPtr;
                }
            }
        } else {
            qDebug("PyList of integers is expected as input. Empty QImage is returned.");
            $1 = new QImage(600, 600, QImage::Format_RGB32);
        }
    } else {
        qDebug("PyList of integers is expected as input. Empty QImage is returned.");
        $1 = new QImage(600, 600, QImage::Format_RGB32);
    }
}

%typemap(in) const QImage& {
    if (PyList_Check($input)) {
        int rows = PyList_Size($input);
        if (rows > 0) {
            if (PyList_Check(PyList_GET_ITEM($input, 0))) {
                int cols = PyList_Size(PyList_GET_ITEM($input, 0));
                $1 = new QImage(cols, rows, QImage::Format_RGB32);
                QRgb *rgbPtr = reinterpret_cast<QRgb *>($1->bits());
                for(int row=0; row<rows; ++row) {
                    for(int col=0;col<cols; ++col) {
                        double r, g, b;
                        r = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 0));
                        g = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 1));
                        b = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM($input, row), col), 2));
                        *(rgbPtr) = QColor(r,g,b).rgb();
                        ++rgbPtr;
                    }
                }
            } else {
                qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
                 $1 = new QImage(600, 600, QImage::Format_RGB32);
            }
        } else {
            qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
            $1 = new QImage(600, 600, QImage::Format_RGB32);
        }
    } else {
        qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
        $1 = new QImage(600, 600, QImage::Format_RGB32);
    }
}

%typemap(freearg) const QImage {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QImage {
    PyObject *list = static_cast<PyObject *>($1);
    if (PyList_Check(list)) {
        int rows = PyList_Size(list);
        if (rows > 0) {
            if (PyList_Check(PyList_GET_ITEM(list, 0))) {
                int cols = PyList_Size(PyList_GET_ITEM(list, 0));
                $result = QImage(cols, rows, QImage::Format_RGB32);
                QRgb *rgbPtr = reinterpret_cast<QRgb *>($result.bits());
                for(int row=0; row<rows; ++row) {
                    for(int col=0;col<cols; ++col) {
                        double r, g, b;
                        r = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM(list, row), col), 0));
                        g = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM(list, row), col), 1));
                        b = PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(PyList_GET_ITEM(list, row), col), 2));
                        *(rgbPtr) = QColor(r,g,b).rgb();
                        ++rgbPtr;
                    }
                }
            } else {
                qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
                $result = QImage(600, 600, QImage::Format_RGB32);
            }
        } else {
            qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
            $result = QImage(600, 600, QImage::Format_RGB32);
        }
    } else {
        qDebug("PyList of PyList is expected as input. Empty QImage is returned.");
        $result = QImage(600, 600, QImage::Format_RGB32);
    }
}

%typemap(out) QImage {
    int rows = $1.width();
    int cols = $1.height();
    $result = PyList_New(rows);
    for(int row=0; row<rows; ++row) {
        PyObject *py_row = PyList_New(cols);
        for(int col=0;col<cols; ++col) {
            QColor color = $1.pixelColor(col,row);
            PyObject *py_color = PyList_New(3);
            PyList_SET_ITEM(py_color, 0, PyFloat_FromDouble(color.red()));
            PyList_SET_ITEM(py_color, 1, PyFloat_FromDouble(color.green()));
            PyList_SET_ITEM(py_color, 2, PyFloat_FromDouble(color.blue()));
            PyList_SET_ITEM(py_row, col, py_color);
        }
        PyList_SET_ITEM($result, row, py_row);
    }
}

%typemap(directorin) QImage {
    int rows = $1.width();
    int cols = $1.height();
    PyObject *list = PyList_New(rows);
    for(int row=0; row<rows; ++row) {
        PyObject *py_row = PyList_New(cols);
        for(int col=0;col<cols; ++col) {
            QColor color = $1.pixelColor(col,row);
            PyObject *py_color = PyList_New(3);
            PyList_SET_ITEM(py_color, 0, PyFloat_FromDouble(color.red()));
            PyList_SET_ITEM(py_color, 1, PyFloat_FromDouble(color.green()));
            PyList_SET_ITEM(py_color, 2, PyFloat_FromDouble(color.blue()));
            PyList_SET_ITEM(py_row, col, py_color);
        }
        PyList_SET_ITEM($result, row, py_row);
    }
    $input = list;
}


// /////////////////////////////////////////////////////////////////
// QColor <-> list
// /////////////////////////////////////////////////////////////////

%typemap(in) QColor {
    if (PyList_Check($input)) {
        int dims = PyList_Size($input);
        if (dims>=3) {
            double r, g, b;
            r = PyFloat_AsDouble(PyList_GET_ITEM($input, 0));
            g = PyFloat_AsDouble(PyList_GET_ITEM($input, 1));
            b = PyFloat_AsDouble(PyList_GET_ITEM($input, 2));
            $1 = new QColor(r,g,b);
        } else {
            qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
            $1 = new QColor(0, 0, 0);
        }
    } else {
        qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
        $1 = new QColor(0, 0, 0);
    }
}

%typemap(in) const QColor& {
    if (PyList_Check($input)) {
        int dims = PyList_Size($input);
        if (dims>=3) {
            double r, g, b;
            r = PyFloat_AsDouble(PyList_GET_ITEM($input, 0));
            g = PyFloat_AsDouble(PyList_GET_ITEM($input, 1));
            b = PyFloat_AsDouble(PyList_GET_ITEM($input, 2));
            $1 = new QColor(r,g,b);
        } else {
            qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
            $1 = new QColor(0, 0, 0);
        }
    } else {
        qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
        $1 = new QColor(0, 0, 0);
    }
}

%typemap(freearg) const QColor {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QColor {

    PyObject *list = static_cast<PyObject *>($1);
    if (PyList_Check(list)) {
        int dims = PyList_Size(list);
        if (dims>=3) {
            double r, g, b;
            r = PyFloat_AsDouble(PyList_GET_ITEM(list, 0));
            g = PyFloat_AsDouble(PyList_GET_ITEM(list, 1));
            b = PyFloat_AsDouble(PyList_GET_ITEM(list, 2));
            $result = QColor(r,g,b);
        } else {
            qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
            $result = QColor(0, 0, 0);
        }
    } else {
        qDebug("PyList of 3 integers is expected as input. Empty QColor is returned.");
        $result = QColor(0, 0, 0);
    }
}

%typemap(out) QColor {
    $result = PyList_New(3);
    PyList_SET_ITEM($result, 0, PyFloat_FromDouble($1.red()));
    PyList_SET_ITEM($result, 1, PyFloat_FromDouble($1.green()));
    PyList_SET_ITEM($result, 2, PyFloat_FromDouble($1.blue()));
}

%typemap(directorin) QColor {
    PyObject *list = PyList_New(3);
    PyList_SET_ITEM($result, 0, PyFloat_FromDouble($1.red()));
    PyList_SET_ITEM($result, 1, PyFloat_FromDouble($1.green()));
    PyList_SET_ITEM($result, 2, PyFloat_FromDouble($1.blue()));
    $input = list;
}

// /////////////////////////////////////////////////////////////////
// Map key (int) to strings
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<int, QString> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next($input, &pos, &key, &value)) {
            int k = PyLong_AsLong(key);
            QString v = QString(PyUnicode_AsUTF8(value));
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<int, QString> is returned.");
    }
}

%typemap(in) const QMap<QString, QString>& {
    if (PyDict_Check($input)) {
        $1 = new QMap<QString, QString>;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next($input, &pos, &key, &value)) {
            int k = PyLong_AsLong(key);
            QString v = QString(PyUnicode_AsUTF8(value));
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<int, QString> is returned.");
    }
}

%typemap(freearg) const QMap<int, QString>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<int, QString> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            int k = PyLong_AsLong(key);
            QString v = QString(PyUnicode_AsUTF8(value));
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<int, QString> is returned.");
    }
}


/* **************************************************************************
 *
 * ************************************************************************** */

%inline
%{

/* void setupMatplotlib(qlonglong view_address, int num) */
/* { */
/*     QWidget *widget = reinterpret_cast<QWidget *>(view_address); */

/*     foreach(QWidget *top, qApp->topLevelWidgets()) { */
/*         foreach(gnomonViewMatplotlib *view, top->findChildren<gnomonViewMatplotlib *>()) { */
/*             if(view->figureNumber() == num) */
/*             { */
/*                 view->addWidget(widget); */
/*             } */
/*         } */
/*     } */
/* } */

/* void addFormToFigure(gnomonAbstractDynamicForm * form, const QString& name, int figure_number) */
/* { */
/*     qDebug()<<Q_FUNC_INFO<<form<<figure_number; */
/*     foreach(QWidget *top, qApp->topLevelWidgets()) { */
/*         foreach(gnomonViewMatplotlib *view, top->findChildren<gnomonViewMatplotlib *>()) { */
/*             if(view->figureNumber() == figure_number) */
/*             { */
/*                 qDebug()<<Q_FUNC_INFO<<view<<"setForm"<<name; */
/*                 view->setForm(name,form); */
/*             } */
/*         } */
/*     } */
/* } */

/* gnomonAbstractDynamicForm *getFigureForm(const QString& name, int figure_number) */
/* { */
/*     foreach(QWidget *top, qApp->topLevelWidgets()) { */
/*         foreach(gnomonViewMatplotlib *view, top->findChildren<gnomonViewMatplotlib *>()) { */
/*             if(view->figureNumber() == figure_number) */
/*             { */
/*                 return view->form(name); */
/*             } */
/*         } */
/*     } */
/*     return nullptr; */
/* } */

%}

/*%pythoncode %{
    def _figure_func(self):
        from gnomon.utils.matplotlib_tools import gnomon_figure
        return gnomon_figure(self.figureNumber())

    setattr(gnomonViewMatplotlib, "figure", _figure_func)
%}*/

// ///////////////////////////////////////////////////////////////////
// Ignore rules
// ///////////////////////////////////////////////////////////////////

%ignore onInserted;

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
// Wrapper input
// /////////////////////////////////////////////////////////////////

WRAP_DTKCORE_PARAMETER_NO_TEMPLATE(gnomonCoreParameterColorMap, ParameterColorMap)
%include <gnomonVisualization/gnomonCoreParameterColor.h>

// %ignore dtkCoreParameterSimple<gnomonLookupTable>::__str__;
%include <gnomonVisualization/gnomonLookupTable.h>
WRAP_DTKCORE_PARAMETER_NO_TEMPLATE(gnomonCoreParameterColorMap, ParameterLookupTable)
%include <gnomonVisualization/gnomonCoreParameterLookupTable.h>

%include <gnomonVisualization/gnomonCoreParameterGraphical.h>
WRAP_DTKCORE_PARAMETER_NO_TEMPLATE(gnomonCoreParameterGraphical, ParameterGraphical)
//WRAP_DTKCORE_PARAMETER(dtkCoreParameterSimple<gnomonLookupTable>, ParameterLookupTable)

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%include <gnomonVisualization/gnomonActor/gnomonActor.h>
%include <gnomonVisualization/gnomonInteractorStyle/gnomonInteractorStyle.h>
%include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
// %include <gnomonVisualization/gnomonView/gnomonViewManager.h>
%include <gnomonVisualization/gnomonView/gnomonViewForm.h>
%include <gnomonVisualization/gnomonView/gnomonViewMatplotlib.h>
%include <gnomonVisualization/gnomonView/gnomonViewData.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonBinaryImage/gnomonAbstractVisualizationBinaryImage.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonCellComplex/gnomonAbstractVisualizationCellComplex.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonDataFrame/gnomonAbstractMatplotlibVisualizationDataFrame.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractVisualizationLString.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h>
%include <gnomonVisualization/gnomonVisualizations/gnomonTree/gnomonAbstractMatplotlibVisualizationTree.h>


%include <gnomonVisualization/gnomonCoreParameterColor.h>
%include <gnomonVisualization/gnomonLookupTable.h>
%include <gnomonVisualization/gnomonCoreParameterGraphical.h>

//
// gnomonVisualization.i.in ends here
