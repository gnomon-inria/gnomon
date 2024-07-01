#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class GNOMONCORE_EXPORT gnomonAbstractFemSolver : public gnomonAbstractAlgorithm
{

public:
    virtual void setInputMesh(std::shared_ptr<gnomonMeshSeries> mesh) = 0;
    virtual std::shared_ptr<gnomonMeshSeries> inputMesh(void) const = 0;

public:
    virtual std::shared_ptr<gnomonMeshSeries> updatedMesh(void) const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"setInputMesh"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonMesh") {
            return {"inputMesh"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonMesh") {
            return {"updatedMesh"};
        }
        return {};
    };

};

DTK_DECLARE_OBJECT        (gnomonAbstractFemSolver *)
DTK_DECLARE_PLUGIN        (gnomonAbstractFemSolver, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractFemSolver, GNOMONCORE_EXPORT, femSolver, gnomonCore)


//
// gnomonAbstractFemSolver.h ends here
