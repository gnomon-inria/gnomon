#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonVisualizations/gnomonAbstractVtkVisualization.h>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMeshVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMeshVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractMeshVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractMeshVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshVtkVisualization, GNOMONVISUALIZATION_EXPORT, meshVtkVisualization, gnomonVisualization)


//
// gnomonAbstractMeshVtkVisualization.h ends here
