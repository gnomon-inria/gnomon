#pragma once

#include <cstddef>
%module(directors="1", doctring="Wrapping of gnomonCore for python usage", package="gnomon.core", moduleimport="import _gnomoncore") gnomoncore

#ifdef SWIGWIN
%include <windows.i>
#endif

%include <gnomonCore/numpyWrapper.i>
%include "std_array.i"
%include "std_vector.i"

%include <dtkBase/dtkBase.i>
%include <gnomonMacro.i>
%import <dtkCore/dtkCore.i>

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
#include <gnomonCore/gnomonMorphonetHelper.h>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAdapter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormWriter.h>
#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
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
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringReader.h>
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringTranslation.h>
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringWriter.h>
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
#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel.h>
#include <gnomonCore/gnomonModel/gnomonAbstractSystemScenario.h>
#include <gnomonCore/gnomonModel/gnomonSystem.h>

#include <gnomonCore/gnomonCore.h>
#include <gnomonCore/gnomonCorePlugin.h>
#include <gnomonCore/gnomonFormVisitor.h>
#include <gnomonCore/gnomonLandmark.h>
#include <gnomonCore/gnomonPluginFactory.h>
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
    int type = $1.typeId();
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
    } else if (name == "QList<int>") {
        PyObject *array = toNpArray1int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<double>") {
        PyObject *array = toNpArray1double($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<int>>") {
        PyObject *array = toNpArray2int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<double>>") {
        PyObject *array = toNpArray2double($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<QList<int>>>") {
        PyObject *array = toNpArray3int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<QList<double>>>") {
        PyObject *array = toNpArray3double($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<QList<QList<int>>>>") {
        PyObject *array = toNpArray4int($1);
        $result = SWIG_Python_AppendOutput($result,(PyObject*)array);
    } else if (name == "QList<QList<QList<QList<double>>>>") {
        PyObject *array = toNpArray4double($1);
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
    //$1->Register(NULL);
    if ( $1 == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
    }
}

%typemap(directorout) vtkImageData* {

    $result = (vtkImageData*) vtkPythonUtil::GetPointerFromObject ( $1, "vtkImageData" );
    //$result->Register(NULL);
    if ( $result == NULL ) {
        qDebug("Fail to convert to vtkImageData*");
    }
}

%include <gnomonCore/gnomonForm.i>

// /////////////////////////////////////////////////////////////////
// Landmarks
// /////////////////////////////////////////////////////////////////

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

%include <gnomonCore/gnomonPluginFactory.h>
%include <gnomonCore/gnomonAbstractDataDriver.h>
%include <gnomonCore/gnomonMorphonetHelper.h>

%include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
%include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAdapter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormWriter.h>
%include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
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
%include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringReader.h>
%include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringTranslation.h>
%include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringWriter.h>
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
%include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel.h>
%include <gnomonCore/gnomonModel/gnomonAbstractSystemScenario.h>
%include <gnomonCore/gnomonModel/gnomonSystem.h>

%include <gnomonCore/gnomonCore.h>
%include <gnomonCore/gnomonFormVisitor.h>
%include <gnomonCore/gnomonLandmark.h>
%include <gnomonCore/gnomonTime.h>
%include <gnomonCore/gnomonTypeDef.h>

%include <QtCore/QVariant.i>

%inline %{
extern void callback_wrapper(gnomonAbstractAlgorithm *algo, gnomonAbstractCommand *command) {
    algo->callback(command);
}
%}


namespace std {
    %template(vec3_t) array<double, 3>;
}

%template(gnomonAbstractCellComplexAdapter) gnomonAbstractFormAdapter<gnomonCellComplexSeries>;
%template(gnomonAbstractLStringAdapter) gnomonAbstractFormAdapter<gnomonLStringSeries>;
%template(gnomonAbstractMeshAdapter) gnomonAbstractFormAdapter<gnomonMeshSeries>;
%template(gnomonAbstractTreeAdapter) gnomonAbstractFormAdapter<gnomonTreeSeries>;

/* %pythoncode "gnomonCore/gnomonPlugin.py" */

//
// gnomonCore.i.in ends here
