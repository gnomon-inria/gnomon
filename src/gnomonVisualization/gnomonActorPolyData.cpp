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

#include "gnomonActorPolyData.h"

#include <gnomonPolyDataCellImage>
#include <gnomonCellImage>

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActorPolyDataPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorPolyDataPrivate
{
public:
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    vtkSmartPointer<vtkColorTransferFunction> colorFunction;
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;

    vtkRenderWindowInteractor *interactor;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorPolyData
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorPolyData);

void gnomonActorPolyData::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorPolyData::setPolyData(gnomonPolyData *polydata)
{
    d->polydata = polydata;

    this->modified();
    this->update();
}

void gnomonActorPolyData::modified(void)
{
    d->modified = true;
}

void gnomonActorPolyData::update(void)
{
    if(!d->polydata)
        return;

    double bounds[2] = {0., 1.};
    if (d->polydata->GetCellData()->GetNumberOfArrays()>0)
    {
        d->polydata->GetCellData()->GetArray(0)->GetRange(bounds);
    }
    else if (d->polydata->GetPointData()->GetNumberOfArrays()>0)
    {
        d->polydata->GetPointData()->GetArray(0)->GetRange(bounds);
    }

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    d->mapper->SetInputData(d->polydata);
    qDebug()<<"PolyData"<<d->polydata->GetNumberOfPoints();
    
    if (!d->colorFunction)
    {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    }
    d->colorFunction->RemoveAllPoints();
    d->colorFunction->AddRGBPoint(bounds[0], 0.25,  0,  0.5);
    d->colorFunction->AddRGBPoint(0.5*(bounds[0] + bounds[1]), 0,  0.75,  0.56);
    d->colorFunction->AddRGBPoint(bounds[1], 1,  1,  0);
    d->colorFunction->ClampingOn();
    d->colorFunction->Modified();

    d->mapper->SetLookupTable(d->colorFunction);
    d->mapper->Update();

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        this->AddPart(d->actor);
    }
    d->actor->SetMapper(d->mapper);
    d->actor->Modified();
    
    d->modified = false;
}

gnomonActorPolyData::gnomonActorPolyData(void) : gnomonActor(), d(new gnomonActorPolyDataPrivate)
{
    d->polydata = Q_NULLPTR;
    d->mapper = Q_NULLPTR;
    d->actor = Q_NULLPTR;
    d->interactor = Q_NULLPTR;
}

gnomonActorPolyData::~gnomonActorPolyData(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActorPolyData.cpp ends here
