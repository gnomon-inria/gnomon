%module(directors="1", doctring="Wrapping of gnomonForm for python usage", package="gnomon.core", moduleimport="import _gnomonform") gnomonform

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
gnomonForm::registerForms();
%}

%include "std_array.i"
%include "std_vector.i"

%include "std_shared_ptr.i"
%include <dtkBase/dtkBase.i>
%include <gnomonMacro.i>
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

#include <gnomonCore/gnomonPluginFactory.h>
#include <gnomonCore/gnomonCorePlugin.h>

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
#include <gnomonCore/gnomonForm/gnomonAbstractFormData.h>
#include <gnomonCore/gnomonForm/gnomonAbstractForm.h>
//#include <gnomonCore/gnomonForm/gnomonDynamicFormMetadata.h>
#include <gnomonCore/gnomonForm/gnomonSphereForm.h>
#include <gnomonCore/gnomonForm/gnomonTimeSeries.h>
#include <gnomonCore/gnomonForm/gnomonWallForm.h>

#include "gnomonCore/gnomonForm/gnomonDynamicFormFactory.h"
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
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMeshAttribute.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonAbstractPointCloudData.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonAbstractTreeData.h>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree.h>

#include <vtkPythonUtil.h>
#include <vtkImageData.h>
%}

#undef  GNOMONCORE_EXPORT
#define GNOMONCORE_EXPORT


// VTK
%typemap(out) vtkImageData* {

    PyImport_ImportModule("vtk");

    $result = vtkPythonUtil::GetObjectFromPointer ( (vtkImageData*)$1 );
 }

%typemap(directorin) vtkImageData* {

    PyImport_ImportModule("vtk");

    $input = vtkPythonUtil::GetObjectFromPointer ( (vtkImageData*)$1 );
 }

%typemap(in) vtkImageData* {

    $1 = (vtkImageData*) vtkPythonUtil::GetPointerFromObject ( $input, "vtkImageData" );
    //$1->Register(NULL);
    if ( $1 == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
        PyErr_Print();
    }
}

%typemap(directorout) vtkImageData* {
    $result = (vtkImageData*) vtkPythonUtil::GetPointerFromObject ( $1, "vtkImageData" );
    if ( $result == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
    }
}

// /////////////////////////////////////////////////////////////////
// std::vector<double>
// /////////////////////////////////////////////////////////////////


%typemap(in) std::vector<double> {
    if (PyList_Check($input)) {
        int size = PyList_Size($input);
        $1.reserve(size);
        for (int i=0; i<size; ++i) {
            PyObject *py_val = PyList_GET_ITEM($input, i);
            double val = PyFloat_AsDouble(py_val);
            $1.push_back(val);
        }
    } else {
        qDebug("PyList is expected as input. std::vector<double> is returned.");
    }
}

%typemap(directorout) std::vector<double> {
    PyObject *list = static_cast<PyObject *>($1);
    if (PyList_Check(list)) {
        int size = PyList_Size(list);
        $result.reserve(size);
        for (int i=0; i<size; ++i) {
            PyObject *py_val = PyList_GET_ITEM(list, i);
            double val = PyFloat_AsDouble(py_val);
            $result.push_back(val);
        }
    } else {
        qDebug("PyList is expected as input. Empty std::vector<double> is returned.");
    }
}

%typemap(out) std::vector<double> {
    int size = $1.size();
    $result = PyList_New(size);
    for(int i=0; i<size; ++i) {
        double val = $1.at(i);
        PyObject *py_val = PyFloat_FromDouble(val);
        PyList_SET_ITEM($result, i, py_val);
    }
}

%typemap(directorin) std::vector<double> {
    int size = $1.size();
    PyObject *list = PyList_New(size);
    for(int i=0; i<size; ++i) {
        double val = $1.at(i);
        PyObject *py_val = PyFloat_FromDouble(val);
        PyList_SET_ITEM(list, i, py_val);
    }
    $input = list;
}

// /////////////////////////////////////////////////////////////////
// std::vector<long long>
// /////////////////////////////////////////////////////////////////


%typemap(in) std::vector<long long> {
    if (PyList_Check($input)) {
        int size = PyList_Size($input);
        $1.reserve(size);
        for (int i=0; i<size; ++i) {
            PyObject *py_val = PyList_GET_ITEM($input, i);
            long long val = PyLong_AsLongLong(py_val);
            $1.push_back(val);
        }
    } else {
        qDebug("PyList is expected as input. std::vector<long long> is returned.");
    }
}

