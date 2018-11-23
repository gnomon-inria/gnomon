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

class dtkImage;
class gnomonMesh;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshReader
{
public:
    virtual ~gnomonAbstractMeshReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonMesh *mesh(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshReader, GNOMONCORE_EXPORT, meshReader);
}

//
// gnomonAbstractMeshReader.h ends here
