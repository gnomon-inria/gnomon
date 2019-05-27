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
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationCellComplex : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationCellComplex(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellComplex(void) = default;

public:
	virtual void setCellComplex(gnomonCellComplexSeries *cellComplex) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;

public slots:
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