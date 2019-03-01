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
#include "gnomonActorMeshCellComplex.h"

#include <gnomonForm/gnomonCellComplex>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkAssembly.h>
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
// gnomonActorMeshCellComplexPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellComplexPrivate
{
public:
    gnomonCellComplex *cellcomplex;

    vtkSmartPointer<vtkSphereSource> sphere;

    vtkSmartPointer<vtkPolyData> point_mesh;
    vtkSmartPointer<vtkGlyph3D> point_glyph;
    vtkSmartPointer<vtkPolyDataMapper> point_mapper;
    vtkSmartPointer<vtkActor> point_actor;

    double cellScaleFactor;

    QMap<long, vtkSmartPointer<vtkPolyData>> cell_mesh;
    QMap<long, vtkSmartPointer<vtkPolyDataMapper>> cell_mapper;
    QMap<long, vtkSmartPointer<vtkActor>> cell_actor;

    vtkSmartPointer<vtkAssembly> cell_assembly;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellComplex
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellComplex);

void gnomonActorMeshCellComplex::setCellComplex(gnomonCellComplex *cellcomplex)
{
    dd->cellcomplex = cellcomplex;

    this->update();
}

void gnomonActorMeshCellComplex::update(void)
{
    if(!dd->cellcomplex)
        return;

    if(!d->interactor)
        return;

    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();

    vtkSmartPointer<vtkCellArray> polydataLines = vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkCellArray> polydataFaces = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkDoubleArray> polydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();

    QMap<long, QVariant> positions_x = dd->cellcomplex->elementProperty(0,"barycenter_x");
    QMap<long, QVariant> positions_y = dd->cellcomplex->elementProperty(0,"barycenter_y");
    QMap<long, QVariant> positions_z = dd->cellcomplex->elementProperty(0,"barycenter_z");

    QMap<long,long> vertexPoint;

    QList<long> vertices = dd->cellcomplex->elementIds(0);

    for (const auto& vertexId : vertices) {
        long vtkId = polydataPoints->InsertNextPoint(positions_x[vertexId].value<double>(),positions_y[vertexId].value<double>(),positions_z[vertexId].value<double>());
        vertexPoint[vertexId] = vtkId;
        polydataPointData->InsertValue(vtkId,vertexId);
    }

    if(!dd->point_mesh) {
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

    if(!dd->point_glyph) {
        dd->point_glyph = vtkSmartPointer<vtkGlyph3D>::New();
        dd->point_glyph->SetScaleModeToDataScalingOff();
        dd->point_glyph->SetColorModeToColorByScalar();
        dd->point_glyph->SetSourceData(dd->sphere->GetOutput());
        dd->point_glyph->SetInputData(dd->point_mesh);
        dd->point_glyph->Update();
    }

    if(!dd->point_mapper) {
        dd->point_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        dd->point_mapper->SetInputData(dd->point_glyph->GetOutput());
        dd->point_mapper->SetScalarRange(0, dd->cellcomplex->elementCount(0)-1);
    }

    if(!dd->point_actor) {
        dd->point_actor = vtkSmartPointer<vtkActor>::New();
        dd->point_actor ->SetMapper(dd->point_mapper);
        dd->point_actor->SetVisibility(0);
        this->AddPart(dd->point_actor);
    }

    QList<long> faces = dd->cellcomplex->elementIds(2);

    for (const auto& faceId : faces) {
        QList<long> faceVertices = dd->cellcomplex->orientedFaceVertexIds(faceId);
        long vtkId = polydataFaces->InsertNextCell(faceVertices.size());
        for (const auto& v : faceVertices) {
            polydataFaces->InsertCellPoint(vertexPoint[v]);
        }
        polydataFaceData->InsertValue(vtkId,faceId);
    }

    if(!d->mesh) {
        d->mesh = vtkSmartPointer<vtkPolyData>::New();
        d->mesh->SetPoints(polydataPoints);
        d->mesh->SetPolys(polydataFaces);
        d->mesh->GetCellData()->SetScalars(polydataFaceData);
    }

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(d->mesh);
        d->mapper->SetScalarRange(0, dd->cellcomplex->elementCount(2)-1);
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);
        d->actor->SetVisibility(0);
        this->AddPart(d->actor);
    }

    QMap<long, QMap<long,long> > cellVertexPoints;

    QMap<long, QVariant> cell_centers_x = dd->cellcomplex->elementProperty(3,"barycenter_x");
    QMap<long, QVariant> cell_centers_y = dd->cellcomplex->elementProperty(3,"barycenter_y");
    QMap<long, QVariant> cell_centers_z = dd->cellcomplex->elementProperty(3,"barycenter_z");

    QList<long> cells = dd->cellcomplex->elementIds(3);

    for (const auto& cellId : cells) {

        if (!dd->cell_mesh.contains(cellId)) {
            dd->cell_mesh[cellId] = vtkSmartPointer<vtkPolyData>::New();

            vtkSmartPointer<vtkPoints> cellPolydataPoints = vtkSmartPointer<vtkPoints>::New();
            QList<long> cellVertices = dd->cellcomplex->incidentElementIds(3,cellId,0);
            cellVertexPoints[cellId] = QMap<long,long>();

            for (const auto& vertexId : cellVertices) {
                double x = cell_centers_x[cellId].value<double>() + dd->cellScaleFactor * (positions_x[vertexId].value<double>() - cell_centers_x[cellId].value<double>());
                double y = cell_centers_y[cellId].value<double>() + dd->cellScaleFactor * (positions_y[vertexId].value<double>() - cell_centers_y[cellId].value<double>());
                double z = cell_centers_z[cellId].value<double>() + dd->cellScaleFactor * (positions_z[vertexId].value<double>() - cell_centers_z[cellId].value<double>());
                long vtkId = cellPolydataPoints->InsertNextPoint(x,y,z);
                cellVertexPoints[cellId][vertexId] = vtkId;
            }

            vtkSmartPointer<vtkCellArray> cellPolydataFaces = vtkSmartPointer<vtkCellArray>::New();
            vtkSmartPointer<vtkDoubleArray> cellPolydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();

            QList<long> cellFaces = dd->cellcomplex->incidentElementIds(3,cellId,2);
            for (const auto& faceId : cellFaces) {
                QList<long> faceVertices = dd->cellcomplex->orientedFaceVertexIds(faceId);
                long vtkId = cellPolydataFaces->InsertNextCell(faceVertices.size());
                for (const auto& v : faceVertices) {
                    cellPolydataFaces->InsertCellPoint(cellVertexPoints[cellId][v]);
                }
                cellPolydataFaceData->InsertValue(vtkId,cellId);
            }

            dd->cell_mesh[cellId]->SetPoints(cellPolydataPoints);
            dd->cell_mesh[cellId]->SetPolys(cellPolydataFaces);
            dd->cell_mesh[cellId]->GetCellData()->SetScalars(cellPolydataFaceData);
        }

        if(!dd->cell_mapper.contains(cellId)) {
            dd->cell_mapper[cellId] = vtkSmartPointer<vtkPolyDataMapper>::New();
            dd->cell_mapper[cellId]->SetInputData(dd->cell_mesh[cellId]);
            dd->cell_mapper[cellId]->SetScalarRange(0, dd->cellcomplex->elementCount(3)-1);
        }

        if(!dd->cell_actor.contains(cellId)) {
            dd->cell_actor[cellId] = vtkSmartPointer<vtkActor>::New();
            dd->cell_actor[cellId]->SetMapper(dd->cell_mapper[cellId]);
        }
    }

    if(!dd->cell_assembly) {
        dd->cell_assembly = vtkSmartPointer<vtkAssembly>::New();
        for (const auto& cellId : cells) {
            dd->cell_assembly->AddPart(dd->cell_actor[cellId]);
        }
        this->AddPart(dd->cell_assembly);
    }

    d->interactor->Render();
}

gnomonActorMeshCellComplex::gnomonActorMeshCellComplex(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellComplexPrivate)
{
    dd->cellcomplex = Q_NULLPTR;

    dd->cellScaleFactor = 0.99;
}

gnomonActorMeshCellComplex::~gnomonActorMeshCellComplex(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
