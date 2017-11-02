// Version: $Id: ea8e0dd920c1140c7e0fbe8aeb5eb672c78bf8d4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "tissueActorImage.h"

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkColorTransferFunction.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageMapToColors.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarsToColors.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>

// /////////////////////////////////////////////////////////////////
// tissueActorImagePrivate
// /////////////////////////////////////////////////////////////////

class tissueActorImagePrivate
{
public:
    vtkImageData *image;

    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<vtkImagePlaneWidget> planes[3];
    bool plane_states[3];

    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    bool scalarbar_state;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
};

// /////////////////////////////////////////////////////////////////
// tissueActorImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(tissueActorImage);

tissueActorImage::tissueActorImage(void) : tissueActor(), d(new tissueActorImagePrivate)
{
    d->image = NULL;
    d->interactor = NULL;
    d->colorFunction = NULL;
    d->scalarBar = NULL;
    d->scalarbar_state = false;

    for (int i = 0; i < 3; ++i) {
        d->planes[i] = NULL;
        d->plane_states[i] = false;
    }
}

tissueActorImage::~tissueActorImage(void)
{
    delete d;

    d = NULL;
}

void tissueActorImage::setImage(vtkImageData *image)
{
    d->image = image;

    this->update();
}

vtkImageData *tissueActorImage::image(void)
{
    return d->image;
}

void tissueActorImage::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void tissueActorImage::show(void)
{
    showPlaneX(d->plane_states[0]);
    showPlaneY(d->plane_states[1]);
    showPlaneZ(d->plane_states[2]);
    showScalarBar(d->scalarbar_state);
}

void tissueActorImage::hide(void)
{
    bool state_x = d->plane_states[0];
    bool state_y = d->plane_states[1];
    bool state_z = d->plane_states[2];
    bool state_scalarbar = d->scalarbar_state;

    showPlaneX(false);
    showPlaneY(false);
    showPlaneZ(false);
    showScalarBar(false);

    d->plane_states[0] = state_x;
    d->plane_states[1] = state_y;
    d->plane_states[2] = state_z;
    d->scalarbar_state = state_scalarbar;
}

void tissueActorImage::setScalarBarOrientationToVertical(bool value)
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

void tissueActorImage::showPlaneX(bool value)
{
    d->plane_states[0] = value;

    if (d->planes[0]) {
        if (value)
            d->planes[0]->On();
        else
            d->planes[0]->Off();
    }
}

void tissueActorImage::showPlaneY(bool value)
{
    d->plane_states[1] = value;

    if (d->planes[1]) {
        if (value)
            d->planes[1]->On();
        else
            d->planes[1]->Off();
    }
}

void tissueActorImage::showPlaneZ(bool value)
{
    d->plane_states[2] = value;

    if (d->planes[2]) {
        if (value)
            d->planes[2]->On();
        else
            d->planes[2]->Off();
    }
}

void tissueActorImage::update(void)
{
    if(!d->image)
        return;

    if(!d->interactor)
        return;

    double valuesRange[2]; d->image->GetPointData()->GetScalars()->GetRange(valuesRange);

    double min = valuesRange[0];
    double max = valuesRange[1];
    double mid = (min + max)/2.;

    if(!d->colorFunction) {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->colorFunction->SetColorSpaceToHSV();
        d->colorFunction->HSVWrapOff();
        d->colorFunction->RemoveAllPoints();
        d->colorFunction->AddRGBPoint(min, 0.0, 0.0, 1.0);
        d->colorFunction->AddRGBPoint(mid, 0.0, 1.0, 0.0);
        d->colorFunction->AddRGBPoint(max, 1.0, 0.0, 0.0);
    }

    int index = 0;

    for (int i = 0; i < 3; ++i) {

        if(!d->planes[i]) {
            d->planes[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
            d->planes[i]->SetInteractor(d->interactor);
            index = d->image->GetDimensions()[i]/2;
        } else {
            index = d->planes[i]->GetSliceIndex();
        }
        d->planes[i]->SetInputData(d->image);
        d->planes[i]->SetPlaneOrientation(i);
        d->planes[i]->GetColorMap()->SetLookupTable(d->colorFunction);
        d->planes[i]->PlaceWidget();
        d->planes[i]->SetSliceIndex(index);
        d->planes[i]->On();
        d->planes[i]->InteractionOn();
    }

    if(!d->scalarBar) {
        d->scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        d->scalarBar->SetWidth(0.07);
        d->scalarBar->SetHeight(0.7);
        d->scalarBar->SetLookupTable(d->colorFunction);
        d->scalarBar->SetVisibility(1);
        d->scalarBar->DragableOn();
        d->scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
        d->scalarBar->GetPositionCoordinate()->SetValue(0.1, 0.1);

        vtkRenderer *renderer = d->interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
        renderer->AddActor2D(d->scalarBar);
    }

    this->showScalarBarTitle(true);
    this->show();

    d->interactor->Render();
}

void tissueActorImage::showScalarBarTitle(bool show)
{
    if (!d->scalarBar)
        return;

    if (show) {
        d->scalarBar->SetTitle("Scalars");
        d->scalarBar->GetTitleTextProperty()->SetOpacity(1);
    } else
        d->scalarBar->GetTitleTextProperty()->SetOpacity(0);
}

void tissueActorImage::showScalarBar(bool show)
{
    d->scalarbar_state = show;

    if (d->scalarBar)
        d->scalarBar->SetVisibility(show);
}

//
// tissueActorImage.cpp ends here
