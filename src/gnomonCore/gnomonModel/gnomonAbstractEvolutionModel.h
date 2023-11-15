#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAbstractModel.h"

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>

class GNOMONCORE_EXPORT gnomonAbstractEvolutionModel : public gnomonAbstractModel
{
public:
    virtual ~gnomonAbstractEvolutionModel(void) = default;

public:
    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms() = 0;
    virtual void setForm(QString, std::shared_ptr<gnomonMeshSeries>) = 0;
    virtual void setForm(QString, std::shared_ptr<gnomonCellComplexSeries>) = 0;
};

DTK_DECLARE_OBJECT        (gnomonAbstractEvolutionModel *)
DTK_DECLARE_PLUGIN        (gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractEvolutionModel, GNOMONCORE_EXPORT, evolutionModel, gnomonCore)


//
// gnomonAbstractEvolution.h ends here
