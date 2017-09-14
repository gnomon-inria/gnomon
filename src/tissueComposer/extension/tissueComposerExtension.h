// Version: $Id: d0c58a963562b8af0add85fe1984c3f775982dd8 $
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

#include <tissueComposerExtensionExport.h>

#include <dtkComposer/dtkComposerExtension.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class TISSUECOMPOSEREXTENSION_EXPORT tissueComposerExtension : public dtkComposerExtension
{
public:
     tissueComposerExtension(void);
    ~tissueComposerExtension(void);

public:
    void extend(dtkComposerNodeFactory *factory);
};

//
// tissueComposerExtension.h ends here
