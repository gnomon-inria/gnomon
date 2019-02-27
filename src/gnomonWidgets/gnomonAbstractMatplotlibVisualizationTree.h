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

#include "gnomonAbstractMatplotlibVisualization.h"

class gnomonViewMatplotlib;

class gnomonTree;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonAbstractMatplotlibVisualizationTree : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationTree(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationTree(void) = default;

public:
	virtual void setTree(gnomonTree *tree) = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationTree *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationTree, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationTree, GNOMONWIDGETS_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationTree, GNOMONWIDGETS_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonWidgets layer
// /////////////////////////////////////////////////////////////////

namespace gnomonWidgets {
    DTK_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationTree, GNOMONWIDGETS_EXPORT, matplotlibVisualizationTree);
}

//
// gnomonAbstractMatplotlibVisualizationTree.h ends here