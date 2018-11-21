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

class GNOMONCORE_EXPORT gnomonAbstractImagesFusion
{
public:
    virtual ~gnomonAbstractImagesFusion(void) = default;

public:
    virtual QMap<QString, gnomonParameter*> parameters(void) const = 0;

public:
    virtual void addImage(dtkImage *image) = 0;
    virtual void removeImages(void) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual dtkImage *output() = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImagesFusion *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImagesFusion, GNOMONCORE_EXPORT, imagesFusion);
}

//
// gnomonAbstractImagesFusion.h ends here