%typemap(directorout) std::vector<long long> {
    PyObject *list = static_cast<PyObject *>($1);
    if (PyList_Check(list)) {
        int size = PyList_Size(list);
        $result.reserve(size);
        for (int i=0; i<size; ++i) {
            PyObject *py_val = PyList_GET_ITEM(list, i);
            long long val = PyLong_AsLongLong(py_val);
            $result.push_back(val);
        }
    } else {
        qDebug("PyList is expected as input. Empty std::vector<long long> is returned.");
    }
}

%typemap(out) std::vector<long long> {
    int size = $1.size();
    $result = PyList_New(size);
    for(int i=0; i<size; ++i) {
        long long val = $1.at(i);
        PyObject *py_val = PyLong_FromLongLong(val);
        PyList_SET_ITEM($result, i, py_val);
    }
}

%typemap(directorin) std::vector<long long> {
    int size = $1.size();
    PyObject *list = PyList_New(size);
    for(int i=0; i<size; ++i) {
        long long val = $1.at(i);
        PyObject *py_val = PyLong_FromLongLong(val);
        PyList_SET_ITEM(list, i, py_val);
    }
    $input = list;
}

// /////////////////////////////////////////////////////////////////
// Mesh Attributes
// /////////////////////////////////////////////////////////////////

//python to c++
%typemap(in) gnomonMeshAttribute * {
    if (PyDict_Check($input)) {
        QString name;
        gnomonMeshAttribute::KindType kind;
        gnomonMeshAttribute::SupportType support;
        std::vector<int> data_int;
        std::vector<double> data_double;

        PyObject *py_name = PyDict_GetItemString($input, "name");
        if(py_name)
            name = PyUnicode_AsUTF8(py_name);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Name";
        }

        PyObject *py_kind = PyDict_GetItemString($input, "kind");
        if(py_kind)
            kind = (gnomonMeshAttribute::KindType)PyLong_AsLong(py_kind);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Kind";
        }

        PyObject *py_support = PyDict_GetItemString($input, "support");
        if(py_support)
            support = (gnomonMeshAttribute::SupportType)PyLong_AsLong(py_support);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Support";
        }

        PyObject *py_array = PyDict_GetItemString($input, "data");
        if(py_array && PyArray_Check(py_array)) {
            npy_intp array_size = PyArray_DIM((PyArrayObject*)py_array, 0);

            //May be optimized by setting the data pointer directly
            // is necessary

            int type = PyArray_TYPE((PyArrayObject*)py_array);
            if(type == NPY_INT) { // (NPY_INT || NPY_UINT || NPY_LONG || NPY_ULONG )) {
                data_int.reserve(array_size);
                int *py_arr_data = (int *)PyArray_DATA((PyArrayObject*)py_array);
                for(int i=0; i<array_size; ++i) {
                    data_int.push_back(py_arr_data[i]);
                }
                $1 = new gnomonMeshAttribute(name, kind, support, data_int);
            } else if (type == NPY_DOUBLE) {
                data_double.reserve(array_size);
                double *py_arr_data = (double *)PyArray_DATA((PyArrayObject*)py_array);
                for(int i=0; i<array_size; ++i) {
                    data_double.push_back(py_arr_data[i]);
                }
                $1 = new gnomonMeshAttribute(name, kind, support, data_double);
            } else {
                qWarning() << "gnomonMeshAttribute type not supported (only Int or Double)" << type;
            }
        } else {
            qWarning() << "gnomonMeshAttribute wrapper: dict 'data' is not an np array";
        }

    } else {
        qDebug("gnomonMeshAttribute wrapper: PyDict is expected as input. nullptr is returned.");
    }
}

