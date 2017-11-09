// Version: $Id: 3001ea2e52782d6caaca0c3322d1b10590dba293 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonActorVolume.h"

#include <vtkAbstractVolumeMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkConfigure.h>
#include <vtkDoubleArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkOutlineCornerFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkImageData.h>
#include <vtkImageResize.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProp3DCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkTextProperty.h>
#include <vtkScalarsToColors.h>
#include <vtkScalarBarActor.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorVolumePrivate
{
public:
    vtkImageData *volume;

public:
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<vtkVolume> vol;
    vtkSmartPointer<vtkVolumeProperty> volProperty;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;
    vtkSmartPointer<vtkSmartVolumeMapper> mapper;

    vtkSmartPointer<vtkImageActor> planes[3];
    int plane_index[3];
    vtkSmartPointer<vtkImageMapToColors> colors;

    vtkSmartPointer<vtkScalarBarActor> scalarBar;

    vtkSmartPointer<vtkOutlineCornerFilter> outline_corner;
    vtkSmartPointer<vtkOutlineFilter> outline_box;
    vtkSmartPointer<vtkOutlineFilter> outline_contour;

    vtkSmartPointer<vtkPolyDataMapper> outline_corner_mapper;
    vtkSmartPointer<vtkPolyDataMapper> outline_box_mapper;
    vtkSmartPointer<vtkPolyDataMapper> outline_contour_mapper;

    vtkSmartPointer<vtkActor> outline_corner_actor;
    vtkSmartPointer<vtkActor> outline_box_actor;
    vtkSmartPointer<vtkActor> outline_contour_actor;

public:
    vtkImageResize *filter;

public:
    bool scalarbar_state;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorVolume
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorVolume);

void gnomonActorVolume::setVolume(vtkImageData *volume)
{
    d->volume = volume;

    this->update();
}

void gnomonActorVolume::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
    
}

