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

class gnomonImagesSerie;
class gnomonCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImagesRegistration
{
public:
    virtual ~gnomonAbstractImagesRegistration(void) = default;

public:
    virtual QMap<QString, gnomonCoreParameter*> parameters(void) const = 0;
    virtual void setParameter(const QString&, const QVariant&) = 0;

public:
    virtual void addImagesSerie(gnomonImagesSerie *images_serie) = 0;
    virtual void removeImagesSeries(void) = 0;

    virtual gnomonImagesSerie* output() = 0;

public:
    virtual void run(void) = 0;
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
