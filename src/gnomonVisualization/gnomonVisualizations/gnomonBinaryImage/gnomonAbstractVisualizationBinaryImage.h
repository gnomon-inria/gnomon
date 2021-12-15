#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationBinaryImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationBinaryImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationBinaryImage(void) = default;

public:
    virtual void setBinaryImage(gnomonBinaryImageSeries *image) = 0;
    virtual gnomonBinaryImageSeries *binaryImage(void) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const dtkCoreParameters&) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationBinaryImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT, visualizationBinaryImage);
}

//
// gnomonAbstractVisualizationBinaryImage.h ends here
