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

#include "gnomonActorMesh.h"
#include "gnomonActorMesh_p.h"
#include "gnomonActorMeshCellGraph.h"

#include <gnomonCellGraph.h>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellGraphPrivate
{
public:
    gnomonCellGraph *cellgraph; 
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraph
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellGraph);


void gnomonActorMeshCellGraph::setCellGraph(gnomonCellGraph *cellgraph)
{
    dd->cellgraph = cellgraph;

    this->update();
}

void gnomonActorMeshCellGraph::update(void)
{
    d->interactor->Render();
}

gnomonActorMeshCellGraph::gnomonActorMeshCellGraph(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellGraphPrivate)
{
    dd->cellgraph = Q_NULLPTR;
}

gnomonActorMeshCellGraph::~gnomonActorMeshCellGraph(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
