#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractFormWriter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractFormWriter(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractFormWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractFormWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractFormWriter, GNOMONCORE_EXPORT, formWriter, gnomonCore)


//
// gnomonAbstractFormWriter.h ends here
