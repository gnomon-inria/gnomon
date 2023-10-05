#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormAdapter.h"

#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractCellComplexAdapter
// ///////////////////////////////////////////////////////////////////

typedef gnomonAbstractFormAdapter<gnomonCellComplexSeries> gnomonAbstractCellComplexAdapter;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractCellComplexAdapter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellComplexAdapter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexAdapter, GNOMONCORE_EXPORT, cellComplexAdapter, gnomonCore)


//
// gnomonAbstractCellComplexAdapter.h ends here
