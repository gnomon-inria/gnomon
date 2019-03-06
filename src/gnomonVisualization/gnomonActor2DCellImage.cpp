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

#include "gnomonActor2DCellImage.h"

#include <gnomonPolyDataCellImage>
#include <gnomonCore>

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActor2DCellImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DCellImagePrivate
{
public:
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<gnomonPolyDataCellImage> polydata;
    QMap<int, vtkSmartPointer<vtkPolyDataMapper> > sliceMappers;
    QMap<int, vtkSmartPointer<vtkActor> > sliceActors;

public:
    int dimension[3];
    double spacing[3];

public:
    int orientation;
    int slicePositions[3];

public:
    bool modified;

public:
    void updateVisibility(void);
    void updateSlice(int orientation);
};

void gnomonActor2DCellImagePrivate::updateVisibility(void)
{
    for (int i=0;i<3;i++)
    {
        if (this->orientation == i)
            this->sliceActors[i]->VisibilityOn();
        else
            this->sliceActors[i]->VisibilityOff();
    }
}

void gnomonActor2DCellImagePrivate::updateSlice(int orientation)
{
    vtkSmartPointer<vtkPlane> topPlane = vtkSmartPointer<vtkPlane>::New();
    if (orientation==0)
    {
        topPlane->SetOrigin((this->slicePositions[0]+0.1)*this->spacing[0], 0, 0);
        topPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        topPlane->SetOrigin(0, (this->slicePositions[1]+0.1)*this->spacing[1], 0);
        topPlane->SetNormal(0, 1, 0);
    }
    else
    {
        topPlane->SetOrigin(0, 0, (this->slicePositions[2]+0.1)*this->spacing[2]);
        topPlane->SetNormal(0, 0, 1);
    }

    vtkSmartPointer<vtkClipPolyData> topClipper = vtkSmartPointer<vtkClipPolyData>::New();
    topClipper->SetInputData(this->polydata);
    topClipper->SetClipFunction(topPlane);
    topClipper->SetValue(0);
    topClipper->InsideOutOn();
    topClipper->Update();

    vtkSmartPointer<vtkPlane> bottomPlane = vtkSmartPointer<vtkPlane>::New();
    if (orientation==0)
    {
        bottomPlane->SetOrigin((this->slicePositions[0]-0.1)*this->spacing[0], 0, 0);
        bottomPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        bottomPlane->SetOrigin(0, (this->slicePositions[1]-0.1)*this->spacing[1], 0);
        bottomPlane->SetNormal(0, 1, 0);
    }
    else
    {
        bottomPlane->SetOrigin(0, 0, (this->slicePositions[2]-0.1)*this->spacing[2]);
        bottomPlane->SetNormal(0, 0, 1);
    }

    vtkSmartPointer<vtkClipPolyData> bottomClipper = vtkSmartPointer<vtkClipPolyData>::New();
    bottomClipper->SetInputConnection(topClipper->GetOutputPort());
    bottomClipper->SetClipFunction(bottomPlane);
    bottomClipper->SetValue(0);
    bottomClipper->Update();

    if (!this->sliceMappers.contains(orientation)) {
        this->sliceMappers[orientation] = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    vtkSmartPointer<vtkDoubleArray> cellData = (vtkDoubleArray *) this->polydata->GetCellData()->GetArray(0);
    this->sliceMappers[orientation]->SetScalarRange(cellData->GetRange());
    this->sliceMappers[orientation]->SetInputConnection(bottomClipper->GetOutputPort());
    this->sliceMappers[orientation]->Update();

    if(!this->sliceActors.contains(orientation)) {
        this->sliceActors[orientation] = vtkSmartPointer<vtkActor>::New();
        this->sliceActors[orientation]->SetMapper(this->sliceMappers[orientation]);
    }
    this->sliceActors[orientation]->GetProperty()->SetRepresentationToWireframe();
    this->sliceActors[orientation]->GetProperty()->SetOpacity(0.9);
    this->sliceActors[orientation]->GetProperty()->SetLineWidth(2.);
    this->sliceActors[orientation]->Modified();
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DCellImage);

void gnomonActor2DCellImage::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActor2DCellImage::setPolyData(gnomonPolyDataCellImage *polydata)
{
    d->polydata = polydata;
    this->modified();
    this->update();
}

void gnomonActor2DCellImage::setDimensions(int value[3])
{
    for(int i=0;i<3;i++)
        d->dimension[i] = value[i];
    this->modified();
}

void gnomonActor2DCellImage::setSpacing(double value[3])
{
    for(int i=0;i<3;i++)
        d->spacing[i] = value[i];
    this->modified();
}

void gnomonActor2DCellImage::setSlicePositions(int value[3])
{
    for(int i=0;i<3;i++)
        d->slicePositions[i] = value[i];
}

void gnomonActor2DCellImage::modified(void)
{
    d->modified = true;
}

void gnomonActor2DCellImage::update(void)
{
    if(!d->polydata)
        return;

    if (d->modified)
    {
        d->slicePositions[0] = d->dimension[0]/2;
        d->slicePositions[1] = d->dimension[1]/2;
        d->slicePositions[2] = d->dimension[2]/2;
    }

    for (int i=0;i<3;i++)
    {
        d->updateSlice(i);
        if (this->GetNumberOfPaths()<=i)
            this->AddPart(d->sliceActors[i]);
    }

    d->updateVisibility();

    d->interactor->Render();

    d->modified = false;
}

void gnomonActor2DCellImage::hide(void)
{
    this->VisibilityOff();
    d->interactor->Render();
}

void gnomonActor2DCellImage::show(void)
{
    this->VisibilityOn();
    d->interactor->Render();
}

void gnomonActor2DCellImage::setSliceOrientation(int orientation)
{
    d->orientation = orientation;
    d->updateVisibility();
    d->interactor->Render();
}

void gnomonActor2DCellImage::setSlice(int position)
{
    d->slicePositions[d->orientation] = position;
    d->updateSlice(d->orientation);
    d->interactor->Render();
}

gnomonActor2DCellImage::gnomonActor2DCellImage(void) : d(new gnomonActor2DCellImagePrivate)
{
    d->interactor = Q_NULLPTR;
    d->polydata = Q_NULLPTR;

    d->orientation = 2;
    d->slicePositions[0] = 0;
    d->slicePositions[1] = 0;
    d->slicePositions[2] = 0;
}

gnomonActor2DCellImage::~gnomonActor2DCellImage(void)
{
    delete d;

    d = NULL;
}

//
// gnomonActor2D.cpp ends here
