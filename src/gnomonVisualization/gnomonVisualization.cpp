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

#include "gnomonVisualization.h"
#include "gnomonCoreSettings.h"

#include <dtkLog>

namespace gnomonVisualization {

DTK_DEFINE_LAYER_MANAGER;

void activateObjectManager(void)
{
    manager().setObjectManager(dtkObjectManager::instance());
}

void initialize(const QString& path)
{
    QString realpath = path;
    QStringList pathslist;

    if(path.isEmpty()) {
        gnomonCoreSettings settings;
        settings.beginGroup("gnomon-core");
        realpath = settings.value("plugins").toString();
        settings.endGroup();

        if (realpath.isEmpty()) {
            realpath = QDir(DTK_INSTALL_PREFIX).filePath("plugins/gnomonVisualization");
            dtkDebug() << "no plugin path configured for gnomonVisualization, using default" << realpath;
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
// gnomonVisualization.cpp ends here
