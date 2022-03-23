#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

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
DTK_DECLARE_PLUGIN_FACTORY(gnomonFormVisitor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonFormVisitor, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonFormVisitor, GNOMONCORE_EXPORT, formVisitor);
}

//
// gnomonFormVisitor.h ends here
