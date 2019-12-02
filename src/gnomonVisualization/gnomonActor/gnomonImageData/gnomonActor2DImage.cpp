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

#include "gnomonActor2DImage.h"

#include <dtkImagingCore>

#include <QtCore>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonActor2DImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DImagePrivate
{
public:
    vtkSmartPointer<vtkImageData> image;
    vtkSmartPointer<vtkImageActor> plane[3];

    vtkSmartPointer<vtkColorTransferFunction> colorFunction = nullptr;
    vtkSmartPointer<vtkImageMapToColors> colors = nullptr;
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

void gnomonActor2DImagePrivate::updateOpacity(void)
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

void gnomonActor2DImagePrivate::updateColorFunction(void)
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
// gnomonActor2DImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DImage);

void gnomonActor2DImage::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
//    for(int i = 0; i < 3; i++) {
//
//        if(d->plane[i])
//            d->plane[i]->SetInteractor(d->interactor);
//    }
}

void gnomonActor2DImage::setImage(vtkImageData *image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActor2DImage::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImage::update(void)
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

    if (!d->colors) {
        d->colors = vtkSmartPointer<vtkImageMapToColors>::New();
        d->colors->SetInputData(d->image);
        d->colors->SetLookupTable(d->colorFunction);
        d->colors->Update();
    }
    d->colors->SetLookupTable(d->colorFunction);
    d->colors->Update();

    int imageDims[3]; d->image->GetDimensions(imageDims);

    int x_min, x_max, y_min, y_max, z_min, z_max;
    double voxeslize[3];

    d->image->GetExtent(x_min, x_max, y_min, y_max, z_min, z_max);
    d->image->GetSpacing(voxeslize);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!d->plane[i]) {
            d->plane[i] = vtkSmartPointer<vtkImageActor>::New();
            this->AddPart(d->plane[i]);
        }
        d->plane[i]->SetInputData(d->colors->GetOutput());
        if (i == 0) {
            int pos = (x_min + x_max)/2;
            d->plane[i]->SetDisplayExtent(pos, pos, y_min, y_max, z_min, z_max);
            qDebug()<<Q_FUNC_INFO<<"Plane"<<i<<": "<<pos<<pos<<y_min<<y_max<<z_min<<z_max;
        } else if (i == 1) {
            int pos = (y_min + y_max)/2;
            d->plane[i]->SetDisplayExtent(x_min, x_max, pos, pos, z_min, z_max);
            qDebug()<<Q_FUNC_INFO<<"Plane"<<i<<": "<<x_min<<x_max<<pos<<pos<<z_min<<z_max;
        } else if (i ==2) {
            int pos = (z_min + z_max)/2;
            d->plane[i]->SetDisplayExtent(x_min, x_max, y_min, y_max, pos, pos);
            qDebug()<<Q_FUNC_INFO<<"Plane"<<i<<": "<<x_min<<x_max<<y_min<<y_max<<pos<<pos;
        }
//        d->plane[i]->SetPlaneOrientation(i);
//        d->plane[i]->RestrictPlaneToVolumeOn();
//        d->plane[i]->GetPlaneProperty()->SetColor(color);
//        d->plane[i]->SetLookupTable(lut);
//        d->plane[i]->SetResliceInterpolateToNearestNeighbour();
          d->plane[i]->InterpolateOff();
//        // d->plane[i]->SetLeftButtonAction(vtkImageActor::VTK_SLICE_MOTION_ACTION);
//        d->plane[i]->SetMarginSizeX(0);
//        d->plane[i]->SetMarginSizeY(0);
//        d->plane[i]->SetSliceIndex(imageDims[i]/2);
//        d->plane[i]->DisplayTextOn();
//        d->plane[i]->InteractionOn();
    }

    d->modified = false;
}


void gnomonActor2DImage::setSliceOrientation(int value)
{
    d->orientation = value;
}

void gnomonActor2DImage::setSlice(int value)
{
    int pos = value/d->image->GetSpacing()[d->orientation];

    int x_min, x_max, y_min, y_max, z_min, z_max;
    d->image->GetExtent(x_min, x_max, y_min, y_max, z_min, z_max);

    if(d->orientation == 0) {
        x_min = pos;
        x_max = pos;
    } else if (d->orientation == 1) {
        y_min = pos;
        y_max = pos;
    } else if (d->orientation ==2) {
        z_min = pos;
        z_max = pos;
    }

    qDebug()<<Q_FUNC_INFO<<"Plane"<<d->orientation<<": "<<x_min<<x_max<<y_min<<y_max<<z_min<<z_max;
    d->plane[d->orientation]->SetDisplayExtent(x_min, x_max, y_min, y_max, z_min, z_max);
    d->interactor->Render();
}

void gnomonActor2DImage::hide(void)
{
//    for (int o=0;o<3;o++)
//        d->plane[o]->Off();
}

void gnomonActor2DImage::show(void)
{
//    for (int o=0;o<3;o++)
//        d->plane[o]->On();
}


void gnomonActor2DImage::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

void gnomonActor2DImage::setValueRange(const QList<int>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateOpacity();
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DImage::setColorMap(const QMap<double,QColor>& value)
{
    d->colormap = value;
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DImage::setFlatRendering(bool value)
{
    d->flat_rendering = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DImage::gnomonActor2DImage(void) : gnomonActor(), d(new gnomonActor2DImagePrivate)
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

gnomonActor2DImage::~gnomonActor2DImage(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActor2DImage.cpp ends here
