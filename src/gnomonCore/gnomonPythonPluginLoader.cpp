
#include "gnomonPythonPluginLoader.h"

#include <QtCore>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkLog>
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

    dtkScriptInterpreterPython::instance()->childAcquireLock(); // getting lock from main interpreter

    PyObject* pName = PyUnicode_FromString("gnomon.utils");
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
    dtkScriptInterpreterPython::instance()->childReleaseLock();
    return available_plugins;
}

QMap<QString, QString> pluginMetadata(const QString &group, const QString &plugin_name) {
    QMap<QString, QString> metadata;
    dtkScriptInterpreterPython::instance()->childAcquireLock(); // getting lock from main interpreter

    PyObject* pName = PyUnicode_FromString("gnomon.utils.gnomonPlugin");
    PyObject* pModule = PyImport_Import(pName);

    if(pModule)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "plugin_metadata");
        if(pFunc && PyCallable_Check(pFunc))
        {
            PyObject* args = Py_BuildValue("(s, s)", group.toStdString().c_str(), plugin_name.toStdString().c_str());
            PyObject* py_metadata = PyObject_CallObject(pFunc, args);

            PyObject *key, *value;
            Py_ssize_t pos = 0;

            while (PyDict_Next(py_metadata, &pos, &key, &value)) {
                /* do something interesting with the values... */
                Py_ssize_t size_key = 0;
                Py_ssize_t size_val = 0;
                metadata.insert(
                        PyUnicode_AsUTF8AndSize(key, &size_key),
                        PyUnicode_AsUTF8AndSize(value, &size_val)
                );
            }
            Py_DECREF(args);
            Py_DECREF(py_metadata);
            Py_DECREF(key);
            Py_DECREF(value);
        }
        else
        {
            dtkWarn() << Q_FUNC_INFO << "can't get plugins metadata for plugin " << plugin_name;
        }
        Py_DECREF(pFunc);
    }
    else
    {
        dtkWarn() << Q_FUNC_INFO << "Import gnomon.utils.gnomonPlugin failed. This is worrying";
    }
    Py_DECREF(pModule);
    Py_DECREF(pName);

    dtkScriptInterpreterPython::instance()->childReleaseLock();
    return metadata;
}
