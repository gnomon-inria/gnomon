%module(directors="1", doctring="Wrapping of gnomonForm for python usage", package="gnomon.core", moduleimport="import _gnomonform") gnomonform

%include "std_shared_ptr.i"
%include <dtkBase/dtkBase.i>
%import  <dtkCore/dtkCore.i>

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


%{
#include <QtCore>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
#include <gnomonCore/gnomonForm/gnomonAbstractFormData.h>
#include <gnomonCore/gnomonForm/gnomonAbstractForm.h>
//#include <gnomonCore/gnomonForm/gnomonDynamicFormMetadata.h>
#include <gnomonCore/gnomonForm/gnomonSphereForm.h>
#include <gnomonCore/gnomonForm/gnomonTimeSeries.h>
#include <gnomonCore/gnomonForm/gnomonWallForm.h>

#include "gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonCore/gnomonForm/gnomonBinaryImage/gnomonAbstractBinaryImageData.h"
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonAbstractCellComplexData.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonAbstractCellGraphData.h>
#include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonCellGraph.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonAbstractCellImageData.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonAbstractDataFrameData.h>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame.h>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonAbstractDataDictData.h>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonAbstractImageData.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonAbstractLStringData.h>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonAbstractMeshData.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonAbstractPointCloudData.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonAbstractTreeData.h>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree.h>
%}

#undef  GNOMONCORE_EXPORT
#define GNOMONCORE_EXPORT

// /////////////////////////////////////////////////////////////////
// Form dictionary
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<QString, gnomonAbstractForm *> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractForm *>(s_v);
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractForm*> is returned.");
    }
}

%typemap(in) const QMap<QString, gnomonAbstractForm *>& {
    if (PyDict_Check($input)) {
        $1 = new QMap<QString, gnomonAbstractForm *>;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractForm *>(s_v);
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractForm*> is returned.");
    }
}

%typemap(freearg) const QMap<QString, gnomonAbstractForm *>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<QString, gnomonAbstractForm *> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractForm *>(s_v);
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractForm*> is returned.");
    }
}

%typemap(directorout) gnomonAbstractForm * {
    //only for at_impl;
    //coming from a shared_ptr and going to another shared_ptr
    gnomonAbstractForm *v;
    void *s_v = 0;
    int r = SWIG_ConvertPtr($1, &s_v, SWIGTYPE_p_gnomonAbstractForm, 0);
    if (SWIG_IsOK(r)) {
        v = reinterpret_cast<gnomonAbstractForm *>(s_v);
        $result = v->clone();
    } else {
        qWarning() << "dir out gnomonAbstractForm *  error at converting ptr : " << SWIG_ErrorType(SWIG_ArgError(r));
    }
}

// /////////////////////////////////////////////////////////////////
// Id -> Float dictionary
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<long, double> {
if (PyDict_Check($input)) {
PyObject *key, *value;
Py_ssize_t pos = 0;
int r;
while (PyDict_Next($input, &pos, &key, &value)) {
long k = PyInt_AsLong(key);
double v = PyFloat_AsDouble(value);
$1.insert(k, v);
}
} else {
qDebug("PyDict is expected as input. Empty QMap<long, double> is returned.");
}
}

%typemap(in) const QMap<long, double>& {
$1 = new QMap<long, double>;
if (PyDict_Check($input)) {
PyObject *key, *value;
Py_ssize_t pos = 0;
int r;
while (PyDict_Next($input, &pos, &key, &value)) {
long k = PyInt_AsLong(key);
double v = PyFloat_AsDouble(value);
$1->insert(k, v);
}
} else {
qDebug("PyDict is expected as input. Empty QMap<long, double> is returned.");
}
}

%typemap(freearg) const QMap<long, double>& {
if ($1) {
delete $1;
}
}

