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

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorCellGraph : public gnomonInspector
{
public:
     gnomonInspectorCellGraph(void);
    ~gnomonInspectorCellGraph(void);

private:
    gnomonInspectorCellGraph(const gnomonInspectorCellGraph&) = delete;
    void operator = (const gnomonInspectorCellGraph&) = delete;

public:
    gnomonClutEditor *editor(void) const;

 private:
    class gnomonInspectorCellGraphPrivate *d;
};

//
// gnomonInspectorCellGraph.h ends here
