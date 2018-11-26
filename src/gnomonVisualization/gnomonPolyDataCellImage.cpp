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
#include "gnomonPolyDataCellImage.h"

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
#include <vtkQuadricDecimation.h>
#include <vtkCenterOfMass.h>
#include <vtkImageResample.h>

// /////////////////////////////////////////////////////////////////
// gnomonPolyDataCellImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonPolyDataCellImagePrivate
{
public:
    gnomonCellImage *cellimage;

    double cellScaleFactor;
    double resamplingSpacing;
    double smoothingFactor;
    double decimationFactor;

    QMap<long, vtkSmartPointer<vtkPolyData> > cell_mesh;
    vtkSmartPointer<vtkPolyData> mesh;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonPolyDataCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonPolyDataCellImage);


void gnomonPolyDataCellImage::setCellImage(gnomonCellImage *cellimage)
{
    d->cellimage = cellimage;

    this->modified();
    this->update();
}

void gnomonPolyDataCellImage::modified(void)
{
    d->modified = true;
}

void gnomonPolyDataCellImage::update(void)
{
    if(!d->cellimage)
        return;

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if(!converter)
        return;

    dtkImage *image = d->cellimage->image();
    converter->setInput(image);
    if(!converter->convert())
        return;

    vtkImageData *volume = static_cast<vtkImageData *>(converter->output());

    float v_x = volume->GetSpacing()[0];
    float v_y = volume->GetSpacing()[1];
    float v_z = volume->GetSpacing()[2];

    float r_x = floor(d->resamplingSpacing/v_x);
    float r_y = floor(d->resamplingSpacing/v_y);
    float r_z = floor(d->resamplingSpacing/v_z);

    vtkSmartPointer<vtkImageResample> resample = vtkSmartPointer<vtkImageResample>::New();
    resample->SetInputData(volume);
    // resample->SetOutputSpacing(d->resamplingVoxelsize,d->resamplingVoxelsize,d->resamplingVoxelsize);
    resample->SetMagnificationFactors(1/r_x,1/r_y,1/r_z);
    resample->SetInterpolationModeToNearestNeighbor();
    resample->Update();

    QStringList cellProperties = d->cellimage->cellPropertyNames();
    if (!cellProperties.contains("volume")) {
        d->cellimage->computeCellProperty("volume");
    }

    QMap<long, QVariant> cellVolumes = d->cellimage->cellProperty("volume");

    if (d->modified)
        d->cell_mesh.clear();

    QList<long> cells = d->cellimage->cellIds();
    for (const auto& cellId : cells) {

        if (!d->cell_mesh.contains(cellId)) {

            vtkSmartPointer<vtkDiscreteMarchingCubes>contour = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
            contour->SetInputData(resample->GetOutput());
            contour->ComputeNormalsOn();
            contour->ComputeGradientsOn();
            contour->SetValue(0,cellId);
            contour->Update();
            // qDebug()<<"Cell "<<cellId<<" marching cubes : "<<contour->GetOutput()->GetNumberOfCells()<<" faces";
 

            if (contour->GetOutput()->GetNumberOfCells()>0)
            { 
                int smooth_iterations = int(d->smoothingFactor*8);

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

                // int divisions = int(pow(cellVolumes[cellId].value<double>(),1/3.)*d->resolutionFactor);
                // int divisions = 5.*d->resolutionFactor;

                vtkSmartPointer<vtkQuadricDecimation> decimate = vtkSmartPointer<vtkQuadricDecimation>::New();
                decimate->SetInputData(smoother->GetOutput());
                // decimate->SetFeaturePointsAngle(120.0);
                decimate->VolumePreservationOff();
                decimate->SetTargetReduction(1. - 1/d->decimationFactor);
                decimate->Update();

                d->cell_mesh[cellId] = decimate->GetOutput();
     
                double center[3];
                vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
                centerOfMassFilter->SetInputData(d->cell_mesh[cellId]);
                centerOfMassFilter->SetUseScalarsAsWeights(false);
                centerOfMassFilter->Update();
                centerOfMassFilter->GetCenter(center);

                for (int vtkId=0;vtkId<d->cell_mesh[cellId]->GetNumberOfPoints();vtkId++) {
                    double point[3];
                    d->cell_mesh[cellId]->GetPoints()->GetPoint(vtkId,point);
                    for (int i=0;i<3;i++)
                        point[i] = center[i] + d->cellScaleFactor*(point[i]-center[i]);
                    d->cell_mesh[cellId]->GetPoints()->SetPoint(vtkId,point);
                }

                vtkSmartPointer<vtkDoubleArray> cellPolydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();
                for (int vtkId=0;vtkId<d->cell_mesh[cellId]->GetNumberOfCells();vtkId++) {
                    cellPolydataFaceData->InsertValue(vtkId,cellId);
                }
                d->cell_mesh[cellId]->GetCellData()->SetScalars(cellPolydataFaceData);

                d->cell_mesh[cellId] = d->cell_mesh[cellId];
            }
        }
    }

    vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();
    for (const auto& cellId : cells)
        if (d->cell_mesh.contains(cellId))
            appender->AddInputData(d->cell_mesh[cellId]);

    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaner->SetInputConnection(appender->GetOutputPort());
    cleaner->Update();

    d->mesh = cleaner->GetOutput();

    this->SetPoints(d->mesh->GetPoints());
    this->SetPolys(d->mesh->GetPolys());
    this->GetCellData()->SetScalars(d->mesh->GetCellData()->GetScalars());
    
    d->modified = false;
}

gnomonPolyDataCellImage::gnomonPolyDataCellImage(void) : gnomonPolyData(), d(new gnomonPolyDataCellImagePrivate)
{
    d->cellimage = Q_NULLPTR;

    d->cellScaleFactor = 1.;
    d->resamplingSpacing = 1.5;
    d->smoothingFactor = 1.;
    d->decimationFactor = 1.;
}

gnomonPolyDataCellImage::~gnomonPolyDataCellImage(void)
{
    delete d;

    d = NULL;
}

//
// gnomonPolyData.cpp ends here