%typemap(directorout) gnomonMeshAttribute * {
    PyObject *attr = static_cast<PyObject *>($1);
    if (PyDict_Check(attr)) {
        QString name;
        gnomonMeshAttribute::KindType kind;
        gnomonMeshAttribute::SupportType support;
        std::vector<int> data_int;
        std::vector<double> data_double;

        PyObject *py_name = PyDict_GetItemString(attr, "name");
        if(py_name)
            name = PyUnicode_AsUTF8(py_name);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Name";
        }

        PyObject *py_kind = PyDict_GetItemString(attr, "kind");
        if(py_kind)
            kind = (gnomonMeshAttribute::KindType)PyLong_AsLong(py_kind);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Kind";
        }

        PyObject *py_support = PyDict_GetItemString(attr, "support");
        if(py_support)
            support = (gnomonMeshAttribute::SupportType)PyLong_AsLong(py_support);
        else {
            qWarning() << "gnomonMeshAttribute wrapper: dict doesn't have Support";
        }

        PyObject *py_array = PyDict_GetItemString(attr, "data");
        if(py_array && PyArray_Check(py_array)) {
            npy_intp array_size = PyArray_DIM((PyArrayObject*)py_array, 0);

            //May be optimized by setting the data pointer directly
            // is necessary
            int type = PyArray_TYPE((PyArrayObject*)py_array);
            if(type == NPY_INT) { // (NPY_INT || NPY_UINT || NPY_LONG || NPY_ULONG )) {
                data_int.reserve(array_size);
                int *py_arr_data = (int *)PyArray_DATA((PyArrayObject*)py_array);
                for(int i=0; i<array_size; ++i) {
                    data_int.push_back(py_arr_data[i]);
                }
                $result = new gnomonMeshAttribute(name, kind, support, data_int);
            } else if (type == NPY_DOUBLE) {
                data_double.reserve(array_size);
                double *py_arr_data = (double *)PyArray_DATA((PyArrayObject*)py_array);
                for(int i=0; i<array_size; ++i) {
                    data_double.push_back(py_arr_data[i]);
                }
                $result = new gnomonMeshAttribute(name, kind, support, data_double);
            } else {
                qWarning() << "gnomonMeshAttribute type not supported (only Int or Double)" << type;
            }
        } else {
            qWarning() << "gnomonMeshAttribute wrapper: dict 'data' is not an np array";
        }
    } else {
        qDebug("gnomonMeshAttribute wrapper: PyDict is expected as input. nullptr is returned.");
    }
}

%typemap(freearg) gnomonMeshAttribute * {
    if($1) {
        delete $1;
    }
 }

//c++ to python
%typemap(out) gnomonMeshAttribute * {
    $result = PyDict_New();
    if ($1) {
        PyDict_SetItem($result, PyUnicode_FromString("name"), PyUnicode_FromString($1->m_name.toStdString().c_str()));
        PyDict_SetItem($result, PyUnicode_FromString("kind"), PyLong_FromLong($1->m_kind));
        PyDict_SetItem($result, PyUnicode_FromString("support"), PyLong_FromLong($1->m_support));

        npy_intp dims[1] = {0};
        PyObject *array;
        if(std::holds_alternative<std::vector<int>>($1->m_data)) {
            dims[0] = (long)(std::get<std::vector<int>>($1->m_data).size());
           array  = PyArray_SimpleNew(1, dims, NPY_INT);
        } else if(std::holds_alternative<std::vector<double>>($1->m_data)) {
            dims[0] = (long)(std::get<std::vector<double>>($1->m_data).size());
            array  = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
        }
        if(!array) {
            qWarning() << "gnomonMeshAttribute Wrapper, cant create new python array";
            return nullptr;
        }
        if(std::holds_alternative<std::vector<int>>($1->m_data)) {
            int *array_data = (int *)PyArray_DATA((PyArrayObject*)array);
            auto && cxx_data = std::get<std::vector<int>>($1->m_data);
            for(npy_intp i=0; i< dims[0]; ++i) {
                array_data[i] = cxx_data[i];
            }
        } else if(std::holds_alternative<std::vector<double>>($1->m_data)) {
            double *array_data = (double *)PyArray_DATA((PyArrayObject*)array);
            auto && cxx_data = std::get<std::vector<double>>($1->m_data);
            for(npy_intp i=0; i<  dims[0]; ++i) {
                array_data[i] = cxx_data[i];
            }
        }

        PyDict_SetItem($result, PyUnicode_FromString("data"), array);
    }
 }
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
        qDebug() << Q_FUNC_INFO << "cloning form of type " << v->name();
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

%insert("header") %{
std::shared_ptr<gnomonAbstractDynamicForm> ToFormSeries(PyObject *obj, const QString &form_name) {
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    int r;
    std::shared_ptr<gnomonAbstractDynamicForm> series = gnomonForm::createDynamicForm(form_name);
    std::string type_name = "std::shared_ptr<" + form_name.toStdString() + ">";
    swig_type_info *swig_type = SWIG_TypeQuery(type_name.c_str());

    while (PyDict_Next(obj, &pos, &key, &value)) {
        double t = PyFloat_AsDouble(key);
        std::shared_ptr<gnomonAbstractForm> v;
        //std::shared_ptr<gnomon## form_name##> v;
        void *s_v = 0;
        int newmem = 0;
        r = SWIG_ConvertPtrAndOwn(value, &s_v, swig_type,  0 , &newmem);
        if (!SWIG_IsOK(r)) {
            QString msg = "in ToFormSeries " + form_name;
            Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(r)), msg.toStdString().c_str());
        }
        if (s_v) {
            v = *(reinterpret_cast< std::shared_ptr< gnomonAbstractForm > * >(s_v));
            series->insert(t, v);
            if (newmem & SWIG_CAST_NEW_MEMORY) delete reinterpret_cast< std::shared_ptr< gnomonAbstractForm > * >(s_v);
        }
    }
    return series;
}


