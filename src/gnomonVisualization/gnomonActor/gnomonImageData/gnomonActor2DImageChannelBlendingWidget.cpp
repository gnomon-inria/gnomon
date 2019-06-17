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

#include "gnomonActor2DImageChannelBlendingWidget.h"

#include <dtkImagingCore>

#include <QtCore>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImagePlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageChannelBlendingWidgetPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DImageChannelBlendingWidgetPrivate
{
public:
    vtkSmartPointer<vtkImageData> image;
    
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

    vtkRenderWindowInteractor *interactor;

public:
    int orientation;
    
    double alpha;

    bool modified;

public slots:
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActor2DImageChannelBlendingWidgetPrivate::updateOpacity(void)
{
    qDebug()<<Q_FUNC_INFO<<this->alpha<<" : Not implemented yet.";
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageChannelBlendingWidget
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DImageChannelBlendingWidget);

void gnomonActor2DImageChannelBlendingWidget::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
    for(int i = 0; i < 3; i++) {

        if(d->planeWidget[i])
            d->planeWidget[i]->SetInteractor(d->interactor);
    }
}

void gnomonActor2DImageChannelBlendingWidget::setImage(vtkImageData * image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActor2DImageChannelBlendingWidget::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImageChannelBlendingWidget::update(void)
{
    if(!d->image)
        return;

    int imageDims[3]; 
    d->image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!d->planeWidget[i])
            d->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        d->planeWidget[i]->SetInputData(d->image);
        d->planeWidget[i]->SetPlaneOrientation(i);
        d->planeWidget[i]->RestrictPlaneToVolumeOn();
        d->planeWidget[i]->GetPlaneProperty()->SetColor(color);
        d->planeWidget[i]->GetColorMap()->SetLookupTable(NULL);
        d->planeWidget[i]->GetColorMap()->SetOutputFormatToRGBA();
        // d->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        d->planeWidget[i]->SetMarginSizeX(0);
        d->planeWidget[i]->SetMarginSizeY(0);
        d->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        d->planeWidget[i]->DisplayTextOn();
        d->planeWidget[i]->InteractionOn();
    }
    
    d->modified = false;
}


void gnomonActor2DImageChannelBlendingWidget::setSliceOrientation(int value)
{
    d->orientation = value;
}

void gnomonActor2DImageChannelBlendingWidget::setSlice(int value)
{
    d->planeWidget[d->orientation]->SetSliceIndex(value/d->image->GetSpacing()[d->orientation]);
    d->interactor->Render();
}

void gnomonActor2DImageChannelBlendingWidget::hide(void)
{        
    for (int o=0;o<3;o++)
        d->planeWidget[o]->Off();
}

void gnomonActor2DImageChannelBlendingWidget::show(void)
{
    for (int o=0;o<3;o++)
        d->planeWidget[o]->On();
}


void gnomonActor2DImageChannelBlendingWidget::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DImageChannelBlendingWidget::gnomonActor2DImageChannelBlendingWidget(void) : gnomonActor(), d(new gnomonActor2DImageChannelBlendingWidgetPrivate)
{
    // d->image = Q_NULLPTR;

    // d->colorFunction = Q_NULLPTR;
    // d->opacity = Q_NULLPTR;

    d->orientation = 2;

    d->alpha = 1;
    // d->value_range[0] = 0.;
    // d->value_range[1] = 1.;
    // d->colormap = QMap<double, QColor>({
    //     {0., QColor(0, 0, 0, 255)},
    //     {1., QColor(255, 255, 255, 255)} });
}

gnomonActor2DImageChannelBlendingWidget::~gnomonActor2DImageChannelBlendingWidget(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActor2DImageChannelBlendingWidget.cpp ends here
