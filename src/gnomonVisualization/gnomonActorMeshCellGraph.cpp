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
    if(!dd->cellgraph)
        return;

    qDebug()<<d->interactor;

    if(!d->interactor)
        return;

    qDebug()<<"--> Mesh Cell Graph Update";

    if(!dd->sphere) {
        dd->sphere = vtkSmartPointer<vtkSphereSource>::New();
        dd->sphere->SetRadius(1);
        dd->sphere->SetThetaResolution(12);
        dd->sphere->SetPhiResolution(12);
        dd->sphere->Update();
    }

    qDebug()<<"--> Mesh Cell Graph Sphere";

    if(!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(dd->sphere->GetOutput());
    }

    qDebug()<<"--> Mesh Cell Graph Mapper";

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);

        this->AddPart(d->actor);
    }

    qDebug()<<"--> Mesh Cell Graph Actor";

    d->interactor->Render();
}

gnomonActorMeshCellGraph::gnomonActorMeshCellGraph(void) : gnomonActorMesh(), d(new gnomonActorMeshPrivate), dd(new gnomonActorMeshCellGraphPrivate)
{
    qDebug()<<"--> Actor Cell Graph Create";
    dd->cellgraph = Q_NULLPTR;
    d->mesh = Q_NULLPTR;
}

gnomonActorMeshCellGraph::~gnomonActorMeshCellGraph(void)
{
    delete dd;
    delete d;

    dd = NULL;
    d = NULL;
}

//
// gnomonActorMesh.cpp ends here
