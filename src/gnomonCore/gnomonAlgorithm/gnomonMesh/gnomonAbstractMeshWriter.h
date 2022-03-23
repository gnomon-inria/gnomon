#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractMeshWriter(void) = default;

public:
    virtual void setMesh(gnomonMeshSeries *mesh) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"setMesh"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"mesh"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshWriter, GNOMONCORE_EXPORT, meshWriter);
}

//
// gnomonAbstractImageWriter.h ends here
