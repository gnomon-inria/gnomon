#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormAdapter.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractMeshAdapter
// ///////////////////////////////////////////////////////////////////

typedef gnomonAbstractFormAdapter<gnomonMeshSeries> gnomonAbstractMeshAdapter;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshAdapter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshAdapter, GNOMONCORE_EXPORT, meshAdapter, gnomonCore)


//
// gnomonAbstractMeshAdapter.h ends here