%typemap(directorout) QMap<long, double> {
PyObject *dict = static_cast<PyObject *>($1);
if (PyDict_Check(dict)) {
PyObject *key, *value;
Py_ssize_t pos = 0;
int r;
while (PyDict_Next(dict, &pos, &key, &value)) {
long k = PyInt_AsLong(key);
double v = PyFloat_AsDouble(value);
$result.insert(k, v);
}
} else {
qDebug("PyDict is expected as input. Empty QMap<long, double> is returned.");
}
}

%typemap(out) QMap<long, double> {
$result = PyDict_New();

QList<long> keys = $1.keys();
for (auto it = keys.begin(); it != keys.end(); ++it) {
PyDict_SetItem($result, PyLong_FromLong(*it), PyFloat_FromDouble($1[*it]));
}
}

%typemap(directorin) QMap<long, double> {
PyObject *dict = PyDict_New();

QList<long> keys = $1.keys();
for (auto it = keys.begin(); it != keys.end(); ++it) {
PyDict_SetItem($result, PyLong_FromLong(*it), PyFloat_FromDouble($1[*it]));
}
$input = dict;
}

// /////////////////////////////////////////////////////////////////
// Form series
// /////////////////////////////////////////////////////////////////

%define WRAP_GNOMONCORE_FORM_SERIES(form_name)
    %insert("header") %{
  //  %fragment("To## form_name## Series", "header") {
        std::shared_ptr<gnomon## form_name## Series> To## form_name## Series(PyObject *obj) {
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            int r;
            std::shared_ptr<gnomon## form_name## Series> series = std::make_shared<gnomon## form_name## Series>();
            while (PyDict_Next(obj, &pos, &key, &value)) {
                double t = PyFloat_AsDouble(key);
                std::shared_ptr<gnomon## form_name##> v;
                void *s_v = 0;
                int newmem = 0;
                r = SWIG_ConvertPtrAndOwn(value, &s_v, SWIGTYPE_p_std__shared_ptrT_gnomon## form_name##_t,  0 , &newmem);
                if (!SWIG_IsOK(r)) {
                    Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(r)), "in To## form_name## Series");
                }
                if (s_v) {
                    v = *(reinterpret_cast< std::shared_ptr< gnomon## form_name## > * >(s_v));
                    series->insert(t, v);
                    if (newmem & SWIG_CAST_NEW_MEMORY) delete reinterpret_cast< std::shared_ptr< gnomon## form_name##Series > * >(s_v);
                }
            }
            return series;
        }
    //}

    //%fragment("From## form_name## Series", "header") {
        PyObject *From## form_name## Series(std::shared_ptr<gnomon## form_name## Series> series) {
            if (series) {
                PyObject *dict = PyDict_New();
                QList<double> times = series->times();
                std::shared_ptr<gnomon## form_name##> *c;
                double t;
                PyObject *v;
                for (auto it = times.begin(); it != times.end(); ++it) {
                    t = *it;
                    c = new std::shared_ptr<  gnomon## form_name##>(series->at(t)) ;
                    v = SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p_std__shared_ptrT_gnomon## form_name##_t, SWIG_POINTER_OWN |  0 );
                    PyDict_SetItem(dict, PyFloat_FromDouble(t), v);
                }
                return dict;
            } else {
                Py_RETURN_NONE;
            }
        }
    //}
    %}

    // ,fragment="To## form_name## Series"
    %typemap(in) std::shared_ptr<gnomon## form_name## Series> {
        if (PyDict_Check($input)) {
            $1 = To## form_name## Series($input);
        } else {
            qDebug("typemap in PyDict is expected as input. Empty gnomon## form_name## Series is returned.");
            $1 = std::make_shared<gnomon## form_name## Series>();
        }
    }

    %typemap(directorout) std::shared_ptr<gnomon## form_name## Series> {
        PyObject *dict = static_cast<PyObject *>($1);
        if (PyDict_Check(dict)) {
            $result = To## form_name## Series(dict);
        } else {
            qDebug("typemap dirout PyDict is expected as input. Empty gnomon## form_name## Series is returned.");
            $result = std::make_shared<gnomon## form_name## Series>();
        }
    }

    //, fragment="From## form_name## Series"
    %typemap(out) std::shared_ptr<gnomon## form_name## Series> {
        $result = From## form_name## Series($1);
    }

    %typemap(directorin) std::shared_ptr<gnomon## form_name## Series> {
        $input = From## form_name## Series($1);
    }
