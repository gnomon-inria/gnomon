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

#include "gnomonActor2DImageWidget.h"

#include <dtkImagingCore>

#include <QtCore>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkImagePlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageWidgetPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DImageWidgetPrivate
{
public:
    vtkSmartPointer<vtkImageData> image;
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

    vtkSmartPointer<vtkColorTransferFunction> colorFunction = nullptr;
    vtkSmartPointer<vtkPiecewiseFunction> opacity = nullptr;

    vtkRenderWindowInteractor *interactor;

public:
    int orientation;
    
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

void gnomonActor2DImageWidgetPrivate::updateOpacity(void)
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

void gnomonActor2DImageWidgetPrivate::updateColorFunction(void)
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
// gnomonActor2DImageWidget
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DImageWidget);

void gnomonActor2DImageWidget::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
    for(int i = 0; i < 3; i++) {

        if(d->planeWidget[i])
            d->planeWidget[i]->SetInteractor(d->interactor);
    }
}

void gnomonActor2DImageWidget::setImage(vtkImageData *image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActor2DImageWidget::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImageWidget::update(void)
{
    if(!d->image)
        return;

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    d->updateColorFunction();

    if (!d->opacity)
        d->opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    d->updateOpacity();

    double image_range[2];
    d->image->GetPointData()->GetScalars()->GetRange(image_range);

    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    int n_values = 1000;
    lut->SetNumberOfTableValues(n_values);
    for(size_t i = 0; i < n_values; ++i) {
        double val = (i*image_range[1] + (n_values-i)*image_range[0])/(n_values-1.);
        double *rgb = d->colorFunction->GetColor(val);
        double a = d->opacity->GetValue(val);
        lut->SetTableValue(i,rgb[0],rgb[1],rgb[2],a);
    }
    lut->SetTableRange(image_range);

    int imageDims[3]; d->image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!d->planeWidget[i])
            d->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        d->planeWidget[i]->SetInputData(d->image);
        d->planeWidget[i]->SetPlaneOrientation(i);
        d->planeWidget[i]->RestrictPlaneToVolumeOn();
        d->planeWidget[i]->GetPlaneProperty()->SetColor(color);
        d->planeWidget[i]->SetLookupTable(lut);
        d->planeWidget[i]->SetResliceInterpolateToNearestNeighbour();
        // d->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        d->planeWidget[i]->SetMarginSizeX(0);
        d->planeWidget[i]->SetMarginSizeY(0);
        d->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        d->planeWidget[i]->DisplayTextOn();
        d->planeWidget[i]->InteractionOn();
    }
    
    d->modified = false;
}


void gnomonActor2DImageWidget::setSliceOrientation(int value)
{
    d->orientation = value;
}

void gnomonActor2DImageWidget::setSlice(int value)
{
    d->planeWidget[d->orientation]->SetSliceIndex(value/d->image->GetSpacing()[d->orientation]);
    d->interactor->Render();
}

void gnomonActor2DImageWidget::hide(void)
{        
    for (int o=0;o<3;o++)
        d->planeWidget[o]->Off();
}

void gnomonActor2DImageWidget::show(void)
{
    for (int o=0;o<3;o++)
        d->planeWidget[o]->On();
}


void gnomonActor2DImageWidget::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

void gnomonActor2DImageWidget::setValueRange(const QList<int>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateOpacity();
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DImageWidget::setColorMap(const QMap<double,QColor>& value)
{
    d->colormap = value;
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DImageWidget::setFlatRendering(bool value)
{
    d->flat_rendering = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DImageWidget::gnomonActor2DImageWidget(void) : gnomonActor(), d(new gnomonActor2DImageWidgetPrivate)
{
    d->image = Q_NULLPTR;

    d->colorFunction = Q_NULLPTR;
    d->opacity = Q_NULLPTR;

    d->orientation = 2;

    d->alpha = 1;
    d->flat_rendering = false;
    d->background_value = 1;
    d->value_range[0] = 0.;
    d->value_range[1] = 1.;
    d->colormap = QMap<double, QColor>({
        {0., QColor(0, 0, 0, 255)},
        {1., QColor(255, 255, 255, 255)} });
}

gnomonActor2DImageWidget::~gnomonActor2DImageWidget(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActor2DImageWidget.cpp ends here
