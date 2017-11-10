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
#include <vtkColorTransferFunction.h>
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

    vtkSmartPointer<vtkPolyData> point_mesh;
    vtkSmartPointer<vtkGlyph3D> point_glyph;
    vtkSmartPointer<vtkPolyDataMapper> point_mapper;
    vtkSmartPointer<vtkActor> point_actor;

public:
    double range_min;
    double range_max;
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
        // qDebug()<<vtkId<<":"<<vertexId<<" ("<<positions_x[vertexId].value<double>()<<","<<positions_y[vertexId].value<double>()<<","<<positions_z[vertexId].value<double>()<<")";
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
    d->actor->Modified();


    if (!dd->point_mesh) {
        dd->point_mesh = vtkSmartPointer<vtkPolyData>::New();
        dd->point_mesh->SetPoints(polydataPoints);
        dd->point_mesh->GetPointData()->SetScalars(polydataPointData);
    }

    if(!dd->sphere) {
        dd->sphere = vtkSmartPointer<vtkSphereSource>::New();
        dd->sphere->SetRadius(1);
        dd->sphere->SetThetaResolution(12);
        dd->sphere->SetPhiResolution(12);
        dd->sphere->Update();
    }

    if (!dd->point_glyph) {
        dd->point_glyph = vtkSmartPointer<vtkGlyph3D>::New();
        dd->point_glyph->SetScaleModeToDataScalingOff();
        dd->point_glyph->SetColorModeToColorByScalar();
        dd->point_glyph->SetSourceData(dd->sphere->GetOutput());
        dd->point_glyph->SetInputData(dd->point_mesh);
        dd->point_glyph->Update();
    }
    dd->point_glyph->Modified();

    double valuesRange[2];
    dd->point_mesh->GetPointData()->GetScalars()->GetRange(valuesRange);

    dd->range_min = valuesRange[0];
    dd->range_max = valuesRange[1];

    double min = valuesRange[0];
    double max = valuesRange[1];
    double mid = (min + max)/2.;

    if(!d->colorFunction) {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->colorFunction->SetColorSpaceToRGB();
        d->colorFunction->RemoveAllPoints();
        d->colorFunction->AddRGBPoint(min, 0.0, 0.0, 1.0);
        d->colorFunction->AddRGBPoint(mid, 0.0, 1.0, 0.0);
        d->colorFunction->AddRGBPoint(max, 1.0, 0.0, 0.0);
        d->colorFunction->ClampingOn();
    }
    d->colorFunction->Modified();

    if (!dd->point_mapper) {
        dd->point_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        dd->point_mapper->SetInputData(dd->point_glyph->GetOutput());
        dd->point_mapper->SetScalarRange(0, dd->cellgraph->vertexCount()-1);
    }
    dd->point_mapper->SetLookupTable(d->colorFunction);
    dd->point_mapper->Modified();

    if(!dd->point_actor) {
        dd->point_actor = vtkSmartPointer<vtkActor>::New();
        dd->point_actor ->SetMapper(dd->point_mapper);
        this->AddPart(dd->point_actor);
    }
    dd->point_actor->Modified();

    d->interactor->Render();
}

double gnomonActorMeshCellGraph::rangeMin() const
{
    return dd->range_min;
}

double gnomonActorMeshCellGraph::rangeMax() const
{
    return dd->range_max;
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
