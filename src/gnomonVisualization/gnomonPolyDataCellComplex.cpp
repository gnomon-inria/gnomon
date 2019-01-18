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

#include "gnomonPolyData.h"
#include "gnomonPolyDataCellComplex.h"

#include <gnomonCellComplex.h>

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
// gnomonPolyDataCellComplexPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPolyDataCellComplexPrivate
{
public:
    gnomonCellComplex *cellComplex;

    QString property_name;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonPolyDataCellComplex
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonPolyDataCellComplex);


void gnomonPolyDataCellComplex::setCellComplex(gnomonCellComplex *cellComplex)
{
    d->cellComplex = cellComplex;

    this->modified();
}

void gnomonPolyDataCellComplex::setPropertyName(const QString& property_name)
{
    d->property_name = property_name;

    this->modified();
}

void gnomonPolyDataCellComplex::modified(void)
{
    d->modified = true;
}

void gnomonPolyDataCellComplex::update(void)
{
    if(!d->cellComplex)
        return;

    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkCellArray> polydataFaces = vtkSmartPointer<vtkCellArray>::New();

    QMap<long, QVariant> positions_x = d->cellComplex->elementProperty(0,"barycenter_x");
    QMap<long, QVariant> positions_y = d->cellComplex->elementProperty(0,"barycenter_y");
    QMap<long, QVariant> positions_z = d->cellComplex->elementProperty(0,"barycenter_z");

    QMap<long,long> vertexPoint;

    QList<long> vertices = d->cellComplex->elementIds(0);

    QMap<long, QVariant> vertexProperty;
    if (d->cellComplex->elementPropertyNames(0).contains(d->property_name)) {
        vertexProperty = d->cellComplex->elementProperty(0,d->property_name);
    } else {
        for (const auto& vertexId : vertices) {
            vertexProperty[vertexId] = QVariant((double)vertexId);
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


    QList<long> faces = d->cellComplex->elementIds(2);

    for (const auto& faceId : faces) {
      QList<long> faceVertices = d->cellComplex->orientedFaceVertexIds(faceId);
      long vtkId = polydataFaces->InsertNextCell(faceVertices.size());
      for (const auto& v : faceVertices) {
          polydataFaces->InsertCellPoint(vertexPoint[v]);
      }
    }

    this->SetPolys(polydataFaces);

    d->modified = false;
}

gnomonPolyDataCellComplex::gnomonPolyDataCellComplex(void) : gnomonPolyData(), d(new gnomonPolyDataCellComplexPrivate)
{
    d->cellComplex = Q_NULLPTR;

    d->property_name = "";
}

gnomonPolyDataCellComplex::~gnomonPolyDataCellComplex(void)
{
    delete d;

    d = NULL;
}

//
// gnomonPolyData.cpp ends here
