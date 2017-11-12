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

#include "gnomonActorScalarBar.h"

#include <vtkAbstractVolumeMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkConfigure.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProp3DCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkTextProperty.h>
#include <vtkScalarsToColors.h>
#include <vtkScalarBarActor.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageAccumulate.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorScalarBarPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorScalarBarPrivate
{
public:
    vtkImageData *data;

public:
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<vtkColorTransferFunction> colorFunction;
    vtkSmartPointer<vtkSmartVolumeMapper> mapper;

    vtkSmartPointer<vtkScalarBarActor> scalarBar;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorScalarBar
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorScalarBar);

void gnomonActorScalarBar::setData(vtkImageData *data)
{
    d->data = data;

    this->update();
}

void gnomonActorScalarBar::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorScalarBar::update(void)
{
    if(!d->data)
        return;

    if (!d->interactor)
        return;

    if(!d->mapper)
        d->mapper = vtkSmartVolumeMapper::New();

    d->mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);
    d->mapper->SetInputData(d->data);
    d->mapper->Modified();
    d->mapper->Update();

    double valuesRange[2];
    d->data->GetPointData()->GetScalars()->GetRange(valuesRange);

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

    if(!d->scalarBar) {
        d->scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        d->scalarBar->SetWidth(0.07);
        d->scalarBar->SetHeight(0.7);

        vtkRenderer *renderer = d->interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
        renderer->AddActor2D(d->scalarBar);
    }

    d->scalarBar->SetLookupTable(d->colorFunction);

    d->interactor->Render();
}

void gnomonActorScalarBar::showTitle(bool show)
{
    if (!d->scalarBar)
        return;

    if (show) {
        d->scalarBar->SetTitle("Scalars");
        d->scalarBar->GetTitleTextProperty()->SetOpacity(1);
    } else
        d->scalarBar->GetTitleTextProperty()->SetOpacity(0);
}

void gnomonActorScalarBar::show()
{
    if (d->scalarBar)
       d->scalarBar->SetVisibility(true);
    this->showTitle(true);
    d->interactor->Render();
}

void gnomonActorScalarBar::hide()
{
    if (d->scalarBar) {
        d->scalarBar->SetVisibility(false);
    }
    this->showTitle(false);
   d->interactor->Render();
}

void gnomonActorScalarBar::setOrientationToVertical(bool value)
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

void *gnomonActorScalarBar::colorTransferFunction(void)
{
    return d->colorFunction;
}

void gnomonActorScalarBar::setColorTransferFunction(vtkColorTransferFunction *func)
{
    d->colorFunction = func;
    this->update();
}

gnomonActorScalarBar::gnomonActorScalarBar(void) : gnomonActor(), d(new gnomonActorScalarBarPrivate)
{
    d->interactor = NULL;
    d->scalarBar = NULL;
}

gnomonActorScalarBar::~gnomonActorScalarBar(void)
{
    delete d;

    d = NULL;
}

//
// gnomonActorScalarBar.cpp ends here
