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


%include <dtkBase/dtkBase.i>

%{


#include <gnomonCore/gnomonCoreParameter.h>

%}

%feature("director");
%feature("autodoc","1");

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONCORE_EXPORT
#define GNOMONCORE_EXPORT

// /////////////////////////////////////////////////////////////////
// Parameter dictionary
// /////////////////////////////////////////////////////////////////

%typemap(in) QMap<QString, gnomonCoreParameter *> {
    if (PyDict_Check($input)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonCoreParameter *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonCoreParameter, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonCoreParameter *>(s_v);
            $1.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonCoreParameter*> is returned.");
    }
}

%typemap(in) const QMap<QString, gnomonCoreParameter *>& {
    if (PyDict_Check($input)) {
        $1 = new QMap<QString, gnomonCoreParameter *>;
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next($input, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonCoreParameter *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonCoreParameter, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonCoreParameter *>(s_v);
            $1->insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonCoreParameter*> is returned.");
    }
}

%typemap(freearg) const QMap<QString, gnomonCoreParameter *>& {
    if ($1) {
        delete $1;
    }
}

%typemap(directorout) QMap<QString, gnomonCoreParameter *> {
    PyObject *dict = static_cast<PyObject *>($1);
    if (PyDict_Check(dict)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        int r;
        while (PyDict_Next(dict, &pos, &key, &value)) {
            QString k = QString(PyUnicode_AsUTF8(key));
            gnomonCoreParameter *v;
            void *s_v = 0;
            r = SWIG_ConvertPtr(value, &s_v, SWIGTYPE_p_gnomonCoreParameter, 0);
            if (SWIG_IsOK(r))
                v = reinterpret_cast<gnomonCoreParameter *>(s_v);
            $result.insert(k, v);
        }
    } else {
        qDebug("PyDict is expected as input. Empty QMap<QString, gnomonCoreParameter*> is returned.");
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



%rename(ParameterBool)       gnomonCoreParameterBool;
%rename(ParameterString)     gnomonCoreParameterString;
%rename(ParameterStringList) gnomonCoreParameterStringList;
%rename(ParameterVariant)    gnomonCoreParameterVariant;
%rename(ParameterText)       gnomonCoreParameterText;

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////



%include <gnomonCore/gnomonCoreParameter.h>


%template(ParameterShort)  gnomonCoreParameterNumeric<short>;
%template(ParameterUShort) gnomonCoreParameterNumeric<unsigned short>;
%template(ParameterInt)    gnomonCoreParameterNumeric<int>;
%template(ParameterUInt)   gnomonCoreParameterNumeric<unsigned int>;
%template(ParameterLong)   gnomonCoreParameterNumeric<long>;
%template(ParameterULong)  gnomonCoreParameterNumeric<unsigned long>;
%template(ParameterDouble) gnomonCoreParameterNumeric<double>;


//
// gnomonCoreParameter.i.in ends here
