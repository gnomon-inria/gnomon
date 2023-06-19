#include "gnomonPythonPluginLoader.h"
#include <gnomonConfig.h>

#include <QtCore>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkLog>
#include <dtkScript>

QMap<QString, gnomonPluginManagerBase*>& pluginsManagers(void) {
    static QMap<QString, gnomonPluginManagerBase*> _managers_instance;
    return _managers_instance;
}

QMap<QString, gnomonPluginFactoryBase*>& pluginsFactories(void) {
    static QMap<QString, gnomonPluginFactoryBase*> _factories_instance;
    return _factories_instance;
}

void loadPluginGroup (const QString& module)
{
    int stat;
    QString code = "from gnomon.utils import load_plugin_group\n";
    code = code + QString("load_plugin_group(\"") + module  + QString("\")");
    dtkScriptInterpreterPython::instance()->interpret(code, &stat);

    if(!pluginsManagers()[module]) {
        dtkWarn() << "cannot find plugin manager for " << module;
        dtkWarn() << "keys are " << pluginsManagers().keys();
        return;
    }
    pluginsManagers()[module]->initialize(GNOMON_PLUGIN_PATH);
    //Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);
}

QStringList availablePluginsFromGroup(const QString & module) {
    QStringList available_plugins;
    if(!pluginsManagers()[module]) {
      dtkWarn() << "cannot find plugin Manager for " << module;
      dtkWarn() << "keys are " << pluginsManagers().keys();
    } else {
        //needs to initialize the plugins manager to
        //initialize factory for c++ plugins
        pluginsManagers()[module]->initialize(GNOMON_PLUGIN_PATH);
        available_plugins += pluginsFactories()[module]->keys();
    }

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

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

    PyGILState_Release(gstate);

    return available_plugins;
}

QMap<QString, QString> pluginMetadata(const QString &group, const QString &plugin_name) {
    //need package and conda_channel at least;

    //c++ plugin
    if(pluginsManagers()[group] && pluginsManagers()[group]->availablePlugins().contains(plugin_name)) {
        return pluginsManagers()[group]->metadatas(plugin_name);
    }

    //python plugin
    QMap<QString, QString> metadata;
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

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
            if(py_metadata) {
                while (PyDict_Next(py_metadata, &pos, &key, &value)) {
                    Py_ssize_t size_key = 0;
                    Py_ssize_t size_val = 0;
                    metadata.insert(
                                    PyUnicode_AsUTF8AndSize(key, &size_key),
                                    PyUnicode_AsUTF8AndSize(value, &size_val)
                                    );
                }
            }
            Py_DECREF(args);
            Py_XDECREF(py_metadata);
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

    PyGILState_Release(gstate);
    return metadata;
}
