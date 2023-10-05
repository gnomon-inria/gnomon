#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormAdapter.h"

#include "gnomonForm/gnomonTree/gnomonTree.h"

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractTreeAdapter
// ///////////////////////////////////////////////////////////////////

typedef gnomonAbstractFormAdapter<gnomonTreeSeries> gnomonAbstractTreeAdapter;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeAdapter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeAdapter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeAdapter, GNOMONCORE_EXPORT, treeAdapter, gnomonCore)


//
// gnomonAbstractTreeAdapter.h ends here
