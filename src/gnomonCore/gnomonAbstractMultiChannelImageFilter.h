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

class GNOMONCORE_EXPORT gnomonAbstractMultiChannelImageFilter
{
public:
    virtual ~gnomonAbstractMultiChannelImageFilter(void) {}

public:
    virtual void setImage(gnomonMultiChannelImage* image) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonMultiChannelImage *filteredImage(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractMultiChannelImageFilter *)

DTK_DECLARE_PLUGIN(gnomonAbstractMultiChannelImageFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMultiChannelImageFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMultiChannelImageFilter, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMultiChannelImageFilter, GNOMONCORE_EXPORT, multichannelimageFilter);
}

//
// gnomonAbstractMultiChannelImageFilter.h ends here
