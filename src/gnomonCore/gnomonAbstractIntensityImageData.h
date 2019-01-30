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

class dtkImage;

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractIntensityImageData
{
public:
             gnomonAbstractIntensityImageData(void) = default;
    virtual ~gnomonAbstractIntensityImageData(void) {};

    virtual gnomonAbstractIntensityImageData* clone(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QVariantMap metadata(void) const = 0;


public:
    virtual void setImage(dtkImage *image) = 0;
    virtual dtkImage* image(void) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractIntensityImageData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractIntensityImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractIntensityImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractIntensityImageData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractIntensityImageData, GNOMONCORE_EXPORT, intensityimagedata);
}

// gnomonAbstractIntensityImageData.h ends here
