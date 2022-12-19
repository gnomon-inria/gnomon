#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractMeshFromImage : public gnomonAbstractAlgorithm
{
public:
  virtual void setInput(std::shared_ptr<gnomonImageSeries> images) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonMeshSeries> output() const = 0;
    virtual std::shared_ptr<gnomonImageSeries> input() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
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

DTK_DECLARE_OBJECT(gnomonAbstractMeshFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT, meshFromImage);
}

//
// gnomonAbstractMeshFromImage.h ends here
