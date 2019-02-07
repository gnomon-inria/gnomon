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

#include "gnomonActorImageVolume.h"

#include <dtkImagingCore>

#include <QtCore>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>


// /////////////////////////////////////////////////////////////////
// gnomonActorImageVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorImageVolumePrivate
{
public:
    vtkSmartPointer<vtkImageData> image;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper;
    vtkSmartPointer<vtkVolume> volume;

    vtkSmartPointer<vtkColorTransferFunction> colorFunction = nullptr;
    vtkSmartPointer<vtkPiecewiseFunction> opacity = nullptr;

    vtkRenderWindowInteractor *interactor;

    double alpha;
    int value_range[2];
    QMap<double,QColor> colormap;

    bool flat_rendering;
    int background_value;

    bool modified;

public slots:
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActorImageVolumePrivate::updateOpacity(void)
{
    if (!this->opacity)
        return;

    this->opacity->RemoveAllPoints();

    if (this->flat_rendering) {
        this->opacity->AddPoint(this->value_range[0],this->alpha);
        this->opacity->AddPoint(this->background_value - 0.5,this->alpha);
        this->opacity->AddPoint(this->background_value,0.00);
        this->opacity->AddPoint(this->background_value + 0.5,this->alpha);
        this->opacity->AddPoint(this->value_range[1],this->alpha);
    } else {
        this->opacity->AddPoint(this->value_range[0],0.00);
        this->opacity->AddPoint(this->value_range[1],this->alpha);
    }
}

void gnomonActorImageVolumePrivate::updateColorFunction(void)
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
// gnomonActorImageVolume
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorImageVolume);

void gnomonActorImageVolume::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorImageVolume::setImage(vtkImageData *image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActorImageVolume::modified(void)
{
    d->modified = true;
}

void gnomonActorImageVolume::update(void)
{
    if(!d->image)
        return;

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    d->updateColorFunction();

    if (!d->opacity)
        d->opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    d->updateOpacity();

    if(!d->volume_mapper)
        d->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    d->volume_mapper->SetInputData(d->image);
    d->volume_mapper->SetRequestedRenderModeToRayCast();
    d->volume_mapper->Modified();
    d->volume_mapper->Update();

    if(!d->volume) {
        d->volume = vtkSmartPointer<vtkVolume>::New();
        this->AddPart(d->volume);
    }

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(d->opacity);
    property->SetColor(d->colorFunction);
    property->ShadeOff();

    if (d->flat_rendering) {
        property->SetInterpolationType(VTK_NEAREST_INTERPOLATION);
    } else {
        property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    }

    d->volume->SetMapper(d->volume_mapper);
    d->volume->SetProperty(property);
    d->volume->Modified();
    d->volume->Update();

    d->updateOpacity();
    
    d->modified = false;
}

void gnomonActorImageVolume::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

void gnomonActorImageVolume::setValueRange(const QList<int>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateOpacity();
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActorImageVolume::setColorMap(const QMap<double,QColor>& value)
{
    d->colormap = value;
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActorImageVolume::setFlatRendering(bool value)
{
    d->flat_rendering = value;
    d->updateOpacity();

    if (d->volume) {
        vtkSmartPointer<vtkVolumeProperty> property = d->volume->GetProperty();
        if (d->flat_rendering) {
            property->SetInterpolationType(VTK_NEAREST_INTERPOLATION);
        } else {
            property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
        }
    }

    d->interactor->Render();
}

gnomonActorImageVolume::gnomonActorImageVolume(void) : gnomonActor(), d(new gnomonActorImageVolumePrivate)
{
    d->image = Q_NULLPTR;
    d->volume_mapper = Q_NULLPTR;
    d->volume = Q_NULLPTR;
    d->interactor = Q_NULLPTR;

    d->colorFunction = Q_NULLPTR;
    d->opacity = Q_NULLPTR;

    d->alpha = 1;
    d->flat_rendering = false;
    d->background_value = 1;
    d->value_range[0] = 0.;
    d->value_range[1] = 1.;
    d->colormap = QMap<double, QColor>({
        {0., QColor(0, 0, 0, 255)},
        {1., QColor(255, 255, 255, 255)} });
}

gnomonActorImageVolume::~gnomonActorImageVolume(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActorImageVolume.cpp ends here
