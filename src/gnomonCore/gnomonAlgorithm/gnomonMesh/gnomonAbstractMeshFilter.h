#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshFilter : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractMeshFilter(void) = default;

public:
    virtual void setInput(std::shared_ptr<gnomonMeshSeries> mesh_series) = 0;
    virtual std::shared_ptr<gnomonMeshSeries> input() = 0;

    virtual std::shared_ptr<gnomonMeshSeries> output() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"input"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonMesh") {
            return {"output"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshFilter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshFilter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshFilter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshFilter, GNOMONCORE_EXPORT, meshFilter);
}

//
// gnomonAbstractMeshFilter.h ends here
