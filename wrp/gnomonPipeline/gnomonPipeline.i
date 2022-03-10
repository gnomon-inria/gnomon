#pragma once

%module(directors="1", package="gnomon.pipeline", moduleimport="import _gnomonpipeline") gnomonpipeline

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}

%include "std_array.i"
%include "std_vector.i"
%include "carrays.i"

%array_class(double, doubleArray);

%import <dtkBase/dtkBase.i>
%import <dtkCore/dtkCore.i>

%{
    #include <dtkCore>

    #include <gnomonCore>
    #include <gnomonPipeline.h>
    #include <gnomonPipelineManager.h>
    #include <gnomonPipelineEdge.h>
    #include <gnomonPipelineNode.h>
    #include <gnomonPipelinePort.h>
%}

%include <gnomonCore/gnomonCore.i>


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

%include <gnomonPipeline.h>
%include <gnomonPipelineManager.h>
%include <gnomonPipelineEdge.h>
%include <gnomonPipelineNode.h>
%include <gnomonPipelinePort.h>