%enddef

WRAP_GNOMONCORE_FORM_SERIES(BinaryImage)
WRAP_GNOMONCORE_FORM_SERIES(CellComplex)
WRAP_GNOMONCORE_FORM_SERIES(CellGraph)
WRAP_GNOMONCORE_FORM_SERIES(CellImage)
WRAP_GNOMONCORE_FORM_SERIES(DataDict)
WRAP_GNOMONCORE_FORM_SERIES(DataFrame)
WRAP_GNOMONCORE_FORM_SERIES(Image)
WRAP_GNOMONCORE_FORM_SERIES(LString)
WRAP_GNOMONCORE_FORM_SERIES(Mesh)
WRAP_GNOMONCORE_FORM_SERIES(PointCloud)
WRAP_GNOMONCORE_FORM_SERIES(Tree)
//WRAP_GNOMONCORE_FORM_SERIES(Sphere)
//WRAP_GNOMONCORE_FORM_SERIES(Wall)
 
// /////////////////////////////////////////////////////////////////

%extend QVariant {

    //TODO in dtk-script repo -> QVariant.i line 30
    void setValue(QString value) {
        $self->setValue(QVariant::fromValue(value));
    }


    void setValue(gnomonCellComplex *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomonCellComplex* tognomonCellComplex() const {
        return $self->value<gnomonCellComplex *>();
    }
   
    void setValue(gnomonCellImage *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomonCellImage* tognomonCellImage() const {
        return $self->value<gnomonCellImage *>();
    }

    void setValue(gnomonCellGraph *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomonCellGraph* tognomonCellGraph() const {
        return $self->value<gnomonCellGraph *>();
    }

    /* void setValue(gnomonDiscreteDynamicForm *value) {
        $self->setValue(dtkCoreMetaType::variantFromValue(value));
    }
    gnomonDiscreteDynamicForm* tognomonDiscreteDynamicForm() const {
        return $self->value<gnomonDiscreteDynamicForm *>();
    } */

    void setValue(gnomonSphereForm *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomonSphereForm* tognomonSphereForm() const {
        return $self->value<gnomonSphereForm *>();
    }

    void setValue(gnomonWallForm *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomonWallForm* tognomonWallForm() const {
        return $self->value<gnomonWallForm *>();
    }
 }


// /////////////////////////////////////////////////////////////////
// Form Series dictionary
// /////////////////////////////////////////////////////////////////


%typemap(directorout) QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            std::shared_ptr<gnomonAbstractDynamicForm> series;
            PyObject *k2, *v2;
            Py_ssize_t pos2 = 0;

            //use the first element to infer the type
            if(PyDict_Next(value, &pos2, &k2, &v2)) {
                std::shared_ptr<gnomonAbstractForm> v;
                void *s_v = 0;
                int newmem = 0;
                r = SWIG_ConvertPtrAndOwn(v2, &s_v, SWIGTYPE_p_std__shared_ptrT_gnomonAbstractForm_t,  0 , &newmem);
                if (!SWIG_IsOK(r)) {
                    Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(r)), "in cast to gnomonAbstractForm");
                }
                v = *(reinterpret_cast< std::shared_ptr< gnomonAbstractForm> * >(s_v));
                if(v->asBinaryImage()) {
                    series = ToBinaryImageSeries(value);
                } else if(v->asCellComplex()) {
                    series = ToCellComplexSeries(value);
                } else if(v->asCellGraph()) {
                    series = ToCellGraphSeries(value);
                } else if(v->asCellImage()) {
                    series = ToCellImageSeries(value);
                } else if(v->asDataDict()) {
                    series = ToDataDictSeries(value);
                } else if(v->asDataFrame()) {
                    series = ToDataFrameSeries(value);
                } else if(v->asImage()) {
                    series = ToImageSeries(value);
                } else if(v->asLString()) {
                    series = ToLStringSeries(value);
                } else if(v->asMesh()) {
                    series = ToMeshSeries(value);
                /*} else if(v->asPointCloud()) {
                    series = ToPointCloudSeries(value);
                } else if(v->asSphere()) {
                    series = ToSphereSeries(value);
                } else if(v->asTree()) {
                    series = ToTreeSeries(value);
                    */
                } else {
                    qWarning() << Q_FUNC_INFO << "Cannot cast to derived type: " << v.get();
                }
                qDebug() << "inserting "<< k << series->times() << series->formName();
                $result.insert(k, series);
            } else {
                qWarning() << Q_FUNC_INFO << "No abstractDynamicForm for k " << k;
            }
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> is returned.");
    }
}

%typemap(in) QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> {
    $1 = QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>>();
    PyObject *dict = static_cast<PyObject *>($input);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            std::shared_ptr<gnomonAbstractDynamicForm> series;
            PyObject *k2, *v2;
            Py_ssize_t pos2 = 0;

            //use the first element to infer the type
            if(PyDict_Next(value, &pos2, &k2, &v2)) {
                std::shared_ptr<gnomonAbstractForm> v;
                void *s_v = 0;
                int newmem = 0;
                r = SWIG_ConvertPtrAndOwn(v2, &s_v, SWIGTYPE_p_std__shared_ptrT_gnomonAbstractForm_t,  0 , &newmem);
                if (!SWIG_IsOK(r)) {
                    Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(r)), "in cast to gnomonAbstractForm");
                }
                v = *(reinterpret_cast< std::shared_ptr< gnomonAbstractForm> * >(s_v));
                if(v->asBinaryImage()) {
                    series = ToBinaryImageSeries(value);
                } else if(v->asCellComplex()) {
                    series = ToCellComplexSeries(value);
                } else if(v->asCellGraph()) {
                    series = ToCellGraphSeries(value);
                } else if(v->asCellImage()) {
                    series = ToCellImageSeries(value);
                } else if(v->asDataDict()) {
                    series = ToDataDictSeries(value);
                } else if(v->asDataFrame()) {
                    series = ToDataFrameSeries(value);
                } else if(v->asImage()) {
                    series = ToImageSeries(value);
                } else if(v->asLString()) {
                    series = ToLStringSeries(value);
                } else if(v->asMesh()) {
                    series = ToMeshSeries(value);
                /*} else if(v->asPointCloud()) {
                    series = ToPointCloudSeries(value);
                } else if(v->asSphere()) {
                    series = ToSphereSeries(value);
                } else if(v->asTree()) {
                    series = ToTreeSeries(value);
                    */
                } else {
                    qWarning() << Q_FUNC_INFO << "Cannot cast to derived type: " << v.get();
                }
                qDebug() << "inserting "<< k << series->times() << series->formName();
                $1.insert(k, series);
            } else {
                qWarning() << Q_FUNC_INFO << "No abstractDynamicForm for k " << k;
            }
        }
    } else {
        qWarning("PyDict is expected as input. Empty QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > is returned.");
    }
}

