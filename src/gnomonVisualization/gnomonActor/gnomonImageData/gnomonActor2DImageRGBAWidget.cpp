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

#include "gnomonActor2DImageRGBAWidget.h"

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
// gnomonActor2DImageRGBAWidgetPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DImageRGBAWidgetPrivate
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

void gnomonActor2DImageRGBAWidgetPrivate::updateOpacity(void)
{
    // qDebug()<<Q_FUNC_INFO<<this->alpha;
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageRGBAWidget
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DImageRGBAWidget);

void gnomonActor2DImageRGBAWidget::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
    for(int i = 0; i < 3; i++) {

        if(d->planeWidget[i])
            d->planeWidget[i]->SetInteractor(d->interactor);
    }
}

void gnomonActor2DImageRGBAWidget::setImage(vtkImageData * image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActor2DImageRGBAWidget::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImageRGBAWidget::update(void)
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
        d->planeWidget[i]->SetResliceInterpolateToNearestNeighbour();
        d->planeWidget[i]->TextureInterpolateOff();
        // d->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        d->planeWidget[i]->SetMarginSizeX(0);
        d->planeWidget[i]->SetMarginSizeY(0);
        d->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        d->planeWidget[i]->DisplayTextOn();
        d->planeWidget[i]->InteractionOff();
    }

    d->modified = false;
}


void gnomonActor2DImageRGBAWidget::setSliceOrientation(int value)
{
    d->orientation = value;
}

void gnomonActor2DImageRGBAWidget::setSlice(int value)
{
    d->planeWidget[d->orientation]->SetSliceIndex(value/d->image->GetSpacing()[d->orientation]);
    d->interactor->Render();
}

void gnomonActor2DImageRGBAWidget::hide(void)
{
    for (int o=0;o<3;o++)
        d->planeWidget[o]->Off();
}

void gnomonActor2DImageRGBAWidget::show(void)
{
    for (int o=0;o<3;o++)
        d->planeWidget[o]->On();
}


void gnomonActor2DImageRGBAWidget::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DImageRGBAWidget::gnomonActor2DImageRGBAWidget(void) : gnomonActor(), d(new gnomonActor2DImageRGBAWidgetPrivate)
{
    d->orientation = 2;

    d->alpha = 1;
}

gnomonActor2DImageRGBAWidget::~gnomonActor2DImageRGBAWidget(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActor2DImageRGBAWidget.cpp ends here
