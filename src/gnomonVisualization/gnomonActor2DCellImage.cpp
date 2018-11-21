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

#include "gnomonActor2DCellImage.h"

#include <gnomonPolyDataCellImage>
#include <gnomonCellImage>

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkClipPolydata.h>
#include <vtkImageData.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActor2DCellImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DCellImagePrivate
{
public:
    vtkRenderWindowInteractor *interactor;

    gnomonCellImage *cellimage;

    vtkSmartPointer<gnomonPolyDataCellImage> polydata;
    QMap<int, vtkSmartPointer<vtkPolyDataMapper> > sliceMappers;
    QMap<int, vtkSmartPointer<vtkActor> > sliceActors;

public:
    float v_x=0, v_y=0, v_z=0;

public:
    int orientation;
    int slicePositions[3];

public:
    bool modified;

public:
    void updateVisibility(void);
};

void gnomonActor2DCellImagePrivate::updateVisibility(void)
{
    for (int i=0;i<3;i++)
    {
        if (this->orientation == i)
            this->sliceActors[i]->VisibilityOn();
        else
            this->sliceActors[i]->VisibilityOff();
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonActor2DCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DCellImage);

void gnomonActor2DCellImage::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActor2DCellImage::setCellImage(gnomonCellImage *cellimage)
{
    d->cellimage = cellimage;

    this->modified();
    this->update();
}

void gnomonActor2DCellImage::modified(void)
{
    d->modified = true;
}

void gnomonActor2DCellImage::update(void)
{
    if(!d->cellimage)
        return;

    if(!d->polydata)
        d->polydata = gnomonPolyDataCellImage::New();
    
    if (d->modified)
        d->polydata->setCellImage(d->cellimage);

        dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
        if(!converter)
            return;

        dtkImage *image = d->cellimage->image();
        converter->setInput(image);
        if(!converter->convert())
            return;

        vtkImageData *volume = static_cast<vtkImageData *>(converter->output());
        d->v_x = volume->GetSpacing()[0];
        d->v_y = volume->GetSpacing()[1];
        d->v_z = volume->GetSpacing()[2];
    
    float c_x = d->slicePositions[0];
    float c_y = d->slicePositions[1];
    float c_z = d->slicePositions[2];

    for (int i=0;i<3;i++)
    {
        vtkSmartPointer<vtkPlane> topPlane = vtkSmartPointer<vtkPlane>::New();
        if (i==0)
        {
            topPlane->SetOrigin((c_x+0.1)*d->v_x, 0, 0);
            topPlane->SetNormal(1, 0, 0);
        }
        else if (i==1)
        {
            topPlane->SetOrigin(0, (c_y+0.1)*d->v_y, 0);
            topPlane->SetNormal(0, 1, 0);
        }
        else
        {
            topPlane->SetOrigin(0, 0, (c_z+0.1)*d->v_z);
            topPlane->SetNormal(0, 0, 1);
        }

        vtkSmartPointer<vtkClipPolyData> topClipper = vtkSmartPointer<vtkClipPolyData>::New();
        topClipper->SetInputData(d->polydata);
        topClipper->SetClipFunction(topPlane);
        topClipper->SetValue(0);
        topClipper->InsideOutOn();
        topClipper->GenerateClippedOutputOn();

        vtkSmartPointer<vtkPlane> bottomPlane = vtkSmartPointer<vtkPlane>::New();
        if (i==0)
        {
            bottomPlane->SetOrigin((c_x-0.1)*d->v_x, 0, 0);
            bottomPlane->SetNormal(1, 0, 0);
        }
        else if (i==1)
        {
            bottomPlane->SetOrigin(0, (c_y-0.1)*d->v_y, 0);
            bottomPlane->SetNormal(0, 1, 0);
        }
        else
        {
            bottomPlane->SetOrigin(0, 0, (c_z-0.1)*d->v_z);
            bottomPlane->SetNormal(0, 0, 1);
        }

        vtkSmartPointer<vtkClipPolyData> bottomClipper = vtkSmartPointer<vtkClipPolyData>::New();
        bottomClipper->SetInputConnection(topClipper->GetOutputPort());
        bottomClipper->SetClipFunction(bottomPlane);
        bottomClipper->SetValue(0);
        // bottomClipper->InsideOutOn();
        bottomClipper->GenerateClippedOutputOn();
        bottomClipper->Update();

        if (!d->sliceMappers.contains(i)) {
            d->sliceMappers[i] = vtkSmartPointer<vtkPolyDataMapper>::New();
            d->sliceMappers[i]->SetScalarRange(0, d->cellimage->cellCount()-1);
        }
        d->sliceMappers[i]->SetInputConnection(bottomClipper->GetOutputPort());
        d->sliceMappers[i]->Update();

        if(!d->sliceActors.contains(i)) {
            d->sliceActors[i] = vtkSmartPointer<vtkActor>::New();
            d->sliceActors[i]->SetMapper(d->sliceMappers[i]);
            this->AddPart(d->sliceActors[i]);
        }
        d->sliceActors[i]->GetProperty()->SetOpacity(0.9);
        d->sliceActors[i]->GetProperty()->SetLineWidth(2.);
        d->sliceActors[i]->Modified();
    }

    d->modified = false;
}

void gnomonActor2DCellImage::hide(void)
{
    this->VisibilityOff();

    d->interactor->Render();
}

void gnomonActor2DCellImage::show(void)
{
    this->VisibilityOn();

    d->interactor->Render();
}

void gnomonActor2DCellImage::setSliceOrientation(int orientation)
{
    d->orientation = orientation;
    d->updateVisibility();
}

void gnomonActor2DCellImage::setSlice(int position)
{
    d->slicePositions[d->orientation] = position;
    this->update();
}

gnomonActor2DCellImage::gnomonActor2DCellImage(void) : d(new gnomonActor2DCellImagePrivate)
{
    d->cellimage = Q_NULLPTR;
    d->orientation = 2;
    d->slicePositions[0] = 0;
    d->slicePositions[1] = 0;
    d->slicePositions[2] = 0;
}

gnomonActor2DCellImage::~gnomonActor2DCellImage(void)
{
    delete d;

    d = NULL;
}

//
// gnomonActor2D.cpp ends here
