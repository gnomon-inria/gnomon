// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

#include <gnomonVisualizations/gnomonAbstractVisualization.h>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationMesh : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationMesh(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationMesh(void) = default;

public:
	virtual void setMesh(gnomonMeshSeries *mesh) = 0;
	virtual gnomonMeshSeries *mesh(void) = 0;

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

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const dtkCoreParameters&) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual QMap<QString, QString> parameterGroups(void) override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
    virtual void clear(void) override = 0;
    virtual void setVisible(bool visible) override = 0;

public slots:
    virtual void on2D(void) override = 0;
    virtual void on3D(void) override = 0;
    virtual void onXY(void) override = 0;
    virtual void onXZ(void) override = 0;
    virtual void onYZ(void) override = 0;
    virtual void onSliceChanged(int) override = 0;
    virtual void onSliceOrientationChanged(int) override = 0;
    virtual void onTimeChanged(double) override = 0;
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
