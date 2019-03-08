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

class gnomonMesh;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationMesh : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationMesh(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationMesh(void) = default;

public:
	virtual void setMesh(gnomonMesh *cellComplex) = 0;

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