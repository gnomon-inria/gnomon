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


#include "gnomonPythonPluginLoader.h"

#include <QtCore>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")
#include <dtkScript>


void loadPluginGroup (const QString& module)
{
  int stat;
  QString code = "from pkg_resources import iter_entry_points\n";
  code = code + QString("for entry_point in iter_entry_points(group=") + QString("\"") + module  + QString("\"") + QString(", name=None):\n");
  code = code + QString("   entry_point.load()\n");

  dtkScriptInterpreterPython::instance()->interpret(code, &stat);

  //Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);
}

QStringList availablePluginsFromGroup(const QString & module) {
    QStringList available_plugins;

    Py_Initialize(); // NO OP if already initialized

    PyObject* pName = PyUnicode_FromString("gnomon_utils");
    PyObject* pModule = PyImport_Import(pName);

    if(pModule)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "available_plugins");
        if(pFunc && PyCallable_Check(pFunc))
        {
            PyObject* args = Py_BuildValue("(s)", module.toStdString().c_str());
            PyObject* entry_points = PyObject_CallObject(pFunc, args);

            for (Py_ssize_t i = 0; i < PyList_Size(entry_points); ++i) {
                Py_ssize_t size = 0;
                char const *tmp = PyUnicode_AsUTF8AndSize(PyList_GetItem(entry_points, i), &size);
                available_plugins.push_back(tmp);
            }
            Py_DECREF(args);
            Py_DECREF(entry_points);
        }
        else
        {
            dtkWarn() << Q_FUNC_INFO << "can't get available plugins for module " << module;
        }
        Py_DECREF(pFunc);
    }
    else
    {
         dtkWarn() << Q_FUNC_INFO << " Module" << module << " not imported";
    }
    Py_DECREF(pModule);
    Py_DECREF(pName);
    //Py_Finalize();
    return available_plugins;
}
