#include <gnomonConfig.h>

#include "gnomonCore.h"
#include "gnomonCoreSettings.h"
#include "gnomonForm/gnomonMesh/gnomonAbstractMeshData.h"
#include "gnomonForm/gnomonMesh/gnomonMeshDataStdVector.h"

#include <dtkLog>
#include <dtkScript>

namespace gnomonCore {

DTK_DEFINE_LAYER_MANAGER;

void activateObjectManager(void)
{
    manager().setObjectManager(dtkCoreObjectManager::instance());
}

void initialize(const QString& path, bool from_python)
{
    // Should work with an future dtkScript version
    if(!from_python) {
        dtkScriptInterpreterPython::instance()->allowThreads();
    }

    QString realpath = path;
    QStringList pathslist;

    if(path.isEmpty()) {
        gnomonCoreSettings settings;
        settings.beginGroup("gnomon-core");
        realpath = settings.value("plugins").toString();
        settings.endGroup();

        if (realpath.isEmpty()) {
            realpath = QDir(GNOMON_INSTALL_PREFIX).filePath("plugins");
            dtkDebug() << "no plugin path configured for gnomonCore, using default" << realpath;
        }

        pathslist = realpath.split(":");

    } else {
        pathslist = realpath.split(":");
    }

    for(const QString& v_path : pathslist) {
        manager().initialize(v_path);
    }
    
    gnomonCore::meshData::pluginFactory().record("gnomonMeshDataStdVector", gnomonMeshDataStdVectorCreator);
    gnomonCore::gui_thread = QObject().thread();
}

void uninitialize(void)
{
    dtkScriptInterpreterPython::instance()->endAllowThreads();
    manager().uninitialize();
}

void setVerboseLoading(bool b)
{
    manager().setVerboseLoading(b);
}

void setAutoLoading(bool auto_load)
{
    manager().setAutoLoading(auto_load);
}

QThread *gui_thread = nullptr;

};

//
// gnomonCore.cpp ends here
