#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonFormVisitor
{
public:
             gnomonFormVisitor(void) = default;
    virtual ~gnomonFormVisitor(void) = default;

public:
    virtual void visit(class gnomonAbstractForm*) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonFormVisitor *)
DTK_DECLARE_PLUGIN        (gnomonFormVisitor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonFormVisitor, GNOMONCORE_EXPORT, formVisitor, gnomonCore)


//
// gnomonFormVisitor.h ends here
