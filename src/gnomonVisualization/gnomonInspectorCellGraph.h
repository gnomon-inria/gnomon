// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include "gnomonInspector.h"

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;
class gnomonStringEditor;
class gnomonDoubleRangeEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorCellGraph : public gnomonInspector
{
    Q_OBJECT

public:
     gnomonInspectorCellGraph(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorCellGraph(void);

private:
    gnomonInspectorCellGraph(const gnomonInspectorCellGraph&) = delete;
    void operator = (const gnomonInspectorCellGraph&) = delete;

public:
    gnomonStringEditor *vertexPropertyEditor(void) const;
    gnomonDoubleRangeEditor *sliceEditor(const QString& dim = "x") const;
    gnomonClutEditor *editor(void) const;

public:
	const QString& vertexProperty(void) const;
	double vertexSize(void) const;
	double edgeOpacity(void) const;
	double edgeLinewidth(void) const;
	const QList<double>& slice(const QString& dim = "x") const;

signals:
	void vertexPropertyUpdated(void);
	void vertexSizeUpdated(void);
	void edgeOpacityUpdated(void);
	void edgeLinewidthUpdated(void);
	void xSliceUpdated(void);
	void ySliceUpdated(void);
	void zSliceUpdated(void);

 private:
    class gnomonInspectorCellGraphPrivate *d;
};

//
// gnomonInspectorCellGraph.h ends here
