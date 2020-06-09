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

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class gnomonMesh;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshAdapter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractMeshAdapter(void) = default;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonMeshSeries *mesh_series) = 0;
    virtual gnomonMeshSeries *input() = 0;

    virtual gnomonAbstractDynamicForm *output() = 0;

public:
    virtual QString target(void) = 0;
    
public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshAdapter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT, meshAdapter);
}

//
// gnomonAbstractMeshAdapter.h ends here