void gnomonActorVolume::update(void)
{
    if(!d->volume)
        return;

    if (!d->interactor)
        return;

    if(!d->mapper)
        d->mapper = vtkSmartVolumeMapper::New();

    d->mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);
    d->mapper->SetInputData(d->volume);
    d->mapper->Modified();
    d->mapper->Update();

    double valuesRange[2]; d->volume->GetPointData()->GetScalars()->GetRange(valuesRange);

    double min = valuesRange[0];
    double max = valuesRange[1];
    double mid = (min + max)/2.;

    if(!d->colorFunction) {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->colorFunction->SetColorSpaceToRGB();
        d->colorFunction->RemoveAllPoints();
        d->colorFunction->AddRGBPoint(min, 0.0, 0.0, 1.0);
        d->colorFunction->AddRGBPoint(mid, 0.0, 1.0, 0.0);
        d->colorFunction->AddRGBPoint(max, 1.0, 0.0, 0.0);
    }
    d->colorFunction->Modified();

    if(!d->opacityTransferFunction) {
        d->opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
        d->opacityTransferFunction->RemoveAllPoints();
        d->opacityTransferFunction->AddPoint(min, 0.1);
        d->opacityTransferFunction->AddPoint(mid, 0.1);
        d->opacityTransferFunction->AddPoint(max, 0.1);
    }
    d->opacityTransferFunction->Modified();

    if(!d->volProperty) {
        d->volProperty = vtkSmartPointer<vtkVolumeProperty>::New();
        d->volProperty->SetSpecular(1.0);
    }
    d->volProperty->SetScalarOpacity(d->opacityTransferFunction);
    d->volProperty->SetColor(d->colorFunction);
    d->volProperty->Modified();

    if(!d->vol) {
        d->vol = vtkVolume::New();
        d->vol->SetMapper(d->mapper);
        d->vol->SetProperty(d->volProperty);
        this->AddPart(d->vol);
    }
    d->vol->Modified();

    if (!d->colors) {
        d->colors = vtkSmartPointer<vtkImageMapToColors>::New();
        d->colors->SetInputData(d->volume);
        d->colors->SetLookupTable(d->colorFunction);
        d->colors->Update();
    }

    int index = 0;
    int x_min, x_max, y_min, y_max, z_min, z_max;
    double voxeslize[3];
    d->volume->GetExtent(x_min, x_max, y_min, y_max, z_min, z_max);
    d->volume->GetSpacing(voxeslize);
    for (int i = 0; i < 3; ++i) {

        if(!d->planes[i]) {
            d->planes[i] = vtkSmartPointer<vtkImageActor>::New();
            // d->planes[i]->SetInteractor(d->interactor);
            index = d->volume->GetDimensions()[i]/2;
            d->plane_index[i] = index;
        }
        else index = d->plane_index[i];

        d->planes[i]->SetInputData(d->colors->GetOutput());

        if (i == 0) {
            d->planes[i]->SetDisplayExtent(index, index, y_min, y_max, z_min, z_max);
        } else if (i == 1) {
            d->planes[i]->SetDisplayExtent(x_min, x_max, index, index, z_min, z_max);
        } else if (i ==2) {
            d->planes[i]->SetDisplayExtent(x_min, x_max, y_min, y_max, index, index);
        }
        this->AddPart(d->planes[i]);
    }


    { // Building corner outline actor

        if(!d->outline_corner)
            d->outline_corner = vtkSmartPointer<vtkOutlineCornerFilter>::New();
        d->outline_corner->SetInputData(d->volume);
        d->outline_corner->Update();

        if(!d->outline_corner_mapper) {
            d->outline_corner_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            d->outline_corner_mapper->SetInputConnection(d->outline_corner->GetOutputPort());
        }

        if(!d->outline_corner_actor) {
            d->outline_corner_actor = vtkSmartPointer<vtkActor>::New();
            d->outline_corner_actor->SetMapper(d->outline_corner_mapper);
            d->outline_corner_actor->GetProperty()->SetColor(1, 0, 0);
            d->outline_corner_actor->SetVisibility(1);
            this->AddPart(d->outline_corner_actor);
        }
    }

    { // Building outline box actor

        if(!d->outline_box)
            d->outline_box = vtkSmartPointer<vtkOutlineFilter>::New();
        d->outline_box->SetInputData(d->volume);
        d->outline_box->Update();

        if(!d->outline_box_mapper) {
            d->outline_box_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            d->outline_box_mapper->SetInputConnection(d->outline_box->GetOutputPort());
        }

        if(!d->outline_box_actor) {
            d->outline_box_actor = vtkSmartPointer<vtkActor>::New();
            d->outline_box_actor->SetMapper(d->outline_box_mapper);
            d->outline_box_actor->GetProperty()->SetColor(1, 0, 0);
            d->outline_box_actor->SetVisibility(0);
            this->AddPart(d->outline_box_actor);
        }
    }

    { // Building outline contour actor

        if(!d->outline_contour)
            d->outline_contour = vtkSmartPointer<vtkOutlineFilter>::New();
        d->outline_contour->SetInputData(d->volume);
        d->outline_contour->Update();

        if(!d->outline_contour_mapper) {
            d->outline_contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            d->outline_contour_mapper->SetInputConnection(d->outline_contour->GetOutputPort());
        }

        if(!d->outline_contour_actor) {
            d->outline_contour_actor = vtkSmartPointer<vtkActor>::New();
            d->outline_contour_actor->SetMapper(d->outline_contour_mapper);
            d->outline_contour_actor->GetProperty()->SetColor(1, 0, 0);
            d->outline_contour_actor->SetVisibility(0);
            this->AddPart(d->outline_contour_actor);
        }
    }

    if(!d->scalarBar) {
        d->scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        d->scalarBar->SetWidth(0.07);
        d->scalarBar->SetHeight(0.7);
        d->scalarBar->SetLookupTable(d->colorFunction);

        vtkRenderer *renderer = d->interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
        renderer->AddActor2D(d->scalarBar);
    }

    this->showScalarBarTitle(true);
    this->show();

    d->interactor->Render();
}

