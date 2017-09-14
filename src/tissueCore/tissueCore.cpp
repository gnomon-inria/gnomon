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

#include "tissueCore.h"
#include "tissueCoreSettings.h"

#include <dtkLog>

namespace tissueCore {

DTK_DEFINE_LAYER_MANAGER;

void initialize(const QString& path)
{
    QString realpath = path;
    QStringList pathslist;

    if(path.isEmpty()) {
        tissueCoreSettings settings;
        settings.beginGroup("tissue-core");
        realpath = settings.value("plugins").toString();
        settings.endGroup();

        if (realpath.isEmpty()) {
            realpath = QDir(DTK_INSTALL_PREFIX).filePath("plugins/tissueCore");
            dtkDebug() << "no plugin path configured for tissueCore, using default" << realpath;
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
// tissueCore.cpp ends here
