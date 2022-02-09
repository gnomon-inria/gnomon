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

#include <gnomonCoreExport.h>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace gnomonCore {

    GNOMONCORE_EXPORT dtkCoreLayerManager& manager(void);

    GNOMONCORE_EXPORT void activateObjectManager(void);

    GNOMONCORE_EXPORT void   initialize(const QString& path = QString());
    GNOMONCORE_EXPORT void uninitialize(void);

    GNOMONCORE_EXPORT void setVerboseLoading(bool b);
    GNOMONCORE_EXPORT void setAutoLoading(bool auto_loading);

    //static void * visu_interpreter = nullptr;
};

//
// gnomonCore.h ends here
