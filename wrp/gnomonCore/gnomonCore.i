#pragma once

//%begin %{
//// threads handling
//#define SWIG_PYTHON_NO_USE_GIL
//
//#pragma push_macro("slots")
//#undef slots
//#include <Python.h>
//#pragma pop_macro("slots")
//
//#include <dtkScript>
//
//#define SWIG_PYTHON_THREAD_BEGIN_BLOCK dtkScriptInterpreterPython::instance()->childAcquireLock();
//#define SWIG_PYTHON_THREAD_END_BLOCK dtkScriptInterpreterPython::instance()->childReleaseLock();
//%}

%module(directors="1", package="gnomon.core", moduleimport="import _gnomoncore") gnomoncore

#ifdef SWIGWIN
%include <windows.i>
#endif
%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}

%include "std_array.i"
%include "std_vector.i"

%include <dtkBase/dtkBase.i>
%import <dtkCore/dtkCore.i>

%import <dtkImagingCore/dtkImagingCore.i>

%{

// VTK also includes a Py_hash_t typedef definition for Python 2 that clashes
// with SWIG's preprocessor macro
#if PY_VERSION_HEX < 0x3020000
#ifdef Py_hash_t
#undef Py_hash_t
#endif
#endif

#include <dtkCore>
#include <gnomonCore/gnomonAbstractDataDriver.h>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
#include <gnomonCore/gnomonForm/gnomonAbstractFormData.h>
#include <gnomonCore/gnomonForm/gnomonAbstractForm.h>
#include <gnomonCore/gnomonForm/gnomonSphereForm.h>
#include <gnomonCore/gnomonForm/gnomonTimeSeries.h>
#include <gnomonCore/gnomonForm/gnomonWallForm.h>

#include "gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonCore/gnomonForm/gnomonBinaryImage/gnomonAbstractBinaryImageData.h"
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonAbstractCellComplexData.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
// #include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonAbstractCellGraphData.h>
// #include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonCellGraph.h>
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

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm.h>
#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageFromImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexFromCellImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellGraph/gnomonAbstractCellGraphFromImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFilter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFromImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageQuantification.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageTracking.h>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFilter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFusion.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration.h>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringTranslation.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractFemSolver.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFilter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFromImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudFromImage.h>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudQuantification.h>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeConstructor.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeFromLString.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeTransform.h>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeWriter.h>
 
#include <gnomonCore/gnomonModel/gnomonAbstractModel.h>
#include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel.h>
#include <gnomonCore/gnomonModel/gnomonAbstractSystemScenario.h>
#include <gnomonCore/gnomonModel/gnomonSystem.h>

#include <gnomonCore/gnomonCore.h>
#include <gnomonCore/gnomonDataManager.h>
#include <gnomonCore/gnomonFileSystemFormReader.h>
#include <gnomonCore/gnomonFormVisitor.h>
#include <gnomonCore/gnomonLandmark.h>
#include <gnomonCore/gnomonTime.h>
#include <gnomonCore/gnomonTypeDef.h>
#include <vtkPythonUtil.h>
#include <vtkImageData.h>


%}

%feature("director");
%feature("autodoc","1");

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONCORE_EXPORT
#define GNOMONCORE_EXPORT

%define %apply_numpy_typemaps(TYPE, DATA_TYPECODE)

// 1 D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY1, int DIM) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 1 ? 1 : 0;
    
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (TYPE* IN_ARRAY1, int DIM)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[1] = { -1 };
  array = obj_to_array_contiguous_allow_conversion($input,
                                                   DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 1) ||
      !require_size(array, size, 1)) SWIG_fail;
  $1 = (TYPE*) array_data(array);
  $2 = (int) array_size(array,0);
}
%typemap(freearg) (TYPE* IN_ARRAY1, int DIM)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}


//2D array
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) (TYPE* IN_ARRAY2, int DIM1, int DIM2) {
  int ndims_ok = PyArray_NDIM((PyArrayObject*)$input) == 2 ? 1 : 0;
  $1 = (($input)
        && PyArray_Check($input) 
        && ndims_ok
        && PyArray_EquivTypenums(PyArray_TYPE((PyArrayObject*)$input), DATA_TYPECODE) ) ? 1 : 0;
}
%typemap(in, fragment="NumPy_Fragments")
  (DATA_TYPE* IN_ARRAY2, DIM_TYPE DIM1, DIM_TYPE DIM2)
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  npy_intp size[2] = { -1, -1 };
  array = obj_to_array_contiguous_allow_conversion($input, DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 2) ||
      !require_size(array, size, 2)) SWIG_fail;
  $1 = (DATA_TYPE*) array_data(array);
  $2 = (DIM_TYPE) array_size(array,0);
  $3 = (DIM_TYPE) array_size(array,1);
}
%typemap(freearg) (DATA_TYPE* IN_ARRAY2, DIM_TYPE DIM1, DIM_TYPE DIM2)
{
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}

