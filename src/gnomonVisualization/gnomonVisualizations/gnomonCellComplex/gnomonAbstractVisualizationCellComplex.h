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

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonCellComplex;
class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationCellComplex : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
             gnomonAbstractVisualizationCellComplex(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellComplex(void) = default;

public:
	virtual void setCellComplex(gnomonCellComplexSeries *cellComplex) = 0;
	virtual gnomonCellComplexSeries *cellComplex(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"setCellComplex"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"cellComplex"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellComplex *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT, visualizationCellComplex);
}

//
// gnomonAbstractVisualizationCellComplex.h ends here