%typemap(out) QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> {
    $result = PyDict_New();
    PyObject *k;
    PyObject *v;
    auto it  = $1.cbegin();
    auto end = $1.cend();
    for(; it != end; ++it) {
        k =  PyUnicode_FromString(it.key().toUtf8().constData());
        std::shared_ptr<gnomonAbstractDynamicForm> series = it.value();

        if(series->formName() == "gnomonBinaryImage") {
            v = FromBinaryImageSeries(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(series));
        } else if(series->formName() == "gnomonCellComplex") {
            v = FromCellComplexSeries(std::dynamic_pointer_cast<gnomonCellComplexSeries>(series));
        } else if(series->formName() == "gnomonCellGraph") {
            v = FromCellGraphSeries(std::dynamic_pointer_cast<gnomonCellGraphSeries>(series));
        } else if(series->formName() == "gnomonCellImage") {
            v = FromCellImageSeries(std::dynamic_pointer_cast<gnomonCellImageSeries>(series));
        } else if(series->formName() == "gnomonDataDict") {
            v = FromDataDictSeries(std::dynamic_pointer_cast<gnomonDataDictSeries>(series));
        } else if(series->formName() == "gnomonDataFrame") {
            v = FromDataFrameSeries(std::dynamic_pointer_cast<gnomonDataFrameSeries>(series));
        } else if(series->formName() == "gnomonImage") {
            v = FromImageSeries(std::dynamic_pointer_cast<gnomonImageSeries>(series));
        } else if(series->formName() == "gnomonLString") {
            v = FromLStringSeries(std::dynamic_pointer_cast<gnomonLStringSeries>(series));
        }else if(series->formName() == "gnomonMesh") {
            v = FromMeshSeries(std::dynamic_pointer_cast<gnomonMeshSeries>(series));
        }else {
            qWarning() << Q_FUNC_INFO << "Not implemented for form: " << series->formName();
            v = PyDict_New();
        }

        PyDict_SetItem($result, k, v);
    }
}


 
// /////////////////////////////////////////////////////////////////
// String dictionary
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<QString, QString> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            QString v = QString(PyUnicode_AsUTF8(value));
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, QString> is returned.");
    }
}