void gnomonActorVolume::showScalarBarTitle(bool show)
{
    if (!d->scalarBar)
        return;

    if (show) {
        d->scalarBar->SetTitle("Scalars");
        d->scalarBar->GetTitleTextProperty()->SetOpacity(1);
    } else
        d->scalarBar->GetTitleTextProperty()->SetOpacity(0);
}

void gnomonActorVolume::show()
{
    this->VisibilityOn();

    showScalarBar(d->scalarbar_state);
}

void gnomonActorVolume::hide()
{
    this->VisibilityOff();

    bool state_scalarbar = d->scalarbar_state;
    showScalarBar(false);
    d->scalarbar_state = state_scalarbar;
}

void gnomonActorVolume::setScalarBarOrientationToVertical(bool value)
{
    if (d->scalarBar) {
        if (value) {
            d->scalarBar->SetOrientationToVertical();
            d->scalarBar->SetWidth(0.08);
            d->scalarBar->SetHeight(0.6);
            d->scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
            d->scalarBar->GetPositionCoordinate()->SetValue(0.85, 0.05);
        } else {
            d->scalarBar->SetOrientationToHorizontal();
            d->scalarBar->SetWidth(0.6);
            d->scalarBar->SetHeight(0.08);
            d->scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
            d->scalarBar->GetPositionCoordinate()->SetValue(0.2, 0.05);
        }
        d->scalarBar->SetTextPositionToPrecedeScalarBar();
    }
}

void gnomonActorVolume::outlineNone(void)
{
    d->outline_corner_actor->SetVisibility(0);
    d->outline_box_actor->SetVisibility(0);
    d->outline_contour_actor->SetVisibility(0);
}

void gnomonActorVolume::outlineCorners(void)
{
    d->outline_corner_actor->SetVisibility(1);
    d->outline_box_actor->SetVisibility(0);
    d->outline_contour_actor->SetVisibility(0);
}

void gnomonActorVolume::outlineBox(void)
{
    d->outline_corner_actor->SetVisibility(0);
    d->outline_box_actor->SetVisibility(1);
    d->outline_contour_actor->SetVisibility(0);
}

void gnomonActorVolume::outlineContour(void)
{
    d->outline_corner_actor->SetVisibility(0);
    d->outline_box_actor->SetVisibility(0);
    d->outline_contour_actor->SetVisibility(1);
}

void *gnomonActorVolume::colorTransferFunction(void)
{
    return d->colorFunction;
}

void *gnomonActorVolume::opacityTransferFunction(void)
{
    return d->opacityTransferFunction;
}

void *gnomonActorVolume::mapper(void)
{
    return d->mapper;
}

void *gnomonActorVolume::vol(void)
{
    return d->vol;
}

void *gnomonActorVolume::volumeProperty(void)
{
    return d->volProperty;
}

void gnomonActorVolume::setColorTransferFunction(vtkColorTransferFunction *func)
{
    d->colorFunction = func;
    this->update();
}

void gnomonActorVolume::setOpacityTransferFunction(vtkPiecewiseFunction *func)
{
    d->opacityTransferFunction = func;
    this->update();
}

gnomonActorVolume::gnomonActorVolume(void) : gnomonActor(), d(new gnomonActorVolumePrivate)
{
    d->volume = NULL;
    d->interactor = NULL;
    d->filter = NULL;
    d->scalarBar = NULL;
    d->scalarbar_state = false;
    d->mapper = NULL;

    for (int i = 0; i < 3; ++i) {
        d->planes[i] = NULL;
        d->plane_index[i] = 0;
    }
}

gnomonActorVolume::~gnomonActorVolume(void)
{
    delete d;

    d = NULL;
}

void gnomonActorVolume::showScalarBar(bool show)
{
    d->scalarbar_state = show;

    if (d->scalarBar)
        d->scalarBar->SetVisibility(show);
}

//
// gnomonActorVolume.cpp ends here
