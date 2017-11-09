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

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

#include "gnomonActor.h"
#include "gnomonActorMesh.h"

class gnomonCellGraph;

class GNOMONVISUALIZATION_EXPORT gnomonActorMeshCellGraph : public gnomonActorMesh
{
public:
    vtkTypeMacro(gnomonActorMeshCellGraph, vtkAssembly);

    static gnomonActorMeshCellGraph *New(void);

public:
	void setCellGraph(gnomonCellGraph *cellgraph);

public slots:
    void update(void) override;

protected:
     gnomonActorMeshCellGraph(void);
    ~gnomonActorMeshCellGraph(void);

private:
    gnomonActorMeshCellGraph(const gnomonActorMeshCellGraph&);
    void operator = (const gnomonActorMeshCellGraph&);

private:
	class gnomonActorMeshCellGraphPrivate *dd;
};


//
// gnomonActorMeshCellGraph.h ends here