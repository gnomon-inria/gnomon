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
#include "gnomonPolyData.h"

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
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

    vtkRenderWindowInteractor *interactor;

    double alpha;
    double value_range[2];
    QMap<double,QColor> colormap;

    bool modified = true;

public slots:
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActorPolyDataPrivate::updateOpacity(void)
{
    this->actor->GetProperty()->SetOpacity(this->alpha);
}

void gnomonActorPolyDataPrivate::updateColorFunction(void)
{
    if (!this->colorFunction)
        return;

    this->colorFunction->RemoveAllPoints();
    for (const auto& val : this->colormap.keys()) {
        double node = val*this->value_range[1] + (1-val)*this->value_range[0];
        this->colorFunction->AddRGBPoint(node, this->colormap[val].red()/255., this->colormap[val].green()/255., this->colormap[val].blue()/255.);
    }

    this->colorFunction->ClampingOn();
    this->colorFunction->Modified();
}

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

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    d->mapper->SetInputData(d->polydata);
    
    if (!d->colorFunction)
    {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    }
    d->updateColorFunction();

    d->mapper->SetLookupTable(d->colorFunction);
    d->mapper->Update();

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        this->AddPart(d->actor);
    }
    d->actor->SetMapper(d->mapper);
    d->actor->Modified();

    d->updateOpacity();
    
    d->modified = false;
}

void gnomonActorPolyData::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

void gnomonActorPolyData::setValueRange(const QList<double>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActorPolyData::setColorMap(const QMap<double,QColor>& value)
{
    d->colormap = value;
    d->updateColorFunction();
    d->interactor->Render();
}

gnomonActorPolyData::gnomonActorPolyData(void) : gnomonActor(), d(new gnomonActorPolyDataPrivate)
{
    d->polydata = Q_NULLPTR;
    d->mapper = Q_NULLPTR;
    d->actor = Q_NULLPTR;
    d->interactor = Q_NULLPTR;

    d->alpha = 1;
    d->value_range[0] = 0.;
    d->value_range[1] = 1.;
    d->colormap = QMap<double, QColor>({
        {0., QColor(0, 0, 0, 255)},
        {1., QColor(255, 255, 255, 255)} });
}

gnomonActorPolyData::~gnomonActorPolyData(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActorPolyData.cpp ends here
