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

#include "gnomonActor2DImageRGBA.h"

#include <dtkImagingCore>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageRGBAPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DImageRGBAPrivate
{
public:
    vtkSmartPointer<vtkImageData> image;

    vtkSmartPointer<vtkImageActor> plane[3];

    vtkRenderWindowInteractor *interactor;

public:
    int orientation;

    double alpha;

    bool modified;

public slots:
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActor2DImageRGBAPrivate::updateOpacity(void)
{
    // qDebug()<<Q_FUNC_INFO<<this->alpha;
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DImageRGBA
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DImageRGBA);

void gnomonActor2DImageRGBA::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);

//    for(int i = 0; i < 3; i++) {
//
//        if(d->plane[i])
//            d->plane[i]->SetInteractor(d->interactor);
//    }
}

void gnomonActor2DImageRGBA::setImage(vtkImageData * image)
{
    d->image = image;

    this->modified();
    this->update();
}

void gnomonActor2DImageRGBA::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImageRGBA::update(void)
{
    if(!d->image)
        return;

    int imageDims[3];
    d->image->GetDimensions(imageDims);

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
        d->plane[i]->SetInputData(d->image);
//        d->plane[i]->SetPlaneOrientation(i);
        if (i == 0) {
            int pos = (x_min + x_max)/2;
            d->plane[i]->SetDisplayExtent(pos, pos, y_min, y_max, z_min, z_max);
        } else if (i == 1) {
            int pos = (y_min + y_max)/2;
            d->plane[i]->SetDisplayExtent(x_min, x_max, pos, pos, z_min, z_max);
        } else if (i ==2) {
            int pos = (z_min + z_max)/2;
            d->plane[i]->SetDisplayExtent(x_min, x_max, y_min, y_max, pos, pos);
        }

//        d->plane[i]->RestrictPlaneToVolumeOn();
        d->plane[i]->GetProperty()->UseLookupTableScalarRangeOn();
//        d->plane[i]->GetColorMap()->SetLookupTable(NULL);
//        d->plane[i]->GetColorMap()->SetOutputFormatToRGBA();
//        d->plane[i]->SetResliceInterpolateToNearestNeighbour();
//        d->plane[i]->TextureInterpolateOff();
        d->plane[i]->InterpolateOff();
//        // d->plane[i]->SetLeftButtonAction(vtkImageActor::VTK_SLICE_MOTION_ACTION);
//        d->plane[i]->SetMarginSizeX(0);
//        d->plane[i]->SetMarginSizeY(0);
//        d->plane[i]->SetSliceIndex(imageDims[i]/2);
//        d->plane[i]->DisplayTextOn();
//        d->plane[i]->InteractionOff();
    }

    d->modified = false;
}


void gnomonActor2DImageRGBA::setSliceOrientation(int value)
{
    d->orientation = value;
}

void gnomonActor2DImageRGBA::setSlice(int value)
{
//    d->plane[d->orientation]->SetSliceIndex(value/d->image->GetSpacing()[d->orientation]);

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

    d->plane[d->orientation]->SetDisplayExtent(x_min, x_max, y_min, y_max, z_min, z_max);
    d->interactor->Render();
}

void gnomonActor2DImageRGBA::hide(void)
{
//    for (int o=0;o<3;o++)
//        d->plane[o]->Off();
}

void gnomonActor2DImageRGBA::show(void)
{
//    for (int o=0;o<3;o++)
//        d->plane[o]->On();
}


void gnomonActor2DImageRGBA::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

gnomonActor2DImageRGBA::gnomonActor2DImageRGBA(void) : gnomonActor(), d(new gnomonActor2DImageRGBAPrivate)
{
    d->orientation = 2;

    d->alpha = 1;
}

gnomonActor2DImageRGBA::~gnomonActor2DImageRGBA(void)
{
    delete d;
    d = NULL;
}

//
// gnomonActor2DImageRGBA.cpp ends here
