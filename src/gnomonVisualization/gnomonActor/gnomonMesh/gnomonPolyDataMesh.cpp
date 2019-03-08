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

#include "gnomonActor/gnomonPolyData/gnomonPolyData.h"
#include "gnomonPolyDataMesh.h"

#include <gnomonCore>

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkAssembly.h>
#include <vtkCommand.h>
#include <vtkCleanPolyData.h>
#include <vtkImageData.h>
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
#include <vtkDiscreteMarchingCubes.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkCenterOfMass.h>
#include <vtkImageResample.h>

// /////////////////////////////////////////////////////////////////
// gnomonPolyDataMeshPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPolyDataMeshPrivate
{
public:
    gnomonMesh *mesh;

    QString property_name;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonPolyDataMesh
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonPolyDataMesh);


void gnomonPolyDataMesh::setMesh(gnomonMesh *mesh)
{
    d->mesh = mesh;

    this->modified();
}

void gnomonPolyDataMesh::setPropertyName(const QString& property_name)
{
    d->property_name = property_name;

    this->modified();
}

void gnomonPolyDataMesh::modified(void)
{
    d->modified = true;
}

void gnomonPolyDataMesh::update(void)
{
    if(!d->mesh)
        return;

    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkCellArray> polydataFaces = vtkSmartPointer<vtkCellArray>::New();

    QMap<long, QVariant> positions_x = d->mesh->vertexProperty("barycenter_x");
    QMap<long, QVariant> positions_y = d->mesh->vertexProperty("barycenter_y");
    QMap<long, QVariant> positions_z = d->mesh->vertexProperty("barycenter_z");

    QMap<long,long> vertexPoint;

    QList<long> vertices = d->mesh->vertexIds();
    double max_vertices = *std::max_element(vertices.begin(), vertices.end());

    QMap<long, QVariant> vertexProperty;
    if (d->mesh->vertexPropertyNames().contains(d->property_name)) {
        vertexProperty = d->mesh->vertexProperty(d->property_name);
    } else {
        for (const auto& vertexId : vertices) {
            // vertexProperty[vertexId] = QVariant((double)vertexId);
            vertexProperty[vertexId] = QVariant((double)max_vertices);
        }
    }

    QMap<long, double> vertexScalarProperty;
    for (const auto& vertexId : vertices) {
        vertexScalarProperty[vertexId] = vertexProperty[vertexId].value<double>();
    }

    QList<double> vertexScalarPropertyValues = vertexScalarProperty.values();
    auto mm = std::minmax_element(vertexScalarPropertyValues.begin(),vertexScalarPropertyValues.end());

    for (const auto& vertexId : vertices) {
        long vtkId = polydataPoints->InsertNextPoint(positions_x[vertexId].value<double>(),positions_y[vertexId].value<double>(),positions_z[vertexId].value<double>());
        polydataPointData->InsertValue(vtkId,vertexScalarProperty[vertexId]);
        vertexPoint[vertexId] = vtkId;
    }

    this->SetPoints(polydataPoints);
    this->GetPointData()->SetScalars(polydataPointData);

    QList<long> triangles = d->mesh->triangleIds();

    for (const auto& triangleId : triangles) {
      QList<long> triangleVertices = d->mesh->triangleVertexIds(triangleId);
      long vtkId = polydataFaces->InsertNextCell(triangleVertices.size());
      for (const auto& v : triangleVertices) {
          polydataFaces->InsertCellPoint(vertexPoint[v]);
      }
    }

    this->SetPolys(polydataFaces);

    d->modified = false;
}

gnomonPolyDataMesh::gnomonPolyDataMesh(void) : gnomonPolyData(), d(new gnomonPolyDataMeshPrivate)
{
    d->mesh = Q_NULLPTR;

    d->property_name = "";
}

gnomonPolyDataMesh::~gnomonPolyDataMesh(void)
{
    delete d;

    d = NULL;
}

//
// gnomonPolyData.cpp ends here
