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

#include <tissueCoreExport.h>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace tissueCore {

    TISSUECORE_EXPORT dtkCoreLayerManager& manager(void);

    TISSUECORE_EXPORT void   initialize(const QString& path = QString());
    TISSUECORE_EXPORT void uninitialize(void);

    TISSUECORE_EXPORT void setVerboseLoading(bool b);
    TISSUECORE_EXPORT void setAutoLoading(bool auto_loading);
};

//
// tissueCore.h ends here
