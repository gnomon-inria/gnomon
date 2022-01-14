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

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonCellImage;
class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationCellImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationCellImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellImage(void) = default;

public:
	virtual void setCellImage(gnomonCellImageSeries *cellImage) = 0;
	virtual gnomonCellImageSeries *cellImage(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
        }
        return {};
    };

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

public:
    virtual long cellId(long vtkId) = 0;
};

// /////////////////    //////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT, visualizationCellImage);
}

//
// gnomonAbstractVisualizationCellImage.h ends here