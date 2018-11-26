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

class GNOMONCORE_EXPORT gnomonAbstractSurfaceImageFromImage
{
public:
             gnomonAbstractSurfaceImageFromImage(void) = default;
    virtual ~gnomonAbstractSurfaceImageFromImage(void) = default;

public:
    virtual void setImage(dtkImage *image) = 0;
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;

public:
    virtual dtkImage *computed3DSurfaceImage(void) const = 0;
    virtual dtkImage *computed2DSurfaceImage(void) const = 0;
    virtual QMap<QString, QVariant> parameters(void) const = 0;

public:
    virtual void run(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractSurfaceImageFromImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractSurfaceImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractSurfaceImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractSurfaceImageFromImage, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractSurfaceImageFromImage, GNOMONCORE_EXPORT, surfaceImageFromImage);
}

//
// gnomonAbstractForm.h ends here
