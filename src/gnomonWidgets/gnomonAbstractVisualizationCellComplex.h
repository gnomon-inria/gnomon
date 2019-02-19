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

#include <gnomonWidgetsExport.h>

#include <dtkCore>

#include "gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonCellComplex;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractVisualizationCellComplex : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationCellComplex(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellComplex(void) = default;

public:
	virtual void setCellComplex(gnomonCellComplex *cellComplex) = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellComplex *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellComplex, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellComplex, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellComplex, GNOMONWIDGETS_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonWidgets layer
// /////////////////////////////////////////////////////////////////

namespace gnomonWidgets {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationCellComplex, GNOMONWIDGETS_EXPORT, visualizationCellComplex);
}

//
// gnomonAbstractVisualizationCellComplex.h ends here