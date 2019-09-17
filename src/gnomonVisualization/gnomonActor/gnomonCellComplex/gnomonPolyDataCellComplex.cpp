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
#include "gnomonPolyDataCellComplex.h"

#include <gnomonCore>

//TODO: Image
//#include <dtkImagingCore>

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

    double cellScaleFactor;

    QMap<long, vtkSmartPointer<vtkPolyData> > cell_mesh;
    vtkSmartPointer<vtkPolyData> mesh;

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


void gnomonPolyDataCellComplex::setScaleFactor(double scale_factor)
{
    d->cellScaleFactor = scale_factor;

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

//    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
//    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();
//    vtkSmartPointer<vtkCellArray> polydataFaces = vtkSmartPointer<vtkCellArray>::New();

    QMap<long, QVariant> positions_x = d->cellComplex->elementProperty(0,"barycenter_x");
    QMap<long, QVariant> positions_y = d->cellComplex->elementProperty(0,"barycenter_y");
    QMap<long, QVariant> positions_z = d->cellComplex->elementProperty(0,"barycenter_z");

//    QMap<long,long> vertexPoint;
//
//    QList<long> vertices = d->cellComplex->elementIds(0);
//
//    QMap<long, QVariant> vertexProperty;
//    if (d->cellComplex->elementPropertyNames(0).contains(d->property_name)) {
//        vertexProperty = d->cellComplex->elementProperty(0,d->property_name);
//    } else {
//        for (const auto& vertexId : vertices) {
//            vertexProperty[vertexId] = QVariant((double)vertexId);
//        }
//    }
//
//    QMap<long, double> vertexScalarProperty;
//    for (const auto& vertexId : vertices) {
//        vertexScalarProperty[vertexId] = vertexProperty[vertexId].value<double>();
//    }
//
//    QList<double> vertexScalarPropertyValues = vertexScalarProperty.values();
//    auto mm = std::minmax_element(vertexScalarPropertyValues.begin(),vertexScalarPropertyValues.end());
//
//    for (const auto& vertexId : vertices) {
//        long vtkId = polydataPoints->InsertNextPoint(positions_x[vertexId].value<double>(),positions_y[vertexId].value<double>(),positions_z[vertexId].value<double>());
//        polydataPointData->InsertValue(vtkId,vertexScalarProperty[vertexId]);
//        vertexPoint[vertexId] = vtkId;
//    }
//
//    this->SetPoints(polydataPoints);
//    this->GetPointData()->SetScalars(polydataPointData);
//
//
//    QList<long> faces = d->cellComplex->elementIds(2);
//
//    for (const auto& faceId : faces) {
//      QList<long> faceVertices = d->cellComplex->orientedFaceVertexIds(faceId);
//      long vtkId = polydataFaces->InsertNextCell(faceVertices.size());
//      for (const auto& v : faceVertices) {
//          polydataFaces->InsertCellPoint(vertexPoint[v]);
//      }
//    }
//
//    this->SetPolys(polydataFaces);


    QMap<long, QMap<long,long> > cellVertexPoints;

    QMap<long, QVariant> cell_centers_x = d->cellComplex->elementProperty(3,"barycenter_x");
    QMap<long, QVariant> cell_centers_y = d->cellComplex->elementProperty(3,"barycenter_y");
    QMap<long, QVariant> cell_centers_z = d->cellComplex->elementProperty(3,"barycenter_z");

    QList<long> cells = d->cellComplex->elementIds(3);


    QMap<long, QVariant> cellProperty;
    if (d->cellComplex->elementPropertyNames(3).contains(d->property_name)) {
        cellProperty = d->cellComplex->elementProperty(3,d->property_name);
    } else {
        for (const auto& cellId : cells) {
            cellProperty[cellId] = QVariant((double)cellId);
        }
    }

    QMap<long, double> cellScalarProperty;
    for (const auto& cellId : cells) {
        cellScalarProperty[cellId] = cellProperty[cellId].value<double>();
    }


    for (const auto& cellId : cells) {

        if (!d->cell_mesh.contains(cellId)) {
            d->cell_mesh[cellId] = vtkSmartPointer<vtkPolyData>::New();

            vtkSmartPointer<vtkPoints> cellPolydataPoints = vtkSmartPointer<vtkPoints>::New();
            vtkSmartPointer<vtkCellArray> cellPolydataFaces = vtkSmartPointer<vtkCellArray>::New();
            vtkSmartPointer<vtkDoubleArray> cellPolydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();

            QList<long> cellVertices = d->cellComplex->incidentElementIds(3,cellId,0);
            cellVertexPoints[cellId] = QMap<long,long>();

            for (const auto& vertexId : cellVertices) {
                double x = cell_centers_x[cellId].value<double>() + d->cellScaleFactor * (positions_x[vertexId].value<double>() - cell_centers_x[cellId].value<double>());
                double y = cell_centers_y[cellId].value<double>() + d->cellScaleFactor * (positions_y[vertexId].value<double>() - cell_centers_y[cellId].value<double>());
                double z = cell_centers_z[cellId].value<double>() + d->cellScaleFactor * (positions_z[vertexId].value<double>() - cell_centers_z[cellId].value<double>());
                long vtkId = cellPolydataPoints->InsertNextPoint(x,y,z);
                cellVertexPoints[cellId][vertexId] = vtkId;
            }


            QList<long> cellFaces = d->cellComplex->incidentElementIds(3,cellId,2);
            for (const auto& faceId : cellFaces) {
                QList<long> faceVertices = d->cellComplex->orientedFaceVertexIds(faceId);
                long vtkId = cellPolydataFaces->InsertNextCell(faceVertices.size());
                for (const auto& v : faceVertices) {
                    cellPolydataFaces->InsertCellPoint(cellVertexPoints[cellId][v]);
                }
                cellPolydataFaceData->InsertValue(vtkId,cellScalarProperty[cellId]);
            }

            d->cell_mesh[cellId]->SetPoints(cellPolydataPoints);
            d->cell_mesh[cellId]->SetPolys(cellPolydataFaces);
            d->cell_mesh[cellId]->GetCellData()->SetScalars(cellPolydataFaceData);
        }
    }

    vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();
    for (const auto& cellId : cells)
        if (d->cell_mesh.contains(cellId))
            appender->AddInputData(d->cell_mesh[cellId]);
    appender->Update();
    // d->mesh = appender->GetOutput();

    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    // cleaner->ConvertPolysToLinesOff();
    // cleaner->ConvertLinesToPointsOff();
    cleaner->SetInputConnection(appender->GetOutputPort());
    cleaner->Update();
    d->mesh = cleaner->GetOutput();

    this->SetPoints(d->mesh->GetPoints());
    this->SetPolys(d->mesh->GetPolys());
    this->GetCellData()->SetScalars(d->mesh->GetCellData()->GetScalars());

    d->modified = false;
}

gnomonPolyDataCellComplex::gnomonPolyDataCellComplex(void) : gnomonPolyData(), d(new gnomonPolyDataCellComplexPrivate)
{
    d->cellComplex = Q_NULLPTR;

    d->cellScaleFactor = 0.99;
    d->property_name = "";
}

gnomonPolyDataCellComplex::~gnomonPolyDataCellComplex(void)
{
    delete d;

    d = NULL;
}

//
// gnomonPolyData.cpp ends here