%extend QVariant {
    void setValue(TYPE *IN_ARRAY1, int DIM) {
        QVector<TYPE> vec(IN_ARRAY1, IN_ARRAY1 + DIM);
        $self->setValue(vec);
     }

    void setValue(TYPE *IN_ARRAY2, int DIM1, int DIM2) {
        QVector<QVector<TYPE>> vec(DIM1, QVector<TYPE>(DIM2));
        for(int i=0; i<DIM1; ++i) {
            for(int j=0; j<DIM2; ++j) {
                vec[i][j] = IN_ARRAY2[i*DIM2 + j];
            }
        }
        $self->setValue(vec);
     }
}

%inline %{
    PyObject *toNpArray1##TYPE (const QVariant &var) {
        QVector<TYPE> vec = var.value<QVector<TYPE>>();
        npy_intp dims[1] = { vec.size() };
        PyObject *array = PyArray_SimpleNew(1, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim" << dims[0] << " and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< vec.size(); ++i) {
            data[i] = vec[i];
        }

        return array;
    }

    PyObject *toNpArray2##TYPE (const QVariant &var) {
        QVector<QVector<TYPE>> vec = var.value<QVector<QVector<TYPE>>>();
        int rows = vec.size();
        int cols = vec[0].size();
        npy_intp dims[2] = { rows, cols };
        PyObject *array = PyArray_SimpleNew(2, dims, DATA_TYPECODE);
        if (!array) {
            qWarning() << Q_FUNC_INFO << "cant create new python array of dim" << dims[0] << " and type DATA_TYPECODE";
            return nullptr;
        }
        TYPE *data = (TYPE *) PyArray_DATA((PyArrayObject*)array);
        for(int i=0; i< rows; ++i) {
            for(int j=0; j< cols; ++j) {
                data[i*cols + j] = vec[i][j];
            }
        }

        return array;
    }
%}

%enddef    /* %apply_numpy_typemaps() macro */

//%apply_numpy_typemaps(signed char       )
//%apply_numpy_typemaps(unsigned char     )
//%apply_numpy_typemaps(short             )
//%apply_numpy_typemaps(unsigned short    )
%apply_numpy_typemaps(int, NPY_INT)
//%apply_numpy_typemaps(unsigned int      )
  //%apply_numpy_typemaps(long              )
//%apply_numpy_typemaps(unsigned long     )
//%apply_numpy_typemaps(long long         )
//%apply_numpy_typemaps(unsigned long long)
//%apply_numpy_typemaps(float             )
%apply_numpy_typemaps(double, NPY_DOUBLE)


%typemap(out) QVariant gnomonDataDict::get {
    int type = $1.type();
    QString name($1.typeName());
    name = name.remove(' ');

    if (type == QMetaType::Int ||
        type == QMetaType::UInt ||
        type == QMetaType::Long ||
        type == QMetaType::ULong ||
        type == QMetaType::LongLong ||
        type == QMetaType::ULongLong) {
        $result = PyLong_FromLong($1.value<long>());
    } else if (type == QMetaType::Float ||
        type == QMetaType::Double) {
        $result = PyFloat_FromDouble($1.value<double>());
    } else if (type == QMetaType::QString) {
        $result = PyUnicode_FromString(qPrintable($1.value<QString>()));
    } else if (type == QMetaType::Bool) {
        bool b = $1.value<bool>();
        $result = b ? Py_True : Py_False;
    } else if (name == "QVector<int>") {
        PyObject *array = toNpArray1int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QVector<double>") {
        PyObject *array = toNpArray1double($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QVector<QVector<int>>") {
        PyObject *array = toNpArray2int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QVector<QVector<double>>") {
        PyObject *array = toNpArray2double($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else {
        qWarning() << Q_FUNC_INFO << "no conversion for name " << name << "I will return a void *";
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(&$1), SWIGTYPE_p_QVariant, 0 |  0 );
    }
}


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

    if ( $1 == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
    }
}

%typemap(directorout) vtkImageData* {

    $result = (vtkImageData*) vtkPythonUtil::GetPointerFromObject ( $1, "vtkImageData" );

    if ( $result == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
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



// /////////////////////////////////////////////////////////////////
// Form series
// /////////////////////////////////////////////////////////////////

%define WRAP_GNOMONCORE_FORM_SERIES(form_name)
    %fragment("To## form_name## Series", "header") {
        void To## form_name## Series(PyObject *obj, gnomon## form_name## Series *series) {
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            int r;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                double t = PyFloat_AsDouble(key);
                gnomon## form_name##  *v;
                void *s_v = 0;
                r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomon## form_name## , 0);
                if (SWIG_IsOK(r))
                    v = reinterpret_cast<gnomon## form_name##  *>(s_v);
                series->insert(t, v);
            }
        }
    }

    %fragment("From## form_name## Series", "header") {
        PyObject *From## form_name## Series(gnomon## form_name## Series *series) {
            if (series) {
                PyObject *dict = PyDict_New();
                QList<double> times = series->times();
                gnomon## form_name##  *c;
                double t;
                PyObject *v;
                for (auto it = times.begin(); it != times.end(); ++it) {
                    t = *it;
                    c = series->at(t);
                    v = SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p_gnomon## form_name## , 0 |  0 );
                    PyDict_SetItem(dict, PyFloat_FromDouble(t), v);
                }
                return dict;
            } else {
                Py_RETURN_NONE;
            }
        }
    }

    %typemap(in, fragment="To## form_name## Series") gnomon## form_name## Series *{
        $1 = new gnomon## form_name## Series();
        if (PyDict_Check($input)) {
            To## form_name## Series($input, $1);
        } else {
            qDebug("PyDict is expected as input. Empty time series is returned.");
        }
    }

    %typemap(freearg) gnomon## form_name## Series *{
        if ($1) {
            delete $1;
        }
    }

    %typemap(directorout, fragment="To## form_name## Series") gnomon## form_name## Series *{
        $result = new gnomon## form_name## Series();
        PyObject *dict = static_cast<PyObject *>($1);
        if (PyDict_Check(dict)) {
            To## form_name## Series(dict, $result);
        } else {
            qDebug("PyDict is expected as input. Empty time series is returned.");
        }
    }

    %typemap(out, fragment="From## form_name## Series") gnomon## form_name## Series *{
        $result = From## form_name## Series($1);
    }

    %typemap(directorin, fragment="From## form_name## Series") gnomon## form_name## Series *{
        $input = From## form_name## Series($1);
    }
