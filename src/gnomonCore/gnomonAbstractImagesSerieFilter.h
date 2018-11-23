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
class gnomonParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImagesSerieFilter
{
public:
    virtual ~gnomonAbstractImagesSerieFilter(void) = default;

public:
    virtual void setInput(gnomonImagesSerie *images_serie) = 0;
    virtual gnomonImagesSerie *input() = 0;

    virtual gnomonImagesSerie *output() = 0;

    virtual QMap<QString, gnomonParameter*> parameters(void) = 0;
    virtual void setParameter(const QString&, const QVariant&) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual QMap<QString, QVariant> parameters(void) const = 0;
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
