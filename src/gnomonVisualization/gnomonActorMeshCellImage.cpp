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
#include "gnomonActorMeshCellImage.h"

#include <gnomonCellImage.h>

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
#include <vtkQuadricClustering.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellImagePrivate
{
public:
    gnomonCellImage *cellimage;

    double cellScaleFactor;
    double smoothingFactor;
    double resolutionFactor;

    QMap<long, vtkSmartPointer<vtkPolyData> > cell_mesh;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellImage);


void gnomonActorMeshCellImage::setCellImage(gnomonCellImage *cellimage)
{
    dd->cellimage = cellimage;

    this->modified();
    this->update();
}

void gnomonActorMeshCellImage::modified(void)
{
    dd->modified = true;
}

void gnomonActorMeshCellImage::update(void)
{
    if(!dd->cellimage)
        return;

    if(!d->interactor)
        return;

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if(!converter)
        return;

    dtkImage *image = dd->cellimage->image();
    converter->setInput(image);
    if(!converter->convert())
        return;

    vtkImageData *volume = static_cast<vtkImageData *>(converter->output());


    // QList<long> faces = dd->cellimage->elementIds(2);

    // for (const auto& faceId : faces) {
    //     QList<long> faceVertices = dd->cellimage->orientedFaceVertexIds(faceId);
    //     long vtkId = polydataFaces->InsertNextCell(faceVertices.size());
    //     for (const auto& v : faceVertices) {
    //         polydataFaces->InsertCellPoint(vertexPoint[v]);
    //     }
    //     polydataFaceData->InsertValue(vtkId,faceId);
    // }

    // if (!d->mesh) {
    //     d->mesh = vtkSmartPointer<vtkPolyData>::New();
    //     d->mesh->SetPoints(polydataPoints);
    //     d->mesh->SetPolys(polydataFaces);
    //     d->mesh->GetCellData()->SetScalars(polydataFaceData);
    // }

    // if (!d->mapper) {
    //     d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //     d->mapper->SetInputData(d->mesh);
    //     d->mapper->SetScalarRange(0, dd->cellimage->elementCount(2)-1);
    // }

    // if(!d->actor) {
    //     d->actor = vtkSmartPointer<vtkActor>::New();
    //     d->actor->SetMapper(d->mapper);
    //     d->actor->SetVisibility(0);
    //     this->AddPart(d->actor);
    // }


    // QMap<long, QMap<long,long> > cellVertexPoints;

    QStringList cellProperties = dd->cellimage->cellPropertyNames();
    if (!cellProperties.contains("volume")) {
        dd->cellimage->computeCellProperty("volume");
    }

    QMap<long, QVariant> cellVolumes = dd->cellimage->cellProperty("volume");

    QList<long> cells = dd->cellimage->cellIds();

    for (const auto& cellId : cells) {

        if ((!dd->cell_mesh.contains(cellId)) | (dd->modified)) {

            vtkSmartPointer<vtkDiscreteMarchingCubes>contour = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
            contour->SetInputData(volume);
            contour->ComputeNormalsOn();
            contour->ComputeGradientsOn();
            contour->SetValue(0,cellId);
            contour->Update();

            qDebug()<<"Cell "<<cellId<<" marching cubes : "<<contour->GetOutput()->GetNumberOfCells()<<" faces";

            int smooth_iterations = int(dd->smoothingFactor*8);

            vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoother = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
            smoother->SetInputData(contour->GetOutput());
            smoother->BoundarySmoothingOn();
            smoother->FeatureEdgeSmoothingOn();
            smoother->SetFeatureAngle(120.0);
            smoother->SetPassBand(0.01);
            smoother->SetNumberOfIterations(smooth_iterations);
            smoother->NonManifoldSmoothingOn();
            smoother->NormalizeCoordinatesOn();
            smoother->Update();

            // int divisions = int(pow(cellVolumes[cellId].value<double>(),1/3.)*dd->resolutionFactor);
            int divisions = 5.*dd->resolutionFactor;

            vtkSmartPointer<vtkQuadricClustering> decimate = vtkSmartPointer<vtkQuadricClustering>::New();
            decimate->SetInputData(smoother->GetOutput());
            decimate->SetNumberOfDivisions(divisions,divisions,divisions);
            decimate->SetFeaturePointsAngle(120.0);
            decimate->Update();

            dd->cell_mesh[cellId] = decimate->GetOutput();

            vtkSmartPointer<vtkDoubleArray> cellPolydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();
            for (int vtkId=0;vtkId<dd->cell_mesh[cellId]->GetNumberOfCells();vtkId++) {
                cellPolydataFaceData->InsertValue(vtkId,cellId);
            }

            dd->cell_mesh[cellId]->GetCellData()->SetScalars(cellPolydataFaceData);
        }
    }

    vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();
    for (const auto& cellId : cells)
        appender->AddInputData(dd->cell_mesh[cellId]);

    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaner->SetInputConnection(appender->GetOutputPort());
    cleaner->Update();

    d->mesh = cleaner->GetOutput();

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetScalarRange(0, dd->cellimage->cellCount()-1);
    }
    d->mapper->SetInputData(d->mesh);

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);
        this->AddPart(d->actor);
    }
    d->actor->Modified();

    d->interactor->Render();
    
    dd->modified = false;
}

gnomonActorMeshCellImage::gnomonActorMeshCellImage(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellImagePrivate)
{
    dd->cellimage = Q_NULLPTR;

    dd->cellScaleFactor = 0.9;
    dd->smoothingFactor = 1.0;
    dd->resolutionFactor = 2.0;
}

gnomonActorMeshCellImage::~gnomonActorMeshCellImage(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
