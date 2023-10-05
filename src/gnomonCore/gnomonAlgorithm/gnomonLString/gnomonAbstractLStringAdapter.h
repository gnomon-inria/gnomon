#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormAdapter.h"

#include "gnomonForm/gnomonLString/gnomonLString.h"

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractLStringAdapter
// ///////////////////////////////////////////////////////////////////

typedef gnomonAbstractFormAdapter<gnomonLStringSeries> gnomonAbstractLStringAdapter;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractLStringAdapter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringAdapter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringAdapter, GNOMONCORE_EXPORT, lStringAdapter, gnomonCore)


//
// gnomonAbstractLStringAdapter.h ends here
