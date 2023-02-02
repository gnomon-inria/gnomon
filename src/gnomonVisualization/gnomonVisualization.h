#pragma once

#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCoreLayerManager>


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace gnomonVisualization {

    GNOMONVISUALIZATION_EXPORT dtkCoreLayerManager& manager(void);

    GNOMONVISUALIZATION_EXPORT void activateObjectManager(void);

    GNOMONVISUALIZATION_EXPORT void   initialize(const QString& path = QString());
    GNOMONVISUALIZATION_EXPORT void uninitialize(void);

    GNOMONVISUALIZATION_EXPORT void setVerboseLoading(bool b);
    GNOMONVISUALIZATION_EXPORT void setAutoLoading(bool auto_loading);
};

//
// gnomonVisualization.h ends here
