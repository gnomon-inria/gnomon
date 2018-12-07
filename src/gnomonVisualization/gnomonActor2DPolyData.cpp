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

#include "gnomonActor2DPolyData.h"

#include <gnomonPolyData>
#include <gnomonPolyData>

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
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActor2DPolyDataPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DPolyDataPrivate
{
public:
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<gnomonPolyData> polydata;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    QMap<int, vtkSmartPointer<vtkPolyDataMapper> > sliceMappers;
    QMap<int, vtkSmartPointer<vtkActor> > sliceActors;

public:
    double slicePositions[3];
    double sliceThickness;

public:
    int orientation;
    double alpha;

public:
    bool modified;

public slots:
    void updateVisibility(void);
    void updateSlice(int orientation);
    void updateOpacity(void);
};

void gnomonActor2DPolyDataPrivate::updateVisibility(void)
{
    for (int i=0;i<3;i++)
    {
        if (this->orientation == i)
            this->sliceActors[i]->VisibilityOn();
        else
            this->sliceActors[i]->VisibilityOff();
    }
}

void gnomonActor2DPolyDataPrivate::updateSlice(int orientation)
{
    vtkSmartPointer<vtkPlane> topPlane = vtkSmartPointer<vtkPlane>::New();
    if (orientation==0)
    {
        topPlane->SetOrigin(this->slicePositions[0]+this->sliceThickness, 0, 0);
        topPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        topPlane->SetOrigin(0, this->slicePositions[1]+this->sliceThickness, 0);
        topPlane->SetNormal(0, 1, 0);
    }
    else
    {
        topPlane->SetOrigin(0, 0, this->slicePositions[2]+this->sliceThickness);
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
        bottomPlane->SetOrigin(this->slicePositions[0]-this->sliceThickness, 0, 0);
        bottomPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        bottomPlane->SetOrigin(0, this->slicePositions[1]-this->sliceThickness, 0);
        bottomPlane->SetNormal(0, 1, 0);
    }
    else
    {
        bottomPlane->SetOrigin(0, 0, this->slicePositions[2]-this->sliceThickness);
        bottomPlane->SetNormal(0, 0, 1);
    }

    vtkSmartPointer<vtkClipPolyData> bottomClipper = vtkSmartPointer<vtkClipPolyData>::New();
    bottomClipper->SetInputConnection(topClipper->GetOutputPort());
    bottomClipper->SetClipFunction(bottomPlane);
    bottomClipper->SetValue(0);
    bottomClipper->Update();

    double bounds[2] = {0., 1.};
    if (this->polydata->GetCellData()->GetNumberOfArrays()>0)
    {
        this->polydata->GetCellData()->GetArray(0)->GetRange(bounds);
    }
    else if (this->polydata->GetPointData()->GetNumberOfArrays()>0)
    {
        this->polydata->GetPointData()->GetArray(0)->GetRange(bounds);
    }

    if (!this->colorFunction)
    {
        this->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    }
    this->colorFunction->RemoveAllPoints();
    this->colorFunction->AddRGBPoint(bounds[0], 0.25,  0,  0.5);
    this->colorFunction->AddRGBPoint(0.5*(bounds[0] + bounds[1]), 0,  0.75,  0.56);
    this->colorFunction->AddRGBPoint(bounds[1], 1,  1,  0);
    this->colorFunction->ClampingOn();
    this->colorFunction->Modified();


    if (!this->sliceMappers.contains(orientation)) {
        this->sliceMappers[orientation] = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    // vtkSmartPointer<vtkDoubleArray> cellData = (vtkDoubleArray *) this->polydata->GetCellData()->GetArray(0);
    // this->sliceMappers[orientation]->SetScalarRange(cellData->GetRange());
    this->sliceMappers[orientation]->SetLookupTable(this->colorFunction);
    this->sliceMappers[orientation]->SetInputConnection(bottomClipper->GetOutputPort());
    this->sliceMappers[orientation]->Update();

    if(!this->sliceActors.contains(orientation)) {
        this->sliceActors[orientation] = vtkSmartPointer<vtkActor>::New();
        this->sliceActors[orientation]->SetMapper(this->sliceMappers[orientation]);
    }
    this->sliceActors[orientation]->GetProperty()->SetRepresentationToWireframe();
    this->sliceActors[orientation]->GetProperty()->SetLineWidth(2.);
    this->sliceActors[orientation]->Modified();
}

void gnomonActor2DPolyDataPrivate::updateOpacity(void)
{
    for (const auto& orientation : this->sliceActors.keys()) {
        this->sliceActors[orientation]->GetProperty()->SetOpacity(this->alpha);
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DPolyData
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DPolyData);

void gnomonActor2DPolyData::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActor2DPolyData::setPolyData(gnomonPolyData *polydata)
{
    d->polydata = polydata;
    this->modified();
    this->update();
}

void gnomonActor2DPolyData::setSliceThickness(double value)
{
    d->sliceThickness = value;
    this->modified();
}

void gnomonActor2DPolyData::setSlicePositions(double value[3])
{
    for(int i=0;i<3;i++)
        d->slicePositions[i] = value[i];
}

void gnomonActor2DPolyData::modified(void)
{
    d->modified = true;
}

void gnomonActor2DPolyData::update(void)
{
    if(!d->polydata)
        return;

    for (int i=0;i<3;i++)
    {
        qDebug()<<"Update Slice"<<i;
        d->updateSlice(i);
        if (this->GetNumberOfPaths()<=i)
            this->AddPart(d->sliceActors[i]);
    }

    d->updateVisibility();

    d->interactor->Render();

    d->modified = false;
}

void gnomonActor2DPolyData::hide(void)
{
    this->VisibilityOff();
    d->interactor->Render();
}

void gnomonActor2DPolyData::show(void)
{
    this->VisibilityOn();
    d->interactor->Render();
}

void gnomonActor2DPolyData::setSliceOrientation(int value)
{
    d->orientation = value;
    d->updateVisibility();
    d->interactor->Render();
}

void gnomonActor2DPolyData::setSlice(int value)
{
    d->slicePositions[d->orientation] = value;
    d->updateSlice(d->orientation);
    d->interactor->Render();
}

void gnomonActor2DPolyData::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DPolyData::gnomonActor2DPolyData(void) : d(new gnomonActor2DPolyDataPrivate)
{
    d->interactor = Q_NULLPTR;
    d->polydata = Q_NULLPTR;
    d->colorFunction = Q_NULLPTR;

    d->alpha = 1;

    d->orientation = 2;
    d->sliceThickness = 1;
    d->slicePositions[0] = 0;
    d->slicePositions[1] = 0;
    d->slicePositions[2] = 0;
}

gnomonActor2DPolyData::~gnomonActor2DPolyData(void)
{
    delete d;

    d = nullptr;
}

//
// gnomonActor2D.cpp ends here
