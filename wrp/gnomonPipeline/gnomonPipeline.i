#pragma once

%module(directors="1", package="gnomon.pipeline", moduleimport="import _gnomonpipeline") gnomonpipeline
//%include <gnomonCore/gnomonForm.i>

/*
%include "std_shared_ptr.i"

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


%import(module="gnomon.core") <gnomonCore/gnomonForm/gnomonAbstractDynamicForm.h>
//%import(module="gnomon.core") <gnomonCore/gnomonForm/gnomonAbstractForm.h>
*/

%{
    #include <dtkCore>
    //#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
    
    #include <gnomonPipeline.h>
    #include <gnomonPipelineManager.h>
    #include <gnomonPipelineEdge.h>
    #include <gnomonPipelineNode.h>
    #include <gnomonPipelineNodeTask.h>
    #include <gnomonPipelinePort.h>
%}

%include <gnomonCore/gnomonForm.i>

//%include <gnomonCore/gnomonCore.i>

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONPIPELINE_EXPORT
#define GNOMONPIPELINE_EXPORT
#undef  Q_INVOKABLE
#define Q_INVOKABLE

// /////////////////////////////////////////////////////////////////
// List of StringList
// /////////////////////////////////////////////////////////////////

%typemap(in) QList<QStringList> {
    if (PyList_Check($input)) {
        int size = PyList_Size($input);
        for (int i=0; i<size; ++i) {
            if (PyList_Check(PyList_GET_ITEM($input, i))) {
                QStringList l;
                int list_size = PyList_Size(PyList_GET_ITEM($input, i));
                for (int j=0; j<list_size; ++j) {
                    QString s = QString(PyUnicode_AsUTF8(PyList_GET_ITEM(PyList_GET_ITEM($input, i), j)));
                    l.append(s);
                }
                $1.append(l);
             }
        }
    } else {
        qDebug("PyList is expected as input. Empty QList<QStringList> is returned.");
    }
}

%typemap(directorout) QList<QStringList> {
    if (PyList_Check($1)) {
        int size = PyList_Size($1);
        for (int i=0; i<size; ++i) {
            if (PyList_Check(PyList_GET_ITEM($1, i))) {
                QStringList l;
                int list_size = PyList_Size(PyList_GET_ITEM($1, i));
                for (int j=0; j<list_size; ++j) {
                    QString s = QString(PyUnicode_AsUTF8(PyList_GET_ITEM(PyList_GET_ITEM($1, i), j)));
                    l.append(s);
                }
                $result.append(l);
             }
        }
    } else {
        qDebug("PyList is expected as input. Empty QList<QStringList> is returned.");
    }
}

%typemap(out) QList<QStringList> {
    int size = $1.size();
    $result = PyList_New(size);
    for(int i=0; i<size; ++i) {
        int list_size = $1[i].size();
        PyObject *py_list = PyList_New(list_size);
        for(int j=0; j<list_size; ++j) {
            PyList_SET_ITEM(py_list, j, PyUnicode_FromString($1[i][j].toStdString().c_str()));
        }
        PyList_SET_ITEM($result, i, py_list);
    }
}

%typemap(directorin) QList<QStringList> {
    int size = $1.size();
    PyObject *list = PyList_New(size);
    for(int i=0; i<size; ++i) {
        int list_size = $1[i].size();
        PyObject *py_list = PyList_New(list_size);
        for(int j=0; j<list_size; ++j) {
            PyList_SET_ITEM(py_list, j, PyUnicode_FromString($1[i][j].toStdString().c_str()));
        }
        PyList_SET_ITEM(list, i, py_list);
    }
    $input = list;
}

//%include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
%include <gnomonPipeline.h>
%include <gnomonPipelineManager.h>
%include <gnomonPipelineEdge.h>
%include <gnomonPipelineNode.h>
%include <gnomonPipelineNodeTask.h>
%include <gnomonPipelinePort.h>