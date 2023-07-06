#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>

namespace gnomonCore {

    GNOMONCORE_EXPORT dtkCoreLayerManager& manager(void);

    GNOMONCORE_EXPORT void activateObjectManager(void);

    GNOMONCORE_EXPORT void   initialize(const QString& path = QString(), bool from_python=false);
    GNOMONCORE_EXPORT void uninitialize(void);

    GNOMONCORE_EXPORT void setVerboseLoading(bool b);
    GNOMONCORE_EXPORT void setAutoLoading(bool auto_loading);

    GNOMONCORE_EXPORT extern QThread *gui_thread;
};

//
// gnomonCore.h ends here
