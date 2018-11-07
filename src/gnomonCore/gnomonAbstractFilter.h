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

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

class gnomonMultiChannelImage;

class GNOMONCORE_EXPORT gnomonAbstractFilter
{
public:
    virtual ~gnomonAbstractFilter(void) {}

public:
    virtual void setImage(gnomonMultiChannelImage* image) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonMultiChannelImage *filteredImage(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractFilter *)

DTK_DECLARE_PLUGIN(gnomonAbstractFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractFilter, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractFilter, GNOMONCORE_EXPORT, filter);
}

//
// gnomonAbstractFilter.h ends here
