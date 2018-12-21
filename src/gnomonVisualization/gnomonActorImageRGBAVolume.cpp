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

#include "gnomonActorImageRGBAVolume.h"

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
// gnomonActorImageRGBAVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorImageRGBAVolumePrivate
{
public:
    vtkSmartPointer<vtkImageData> image;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper;
    vtkSmartPointer<vtkVolume> volume;

    vtkRenderWindowInteractor *interactor;

    double alpha;

    bool modified;

public slots:
    void updateOpacity(void);
};

void gnomonActorImageRGBAVolumePrivate::updateOpacity(void)
{
    // qDebug()<<Q_FUNC_INFO<<this->alpha;
}

// /////////////////////////////////////////////////////////////////
// gnomonActorImageRGBAVolume
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorImageRGBAVolume);

void gnomonActorImageRGBAVolume::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorImageRGBAVolume::setImage(vtkImageData *image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActorImageRGBAVolume::modified(void)
{
    d->modified = true;
}

void gnomonActorImageRGBAVolume::update(void)
{
    if(!d->image)
        return;

    if(!d->volume_mapper)
        d->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    d->volume_mapper->SetInputData(d->image);
    d->volume_mapper->SetBlendModeToComposite();
    // d->volume_mapper->SetRequestedRenderModeToRayCast();
    d->volume_mapper->SetRequestedRenderModeToDefault();
    d->volume_mapper->Modified();
    d->volume_mapper->Update();

    if(!d->volume) {
        d->volume = vtkSmartPointer<vtkVolume>::New();
        this->AddPart(d->volume);
    }

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->IndependentComponentsOff();
    property->ShadeOff();
    property->SetInterpolationTypeToNearest();

    d->volume->SetMapper(d->volume_mapper);
    d->volume->SetProperty(property);
    d->volume->Modified();
    d->volume->Update();

    d->updateOpacity();
    
    d->modified = false;
}

void gnomonActorImageRGBAVolume::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActorImageRGBAVolume::gnomonActorImageRGBAVolume(void) : gnomonActor(), d(new gnomonActorImageRGBAVolumePrivate)
{
    d->image = Q_NULLPTR;
    d->volume_mapper = Q_NULLPTR;
    d->volume = Q_NULLPTR;
    d->interactor = Q_NULLPTR;

    d->alpha = 1;
}

gnomonActorImageRGBAVolume::~gnomonActorImageRGBAVolume(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActorImageRGBAVolume.cpp ends here
