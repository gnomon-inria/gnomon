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
class gnomonParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImagesRegistration
{
public:
    virtual ~gnomonAbstractImagesRegistration(void) = default;

public:
    virtual QMap<QString, gnomonParameter*> parameters(void) const = 0;
    virtual void setParameter(const QString&, const QVariant&) = 0;

public:
    virtual void addImage(dtkImage *image) = 0;
    virtual void removeImages(void) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual double time(void) = 0;
    virtual dtkImage *at(double t) = 0;
    virtual dtkImage *next(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImagesRegistration *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImagesRegistration, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImagesRegistration, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImagesRegistration, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImagesRegistration, GNOMONCORE_EXPORT, imagesRegistration);
}

//
// gnomonAbstractImagesRegistration.h ends here
