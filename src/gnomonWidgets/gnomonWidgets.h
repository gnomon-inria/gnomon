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

#include <gnomonWidgetsExport.h>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace gnomonWidgets {

    GNOMONWIDGETS_EXPORT dtkCoreLayerManager& manager(void);

    GNOMONWIDGETS_EXPORT void activateObjectManager(void);

    GNOMONWIDGETS_EXPORT void   initialize(const QString& path = QString());
    GNOMONWIDGETS_EXPORT void uninitialize(void);

    GNOMONWIDGETS_EXPORT void setVerboseLoading(bool b);
    GNOMONWIDGETS_EXPORT void setAutoLoading(bool auto_loading);
};

//
// gnomonWidgets.h ends here