%typemap(in) const QMap<QString, QString>& {
    $1 = new QMap<QString, QString>;
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            QString v = QString(PyUnicode_AsUTF8(value));
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, QString> is returned.");
    }
}

%typemap(freearg) const QMap<QString, QString>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<QString, QString> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            QString v = QString(PyUnicode_AsUTF8(value));
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, QString> is returned.");
    }
}


%include <QtCore/QVariant.i>
%include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
%include <gnomonCore/gnomonForm/gnomonAbstractForm.h>
%include <gnomonCore/gnomonForm/gnomonAbstractFormData.h>
//%include <gnomonCore/gnomonForm/gnomonDynamicFormMetadata.h>
%include <gnomonCore/gnomonForm/gnomonSphereForm.h>
%include <gnomonCore/gnomonForm/gnomonTimeSeries.h>
%include <gnomonCore/gnomonForm/gnomonWallForm.h>

%include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonAbstractBinaryImageData.h>
%include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
%extend gnomonBinaryImage {
        const char* __repr__()
        {
            static std::string s;
            auto&& image = $self;
            QString str("<gnomoncore.gnomonBinaryImage");
            str += QString(" at 0x%1>").arg((quintptr)image, 12, 16, QChar('0'));
            s = str.toStdString();
            return s.data();
        }
}

