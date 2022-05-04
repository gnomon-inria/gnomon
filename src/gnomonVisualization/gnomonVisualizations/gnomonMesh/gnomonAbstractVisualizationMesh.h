#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

#include <gnomonVisualizations/gnomonAbstractVisualization.h>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationMesh : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationMesh(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationMesh(void) = default;

public:
	virtual void setMesh(std::shared_ptr<gnomonMeshSeries> mesh) = 0;
	virtual std::shared_ptr<gnomonMeshSeries> mesh(void) = 0;

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
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationMesh *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationMesh, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationMesh, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationMesh, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationMesh, GNOMONVISUALIZATION_EXPORT, visualizationMesh);
}

//
// gnomonAbstractVisualizationMesh.h ends here
