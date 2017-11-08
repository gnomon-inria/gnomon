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
#include <vtkSphereSource.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellGraphPrivate
{
public:
    gnomonCellGraph *cellgraph; 
    
    vtkSmartPointer<vtkSphereSource> sphere;
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

    if(!dd->sphere) {
        dd->sphere = vtkSmartPointer<vtkSphereSource>::New();
        dd->sphere->SetRadius(1);
        dd->sphere->SetThetaResolution(12);
        dd->sphere->SetPhiResolution(12);
        dd->sphere->Update();
    }

    if(!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(dd->sphere->GetOutput());
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);

        this->AddPart(d->actor);
    }

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
