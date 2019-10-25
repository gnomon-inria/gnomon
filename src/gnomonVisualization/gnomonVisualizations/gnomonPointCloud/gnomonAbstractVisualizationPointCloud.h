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

#include <dtkCore>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonPointCloud;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationPointCloud : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationPointCloud(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationPointCloud(void) = default;

public:
	virtual void setPointCloud(gnomonPointCloudSeries *PointCloud) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
    virtual void clear(void) override = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationPointCloud *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT, visualizationPointCloud);
}

//
// gnomonAbstractVisualizationPointCloud.h ends here