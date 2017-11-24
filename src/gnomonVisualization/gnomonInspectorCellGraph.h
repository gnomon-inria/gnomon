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

#include <gnomonVisualizationExport.h>

#include <QtWidgets>

class gnomonActorMeshCellGraph;
class gnomonDoubleRangeEditor;
class gnomonStringEditor;
class gnomonColorMapEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorCellGraph : public QFrame
{
    Q_OBJECT

public:
     gnomonInspectorCellGraph(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorCellGraph(void);

 public:
    void setActor(gnomonActorMeshCellGraph *actor) const;
    void updateVertexPropertyRange(gnomonActorMeshCellGraph *actor) const;
    void updateFilterPropertyRange(gnomonActorMeshCellGraph *actor) const;

private:
    gnomonInspectorCellGraph(const gnomonInspectorCellGraph&) = delete;
    void operator = (const gnomonInspectorCellGraph&) = delete;

 private:
    class gnomonInspectorCellGraphPrivate *d;
};

//
// gnomonInspectorCellGraph.h ends here
