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
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkCellData.h>
#include <vtkPointData.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellGraphPrivate
{
public:
    gnomonCellGraph *cellgraph; 

    vtkSmartPointer<vtkSphereSource> sphere;
    vtkSmartPointer<vtkActor> point_actor;
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

    if(!d->interactor)
        return;

    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polydataLines = vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();

    QMap<long, QVariant> positions_x = dd->cellgraph->vertexProperty("barycenter_x");
    QMap<long, QVariant> positions_y = dd->cellgraph->vertexProperty("barycenter_y");
    QMap<long, QVariant> positions_z = dd->cellgraph->vertexProperty("barycenter_z");

    QMap<long,long> vertexPoint;

    QList<long> vertices = dd->cellgraph->vertexIds();

    for (const auto& vertexId : vertices) {
        long vtkId = polydataPoints->InsertNextPoint(positions_x[vertexId].value<double>(),positions_y[vertexId].value<double>(),positions_z[vertexId].value<double>());
        vertexPoint[vertexId] = vtkId;
        polydataPointData->InsertValue(vtkId,vertexId);
    }

    QList<long> edges = dd->cellgraph->edgeIds();

    for (const auto& edgeId : edges) {
        QList<long> edgeVertices = dd->cellgraph->edgeVertexIds(edgeId);
        long vtkId = polydataLines->InsertNextCell(edgeVertices.size());
        for (const auto& v : edgeVertices) {
            polydataLines->InsertCellPoint(vertexPoint[v]);
        }
    }

    if (!d->mesh) {
        d->mesh = vtkSmartPointer<vtkPolyData>::New();
        d->mesh->SetPoints(polydataPoints);
        d->mesh->SetLines(polydataLines);
    }

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(d->mesh);
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);
        this->AddPart(d->actor);
    }

    vtkSmartPointer<vtkPolyData> pointPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointPolydata->SetPoints(polydataPoints);
    pointPolydata->GetPointData()->SetScalars(polydataPointData);

    if(!dd->sphere) {
        dd->sphere = vtkSmartPointer<vtkSphereSource>::New();
        dd->sphere->SetRadius(1);
        dd->sphere->SetThetaResolution(12);
        dd->sphere->SetPhiResolution(12);
        dd->sphere->Update();
    }

    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetScaleModeToDataScalingOff();
    glyph->SetColorModeToColorByScalar();
    glyph->SetSourceData(dd->sphere->GetOutput());
    glyph->SetInputData(pointPolydata);
    glyph->Update();

    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointMapper->SetInputData(glyph->GetOutput());
    pointMapper->SetScalarRange(0, dd->cellgraph->vertexCount()-1);

    if(!dd->point_actor) {
        dd->point_actor = vtkSmartPointer<vtkActor>::New();
        dd->point_actor ->SetMapper(pointMapper);
        this->AddPart(dd->point_actor);
    }

    qDebug()<<"--> Mesh Cell Graph Actor";

    d->interactor->Render();
}

gnomonActorMeshCellGraph::gnomonActorMeshCellGraph(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellGraphPrivate)
{
    qDebug()<<"--> Actor Cell Graph Create";
    dd->cellgraph = Q_NULLPTR;
    // d->mesh = Q_NULLPTR;
}

gnomonActorMeshCellGraph::~gnomonActorMeshCellGraph(void)
{
    delete dd;
    // delete d;

    dd = NULL;
    // d = NULL;
}

//
// gnomonActorMesh.cpp ends here
