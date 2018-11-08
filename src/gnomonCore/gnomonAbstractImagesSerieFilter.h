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

#include <gnomonCoreExport>

#include <dtkCore>

class dtkImage;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImagesSerieFilter
{
public:
    virtual ~gnomonAbstractImagesSerieFilter(void) = default;

public:
    virtual void setImage(dtkImage *image) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual dtkImage *at(double t) = 0;
    virtual dtkImage *next(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImagesSerieFilter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImagesSerieFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImagesSerieFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImagesSerieFilter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImagesSerieFilter, GNOMONCORE_EXPORT, imagesSerieFilter);
}

//
// gnomonAbstractImagesSerieFilter.h ends here
