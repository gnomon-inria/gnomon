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

class gnomonCellImage;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractVisualizationCellImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationCellImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellImage(void) = default;

public:
	virtual void setCellImage(gnomonCellImage *cellImage) = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellImage, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellImage, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellImage, GNOMONWIDGETS_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonWidgets layer
// /////////////////////////////////////////////////////////////////

namespace gnomonWidgets {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationCellImage, GNOMONWIDGETS_EXPORT, visualizationCellImage);
}

//
// gnomonAbstractVisualizationCellImage.h ends here