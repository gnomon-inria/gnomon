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

#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

class gnomonViewMatplotlib;

class gnomonDataFrame;
class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualizationDataFrame : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationDataFrame(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationDataFrame(void) = default;

public:
	virtual void setDataFrame(gnomonDataFrame *dataFrame) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) override = 0;
    virtual void setParameters(const QMap<QString, dtkCoreParameter *>&) override = 0;
    virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;

public:
    virtual QImage imageRendering(void) override = 0;

public slots:
    virtual void update(void) override = 0;
    virtual void render(void) override = 0;
    virtual void clear(void) override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationDataFrame *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT, matplotlibVisualizationDataFrame);
}

//
// gnomonAbstractMatplotlibVisualizationDataFrame.h ends here