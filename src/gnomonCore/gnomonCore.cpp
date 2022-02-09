
#include <gnomonConfig.h>

#include "gnomonCore.h"
#include "gnomonCoreSettings.h"

#include <dtkLog>
#include <dtkScript>

namespace gnomonCore {

DTK_DEFINE_LAYER_MANAGER;

void activateObjectManager(void)
{
    manager().setObjectManager(dtkCoreObjectManager::instance());
}

void initialize(const QString& path)
{
    // Should work with an future dtkScript version
    // dtkScriptInterpreterPython::instance()->allowThreads();
    QString realpath = path;
    QStringList pathslist;

    if(path.isEmpty()) {
        gnomonCoreSettings settings;
        settings.beginGroup("gnomon-core");
        realpath = settings.value("plugins").toString();
        settings.endGroup();

        if (realpath.isEmpty()) {
            realpath = QDir(GNOMON_INSTALL_PREFIX).filePath("plugins/gnomonCore");
            dtkDebug() << "no plugin path configured for gnomonCore, using default" << realpath;
        }

        pathslist = realpath.split(":");

    } else {
        pathslist = realpath.split(":");
    }

    for(const QString& v_path : pathslist) {
        manager().initialize(v_path);
    }
}

void uninitialize(void)
{
    // Should work with an future dtkScript version
    // dtkScriptInterpreterPython::instance()->endAllowThreads();
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

};

//
// gnomonCore.cpp ends here