%enddef

WRAP_GNOMONCORE_FORM_SERIES(BinaryImage)
WRAP_GNOMONCORE_FORM_SERIES(CellComplex)
WRAP_GNOMONCORE_FORM_SERIES(CellImage)
WRAP_GNOMONCORE_FORM_SERIES(DataDict)
WRAP_GNOMONCORE_FORM_SERIES(DataFrame)
WRAP_GNOMONCORE_FORM_SERIES(Image)
WRAP_GNOMONCORE_FORM_SERIES(LString)
WRAP_GNOMONCORE_FORM_SERIES(Mesh)
WRAP_GNOMONCORE_FORM_SERIES(PointCloud)
WRAP_GNOMONCORE_FORM_SERIES(Tree)
 
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

%typemap(in) QMap<QString, gnomonAbstractDynamicForm *> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractDynamicForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractDynamicForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractDynamicForm *>(s_v);
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractDynamicForm*> is returned.");
    }
}

%typemap(in) const QMap<QString, gnomonAbstractDynamicForm *>& {
    if (PyDict_Check($input)) {
        $1 = new QMap<QString, gnomonAbstractDynamicForm *>;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractDynamicForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractDynamicForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractDynamicForm *>(s_v);
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractDynamicForm*> is returned.");
    }
}

%typemap(freearg) const QMap<QString, gnomonAbstractDynamicForm *>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<QString, gnomonAbstractDynamicForm *> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonAbstractDynamicForm *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonAbstractDynamicForm, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonAbstractDynamicForm *>(s_v);
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonAbstractDynamicForm*> is returned.");
    }
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER, noblock=1) std::vector<gnomonLandmark, std::allocator< gnomonLandmark > > {
    $1 = PyList_Check($input) ? 1 : 0;
}

 %typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER, noblock=1) const std::vector<gnomonLandmark, std::allocator< gnomonLandmark > >& {
    $1 = PyList_Check($input) ? 1 : 0;
}

%typemap(out) std::vector<gnomonLandmark, std::allocator<gnomonLandmark>>
{
    $result = PyList_New($1.size());
    auto it  = $1.cbegin();
    auto end = $1.cend();
    for(int i = 0 ; it != end; ++it, ++i)  {
        PyObject *point = PyList_New(3);
        PyList_SET_ITEM(point, 0, PyFloat_FromDouble(it->pos[0]));
        PyList_SET_ITEM(point, 1, PyFloat_FromDouble(it->pos[1]));
        PyList_SET_ITEM(point, 2, PyFloat_FromDouble(it->pos[2]));
        PyList_SET_ITEM($result, i, point);
    }
}

%typemap(out) const std::vector<gnomonLandmark, std:allocator<gnomonLandmark>>&
{
    $result = PyList_New($1.size());
    auto it  = $1.cbegin();
    auto end = $1.cend();
    for(int i = 0 ; it != end; ++it, ++i)  {
        PyObject *point = PyList_New(3);
        PyList_SET_ITEM(point, 0, PyFloat_FromDouble(it->pos[0]));
        PyList_SET_ITEM(point, 1, PyFloat_FromDouble(it->pos[1]));
        PyList_SET_ITEM(point, 2, PyFloat_FromDouble(it->pos[2]));
        PyList_SET_ITEM($result, i, point);
    }
}

