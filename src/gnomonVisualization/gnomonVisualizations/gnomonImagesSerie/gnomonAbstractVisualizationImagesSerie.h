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

#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonImagesSerie;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationImagesSerie : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationImagesSerie(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationImagesSerie(void) = default;

public:
	virtual void setImagesSerie(gnomonImagesSerie *cellComplex) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationImagesSerie *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationImagesSerie, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationImagesSerie, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationImagesSerie, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationImagesSerie, GNOMONVISUALIZATION_EXPORT, visualizationImagesSerie);
}

//
// gnomonAbstractVisualizationImagesSerie.h ends here