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

    TISSUECORE_EXPORT void   initialize(const QString& path);
    TISSUECORE_EXPORT void uninitialize(void);
};

//
// tissueCore.h ends here