%typemap(directorin) const std::vector<gnomonLandmark, std::allocator< gnomonLandmark > >&
{
    PyObject *list = PyList_New($1.size());
    auto it  = $1.cbegin();
    auto end = $1.cend();
    for(int i = 0 ; it != end; ++it, ++i)  {
        PyObject *point = PyList_New(3);
        PyList_SET_ITEM(point, 0, PyFloat_FromDouble(it->pos[0]));
        PyList_SET_ITEM(point, 1, PyFloat_FromDouble(it->pos[1]));
        PyList_SET_ITEM(point, 2, PyFloat_FromDouble(it->pos[2]));
        PyList_SET_ITEM(list, i, point);
    }
    $input = list;
}

%pythoncode %{
    def world():
        import dtkcore
        return dtkcore.dtkCoreObjectManager_instance()
%}

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%define INCLUDE_GNOMON_CONCEPT(name, short_name, path)
%include < ## path/ ## name.h>
%inline
%{
    ## name *objectManager## short_name(const QString& key)
    {
        dtkCoreObjectManager *object_manager = dtkCoreObjectManager::instance();
        if (object_manager->keys().contains(key)) {
            QVariant v = object_manager->value(key);
            if (v.canConvert<## name *>()) {
                return v.value<## name *>();
            }
            return nullptr;
        } else {
            return nullptr;
        }
    }
%}
%enddef

%include <gnomonCore/gnomonAbstractDataDriver.h>
%include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
%include <gnomonCore/gnomonForm/gnomonAbstractForm.h>
%include <gnomonCore/gnomonForm/gnomonAbstractFormData.h>
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

// %include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonAbstractCellGraphData.h>
// %include <gnomonCore/gnomonForm/gnomonCellGraph/gnomonCellGraph.h>
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

%include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
%include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAdapter.h>
// %include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm.h>
INCLUDE_GNOMON_CONCEPT(gnomonAbstractFormAlgorithm, FormAlgorithm, gnomonCore/gnomonAlgorithm)
%include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageFromImage.h>
%include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageReader.h>  
%include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexFromCellImage.h>
// %include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexReader.h>
INCLUDE_GNOMON_CONCEPT(gnomonAbstractCellComplexReader, CellComplexReader, gnomonCore/gnomonAlgorithm/gnomonCellComplex)
%include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellGraph/gnomonAbstractCellGraphFromImage.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFilter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFromImage.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageQuantification.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageTracking.h>
%include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFilter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFusion.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration.h>
%include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringAdapter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringTranslation.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractFemSolver.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFilter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFromImage.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudFromImage.h>
%include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudQuantification.h>
// %include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudReader.h>
INCLUDE_GNOMON_CONCEPT(gnomonAbstractPointCloudReader, PointCloudReader, gnomonCore/gnomonAlgorithm/gnomonPointCloud)
// %include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudWriter.h>
INCLUDE_GNOMON_CONCEPT(gnomonAbstractPointCloudWriter, PointCloudWriter, gnomonCore/gnomonAlgorithm/gnomonPointCloud)
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeAdapter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeConstructor.h>
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeFromLString.h>
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeTransform.h>
%include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeWriter.h>

%include <gnomonCore/gnomonModel/gnomonAbstractModel.h>
%include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel.h>
%include <gnomonCore/gnomonModel/gnomonAbstractSystemScenario.h>
%include <gnomonCore/gnomonModel/gnomonSystem.h>

%include <gnomonCore/gnomonCore.h>
%include <gnomonCore/gnomonDataManager.h>
%include <gnomonCore/gnomonFileSystemFormReader.h>
%include <gnomonCore/gnomonFormVisitor.h>
%include <gnomonCore/gnomonLandmark.h>
%include <gnomonCore/gnomonTime.h>
%include <gnomonCore/gnomonTypeDef.h>

%include <QtCore/QVariant.i>





namespace std {
    %template(vec3_t) array<double, 3>;
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

%template(gnomonAbstractCellComplexAdapter) gnomonAbstractFormAdapter<gnomonCellComplexSeries>;
%template(gnomonAbstractLStringAdapter) gnomonAbstractFormAdapter<gnomonLStringSeries>;
%template(gnomonAbstractMeshAdapter) gnomonAbstractFormAdapter<gnomonMeshSeries>;
%template(gnomonAbstractTreeAdapter) gnomonAbstractFormAdapter<gnomonTreeSeries>;

/* %pythoncode "gnomonCore/gnomonPlugin.py" */

//
// gnomonCore.i.in ends here
