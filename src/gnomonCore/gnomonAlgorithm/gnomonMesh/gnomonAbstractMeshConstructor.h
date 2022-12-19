#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractMeshConstructor : public gnomonAbstractAlgorithm
{
public:
    virtual std::shared_ptr<gnomonMeshSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonMesh") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractMeshConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshConstructor, GNOMONCORE_EXPORT, meshConstructor);
}

//
// gnomonAbstractMeshConstructor.h ends here