PyObject *FromFormSeries(std::shared_ptr<gnomonAbstractDynamicForm> series) {
    if (series) {
        PyObject *dict = PyDict_New();
        QList<double> times = series->times();
        std::shared_ptr<gnomonAbstractForm> *c;
        double t;
        double current_t = series->time();
        PyObject *v;

        QString type_name = "std::shared_ptr< " + series->formName() + " > *";
        swig_type_info *swig_type = SWIG_TypeQuery(type_name.toStdString().c_str());

        for (auto it = times.begin(); it != times.end(); ++it) {
            t = *it;
            c = new std::shared_ptr<gnomonAbstractForm>(series->atAsAbstract(t));
            v = SWIG_NewPointerObj(SWIG_as_voidptr(c), swig_type, SWIG_POINTER_OWN);  // ref count: 1
            PyDict_SetItem(dict, PyFloat_FromDouble(t), v);  // ref count: 2 (+1)
            Py_XDECREF(v);  // ref count: 1 (-1)
        }
        series->selectCurrentTime(current_t);
        return dict;
    } else {
        Py_RETURN_NONE;
    }
}

%}

%define WRAP_GNOMONCORE_FORM_SERIES(form_name)
    %typemap(in) std::shared_ptr<gnomon## form_name## Series> {
        if (PyDict_Check($input)) {
            $1 = std::dynamic_pointer_cast<gnomon## form_name## Series>(ToFormSeries($input, "gnomon" #form_name));
        } else {
            qDebug("typemap in PyDict is expected as input. Empty gnomon## form_name## Series is returned.");
            $1 = std::make_shared<gnomon## form_name## Series>();
        }
    }

    %typemap(directorout) std::shared_ptr<gnomon## form_name## Series> {
        PyObject *dict = static_cast<PyObject *>($1);
        if (PyDict_Check(dict)) {
            $result = std::dynamic_pointer_cast<gnomon## form_name## Series>(ToFormSeries(dict, "gnomon" #form_name));
        } else {
            qDebug("typemap dirout PyDict is expected as input. Empty gnomon## form_name## Series is returned.");
            $result = std::make_shared<gnomon## form_name## Series>();
        }
    }

    %typemap(out) std::shared_ptr<gnomon## form_name## Series> {
        $result = FromFormSeries(std::dynamic_pointer_cast<gnomonAbstractDynamicForm>(*&$1));
    }

    %typemap(directorin) std::shared_ptr<gnomon## form_name## Series> {
        $input = FromFormSeries(std::dynamic_pointer_cast<gnomonAbstractDynamicForm>(*&$1));
    }

    %extend QVariant {

    void setValue(gnomon## form_name *value) {
        $self->setValue(dtk::variantFromValue(value));
    }
    gnomon## form_name* tognomon## form_name () const {
        return $self->value<gnomon## form_name *>();
    }

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

/*    void setValue(gnomonCellComplex *value) {
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

    // void setValue(gnomonDiscreteDynamicForm *value) {
    //    $self->setValue(dtkCoreMetaType::variantFromValue(value));
    //}
    //gnomonDiscreteDynamicForm* tognomonDiscreteDynamicForm() const {
    //    return $self->value<gnomonDiscreteDynamicForm *>();
    //} 
    */

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
                series = ToFormSeries(value, v->name());
                //qDebug() << "inserting "<< k << series->times() << series->formName();
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
                const QString form_name = v->formName();
                series = ToFormSeries(value, form_name);
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
        v = FromFormSeries(series);
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
%extend gnomonLString{
    const char* __repr__()
    {
        static std::string s;
        auto&& lString = $self;
        QString
        str("<gnomoncore.gnomonLString");
        str += QString(" with %1 module(s)").arg(lString->moduleCount());
        str += QString(" at 0x%1>").arg((quintptr) lString, 12, 16, QChar('0'));
        s = str.toStdString();
        return s.data();
    }
}

%include <gnomonCore/gnomonForm/gnomonMesh/gnomonAbstractMeshData.h>
%include <gnomonCore/gnomonForm/gnomonMesh/gnomonMeshAttribute.h>
%include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
%extend gnomonMesh {
    const char* __repr__()
    {
        static std::string s;
        auto&& mesh = $self;
        QString str("<gnomoncore.gnomonMesh");
        str += QString(" with %1 points(s) and %1 cells").arg(mesh->data()->pointsCount(), mesh->data()->cellsCount());
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
