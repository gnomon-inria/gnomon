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

#include <gnomonCoreExport.h>

#include <dtkCore>

#include "gnomonAbstractModel.h"

class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonAbstractEvolutionModel : public gnomonAbstractModel
{
public:
    virtual ~gnomonAbstractEvolutionModel(void) = default;

public:
    virtual void reset(void) = 0;
    virtual void step(std::size_t time, std::size_t dt) = 0;
    virtual void run(std::size_t timeMin, std::size_t timeMax, std::size_t dt) = 0;

public:
//    virtual QMap<QString, gnomonAbstractForm *> forms() = 0;
    virtual gnomonAbstractForm * form(void) = 0;
};

DTK_DECLARE_OBJECT        (gnomonAbstractEvolutionModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT, evolutionModel);
}

//
// gnomonAbstractEvolution.h ends here
