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

#include <gnomonComposerExtensionExport.h>

#include <dtkComposer/dtkComposerExtension.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSEREXTENSION_EXPORT gnomonComposerExtension : public dtkComposerExtension
{
public:
     gnomonComposerExtension(void);
    ~gnomonComposerExtension(void);

public:
    void extend(dtkComposerNodeFactory *factory);
};

//
// gnomonComposerExtension.h ends here
