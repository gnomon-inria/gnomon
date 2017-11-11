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

#include "gnomonActorImage.h"

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
#include <vtkImageActor.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorImagePrivate
{
public:
    vtkImageData *image;

    //TODO Iteractor is not used
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<vtkImageActor> planes[3];
    bool plane_states[3];
    double plane_positions[3];
    double plane_opacities[3];

    // ///////////////////////////////////////////////////////////////
    vtkSmartPointer<vtkImageMapToColors> colors;
    // ///////////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    bool scalarbar_state;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorImage);

gnomonActorImage::gnomonActorImage(void) : gnomonActor(), d(new gnomonActorImagePrivate)
{
    d->image = NULL;
    d->interactor = NULL;
    d->colorFunction = NULL;
    d->scalarBar = NULL;
    d->scalarbar_state = false;

    for (int i = 0; i < 3; ++i) {
        d->planes[i] = NULL;
        d->plane_states[i] = false;
        d->plane_positions[i] = 0.5;
        d->plane_opacities[i] = 0.5;
    }
}

gnomonActorImage::~gnomonActorImage(void)
{
    delete d;

    d = NULL;
}

/*need to call update to render */
void gnomonActorImage::setImage(vtkImageData *image)
{
    d->image = image;
}

vtkImageData *gnomonActorImage::image(void)
{
    return d->image;
}

void gnomonActorImage::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorImage::show(void)
{
    showPlaneX(true);
    showPlaneY(true);
    showPlaneZ(true);
    showScalarBar(d->scalarbar_state);
    d->interactor->Render();
}

void gnomonActorImage::hide(void)
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
    d->interactor->Render();
}

void gnomonActorImage::setScalarBarOrientationToVertical(bool value)
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

/*An int between 0 and 1000 */
void gnomonActorImage::setXPlanePos(int pos)
{
    d->plane_positions[0] = pos;
    this->update();
}

/*An int between 0 and 1000 */
void gnomonActorImage::setYPlanePos(int pos)
{
    d->plane_positions[1] = pos;
    this->update();
}

/*An int between 0 and 1000 */
void gnomonActorImage::setZPlanePos(int pos)
{
    d->plane_positions[2] = pos;
    this->update();
}

void gnomonActorImage::setXPlaneOpacity(double opacity)
{
    d->plane_opacities[0] = opacity;
    this->update();
}

void gnomonActorImage::setYPlaneOpacity(double opacity)
{
    d->plane_opacities[1] = opacity;
    this->update();
}

void gnomonActorImage::setZPlaneOpacity(double opacity)
{
    d->plane_opacities[2] = opacity;
    this->update();
}

void gnomonActorImage::showPlaneX(bool value)
{
    d->planes[0]->SetVisibility(value);
}

void gnomonActorImage::showPlaneY(bool value)
{
    d->planes[1]->SetVisibility(value);
}

void gnomonActorImage::showPlaneZ(bool value)
{
    d->planes[2]->SetVisibility(value);
}

void gnomonActorImage::update(void)
{
    if(!d->image)
        return;
    if(!d->interactor)
        return;
    double valuesRange[2];
    d->image->GetPointData()->GetScalars()->GetRange(valuesRange);

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

    if (!d->colors) {
        d->colors = vtkSmartPointer<vtkImageMapToColors>::New();
        d->colors->SetInputData(d->image);
        d->colors->SetLookupTable(d->colorFunction);
        d->colors->Update();
    }
    d->colors->SetLookupTable(d->colorFunction);
    d->colors->Update();

    int x_min, x_max, y_min, y_max, z_min, z_max;
    double voxeslize[3];
    d->image->GetExtent(x_min, x_max, y_min, y_max, z_min, z_max);
    d->image->GetSpacing(voxeslize);
    for (int i = 0; i < 3; ++i) {

        if(!d->planes[i])
            d->planes[i] = vtkSmartPointer<vtkImageActor>::New();

        int pos = std::min(d->image->GetDimensions()[i], (int)(d->image->GetDimensions()[i] * d->plane_positions[i] / 100));
        d->planes[i]->SetInputData(d->colors->GetOutput());
        d->plane_states[i] = true;
        if (i == 0) {
            d->planes[i]->SetDisplayExtent(pos, pos, y_min, y_max, z_min, z_max);
        } else if (i == 1) {
            d->planes[i]->SetDisplayExtent(x_min, x_max, pos, pos, z_min, z_max);
        } else if (i ==2) {
            d->planes[i]->SetDisplayExtent(x_min, x_max, y_min, y_max, pos, pos);
        }
        d->planes[i]->SetOpacity(d->plane_opacities[i]);
        d->planes[i]->Update();
        this->AddPart(d->planes[i]);

        d->planes[i]->Modified();
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

    d->interactor->Render();
}

void gnomonActorImage::setColorTransferFunction(vtkColorTransferFunction *func)
{
    d->colorFunction = func;
    this->update();
}

void *gnomonActorImage::colorTransferFunction(void)
{
    return d->colorFunction;
}

void gnomonActorImage::showScalarBarTitle(bool show)
{
    if (!d->scalarBar)
        return;

    if (show) {
        d->scalarBar->SetTitle("Scalars");
        d->scalarBar->GetTitleTextProperty()->SetOpacity(1);
    } else
        d->scalarBar->GetTitleTextProperty()->SetOpacity(0);
}

void gnomonActorImage::showScalarBar(bool show)
{
    d->scalarbar_state = show;

    if (d->scalarBar)
        d->scalarBar->SetVisibility(show);
}

//
// gnomonActorImage.cpp ends here
