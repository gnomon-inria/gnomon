#pragma once

#include <dtkCore>

#include <gnomonCoreExport>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonMesh/gnomonMesh.h"
class dtkCoreParameter;

//  ///////////////////////////////////////////////////////////////////
//
//  ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractFemSolver : public gnomonAbstractAlgorithm
{
public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
	virtual void setMesh(gnomonMeshSeries *mesh) = 0;

public:
    virtual gnomonMeshSeries *updatedMesh(void) const = 0;

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
        if(formName == "gnomonMesh") {
            return {"updatedMesh"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractFemSolver *)
DTK_DECLARE_PLUGIN        (gnomonAbstractFemSolver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractFemSolver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractFemSolver, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractFemSolver, GNOMONCORE_EXPORT, femSolver);
}

//
// gnomonAbstractFemSolver.h ends here