%include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonAbstractCellComplexData.h>
%include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
%extend gnomonCellComplex {
	const char* __repr__()
	{
        static std::string s;
        auto&& cellComplex = $self;
        QString str("<gnomoncore.gnomonCellComplex");
        str += QString(" with %1 cell(s)").arg(cellComplex->elementCount(3));
        str += QString(" at 0x%1>").arg((quintptr)cellComplex, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonAbstractCellGraphData.h>
%include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonCellGraph.h>
%extend gnomonCellGraph {
    const char* __repr__()
	{
        static std::string s;
        auto&& cellGraph = $self;
        QString str("<gnomoncore.gnomonCellGraph");
        //str += QString(" with %1 cell(s)").arg(cellComplex->elementCount(3));
        //str += QString(" at 0x%1>").arg((quintptr)cellComplex, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}
%include <gnomonCore/gnomonForm/gnomonCellImage/gnomonAbstractCellImageData.h>
%include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
%extend gnomonCellImage {
	const char* __repr__()
	{
        static std::string s;
        auto&& cellImage = $self;
        QString str("<gnomoncore.gnomonCellImage");
        str += QString(" with %1 cell(s)").arg(cellImage->cellCount());
        str += QString(" at 0x%1>").arg((quintptr)cellImage, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonDataDict/gnomonAbstractDataDictData.h>
%include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict.h>
%extend gnomonDataDict {
	const char* __repr__()
	{
        static std::string s;
        auto&& dataDict = $self;
        QString str("<gnomoncore.gnomonDataDict");
        str += QString(" with %1 key(s)").arg(dataDict->keys().size());
        str += QString(" at 0x%1>").arg((quintptr)dataDict, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonAbstractDataFrameData.h>
%include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame.h>
%extend gnomonDataFrame {
	const char* __repr__()
	{
        static std::string s;
        auto&& dataFrame = $self;
        QString str("<gnomoncore.gnomonDataFrame");
        str += QString(" with %1 line(s)").arg(dataFrame->index().size());
        str += QString(" at 0x%1>").arg((quintptr)dataFrame, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonImage/gnomonAbstractImageData.h>
%include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
%extend gnomonImage {
	const char* __repr__()
	{
        static std::string s;
        auto&& image = $self;
        QString str("<gnomoncore.gnomonImage");
        str += QString(" with %1 channels(s)").arg(image->channels().size());
        str += QString(" at 0x%1>").arg((quintptr)image, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonLString/gnomonAbstractLStringData.h>
%include <gnomonCore/gnomonForm/gnomonLString/gnomonLString.h>
%extend gnomonLString {
	const char* __repr__()
	{
        static std::string s;
        auto&& lString = $self;
        QString str("<gnomoncore.gnomonLString");
        str += QString(" with %1 module(s)").arg(lString->moduleCount());
        str += QString(" at 0x%1>").arg((quintptr)lString, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonMesh/gnomonAbstractMeshData.h>
%include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
%extend gnomonMesh {
	const char* __repr__()
	{
        static std::string s;
        auto&& mesh = $self;
        QString str("<gnomoncore.gnomonMesh");
        str += QString(" with %1 triangle(s) and %1 vertices").arg(mesh->triangleCount(), mesh->vertexCount());
        str += QString(" at 0x%1>").arg((quintptr)mesh, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonAbstractPointCloudData.h>
%include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
%extend gnomonPointCloud {
	const char* __repr__()
	{
        static std::string s;
        auto&& pointCloud = $self;
        QString str("<gnomoncore.gnomonPointCloud");
        str += QString(" with %1 point(s)").arg(pointCloud->pointCount());
        str += QString(" at 0x%1>").arg((quintptr)pointCloud, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

%include <gnomonCore/gnomonForm/gnomonTree/gnomonAbstractTreeData.h>
%include <gnomonCore/gnomonForm/gnomonTree/gnomonTree.h>
%extend gnomonTree {
	const char* __repr__()
	{
        static std::string s;
        auto&& tree = $self;
        QString str("<gnomoncore.gnomonTree");
        str += QString(" with %1 node(s)").arg(tree->vertexCount());
        str += QString(" at 0x%1>").arg((quintptr)tree, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
	}
}

// %template(gnomonCellComplexSeries) gnomonTimeSeries<gnomonCellComplex>;
// %template(gnomonCellGraphSeries) gnomonTimeSeries<gnomonCellGraph>;
// %template(gnomonCellImageSeries) gnomonTimeSeries<gnomonCellImage>;
// %template(gnomonDataFrameSeries) gnomonTimeSeries<gnomonDataFrame>;
// %template(gnomonImageSeries) gnomonTimeSeries<gnomonImage>;
// %template(gnomonLStringSeries) gnomonTimeSeries<gnomonLString>;
// %template(gnomonMeshSeries) gnomonTimeSeries<gnomonMesh>;
// %template(gnomonPointCloudSeries) gnomonTimeSeries<gnomonPointCloud>;
// %template(gnomonSphereSeries) gnomonTimeSeries<gnomonSphereForm>;
// %template(gnomonTreeSeries) gnomonTimeSeries<gnomonTree>;
