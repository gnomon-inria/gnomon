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

#include <cstddef>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractSystemScenario
{
public:
    virtual ~gnomonAbstractSystemScenario(void) = default;

public:
    virtual void step(std::size_t time, std::size_t dt) = 0;
    virtual void reset(void) {qWarning() << Q_FUNC_INFO << "reset is not implemented";};
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractSystemScenario *)
DTK_DECLARE_PLUGIN        (gnomonAbstractSystemScenario, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractSystemScenario, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractSystemScenario, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractSystemScenario, GNOMONCORE_EXPORT, abstractSystemScenario);
}

//
// gnomonAbstractSystemScenario.h ends here
