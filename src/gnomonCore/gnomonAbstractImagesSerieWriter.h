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

class GNOMONCORE_EXPORT gnomonAbstractImagesSerieWriter
{
public:
    virtual ~gnomonAbstractImagesSerieWriter(void) = default;

public:
    virtual void setImage(dtkImage *img) = 0;
    virtual void setPath(const QString& path) = 0;

public:
    virtual void run(void) = 0;

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImagesSerieWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImagesSerieWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImagesSerieWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImagesSerieWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImagesSerieWriter, GNOMONCORE_EXPORT, imagesSerieWriter);
}

//
// gnomonAbstractImageWriter.h ends here
