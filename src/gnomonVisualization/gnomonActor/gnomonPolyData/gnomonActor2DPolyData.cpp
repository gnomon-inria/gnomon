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

#include "gnomonActor2DPolyData.h"

#include "gnomonPolyData.h"

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActor2DPolyDataPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActor2DPolyDataPrivate
{
public:
    vtkRenderWindowInteractor *interactor;

    vtkSmartPointer<gnomonPolyData> polydata;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    QMap<int, vtkSmartPointer<vtkPolyDataMapper> > sliceMappers;
    QMap<int, vtkSmartPointer<vtkActor> > sliceActors;

public:
    double slicePositions[3];
    double sliceThickness;

public:
    int orientation;

    double alpha;
    double value_range[2];
    QMap<double,QColor> colormap;

public:
    bool modified;

public slots:
    void updateVisibility(void);
    void updateSlice(int orientation);
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActor2DPolyDataPrivate::updateVisibility(void)
{
    for (int i=0;i<3;i++)
    {
        if (this->orientation == i)
            this->sliceActors[i]->VisibilityOn();
        else
            this->sliceActors[i]->VisibilityOff();
    }
}

void gnomonActor2DPolyDataPrivate::updateSlice(int orientation)
{
    qDebug()<<Q_FUNC_INFO;
    vtkSmartPointer<vtkPlane> topPlane = vtkSmartPointer<vtkPlane>::New();
    if (orientation==0)
    {
        topPlane->SetOrigin(this->slicePositions[0]+this->sliceThickness, 0, 0);
        topPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        topPlane->SetOrigin(0, this->slicePositions[1]+this->sliceThickness, 0);
        topPlane->SetNormal(0, 1, 0);
    }
    else
    {
        topPlane->SetOrigin(0, 0, this->slicePositions[2]+this->sliceThickness);
        topPlane->SetNormal(0, 0, 1);
    }
    qDebug()<<Q_FUNC_INFO<<"Top Plane OK!";

    vtkSmartPointer<vtkClipPolyData> topClipper = vtkSmartPointer<vtkClipPolyData>::New();
    topClipper->SetInputData(this->polydata);
    topClipper->SetClipFunction(topPlane);
    topClipper->SetValue(0);
    topClipper->InsideOutOn();
    qDebug()<<Q_FUNC_INFO<<this->polydata;
    topClipper->Update();
    qDebug()<<Q_FUNC_INFO<<"Top Clipper OK!";

    vtkSmartPointer<vtkPlane> bottomPlane = vtkSmartPointer<vtkPlane>::New();
    if (orientation==0)
    {
        bottomPlane->SetOrigin(this->slicePositions[0]-this->sliceThickness, 0, 0);
        bottomPlane->SetNormal(1, 0, 0);
    }
    else if (orientation==1)
    {
        bottomPlane->SetOrigin(0, this->slicePositions[1]-this->sliceThickness, 0);
        bottomPlane->SetNormal(0, 1, 0);
    }
    else
    {
        bottomPlane->SetOrigin(0, 0, this->slicePositions[2]-this->sliceThickness);
        bottomPlane->SetNormal(0, 0, 1);
    }

    vtkSmartPointer<vtkClipPolyData> bottomClipper = vtkSmartPointer<vtkClipPolyData>::New();
    bottomClipper->SetInputConnection(topClipper->GetOutputPort());
    bottomClipper->SetClipFunction(bottomPlane);
    bottomClipper->SetValue(0);
    bottomClipper->Update();


    if (!this->colorFunction)
    {
        this->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    }
    this->updateColorFunction();


    if (!this->sliceMappers.contains(orientation)) {
        this->sliceMappers[orientation] = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    // vtkSmartPointer<vtkDoubleArray> cellData = (vtkDoubleArray *) this->polydata->GetCellData()->GetArray(0);
    // this->sliceMappers[orientation]->SetScalarRange(cellData->GetRange());
    this->sliceMappers[orientation]->SetLookupTable(this->colorFunction);
    this->sliceMappers[orientation]->SetInputConnection(bottomClipper->GetOutputPort());
    this->sliceMappers[orientation]->Update();

    if(!this->sliceActors.contains(orientation)) {
        this->sliceActors[orientation] = vtkSmartPointer<vtkActor>::New();
        this->sliceActors[orientation]->SetMapper(this->sliceMappers[orientation]);
    }
    this->sliceActors[orientation]->GetProperty()->SetRepresentationToWireframe();
    this->sliceActors[orientation]->GetProperty()->SetLineWidth(2.);
    this->sliceActors[orientation]->Modified();
}

void gnomonActor2DPolyDataPrivate::updateOpacity(void)
{
    for (const auto& orientation : this->sliceActors.keys()) {
        this->sliceActors[orientation]->GetProperty()->SetOpacity(this->alpha);
    }
}

void gnomonActor2DPolyDataPrivate::updateColorFunction(void)
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
// gnomonActor2DPolyData
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActor2DPolyData);

void gnomonActor2DPolyData::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActor2DPolyData::setPolyData(gnomonPolyData *polydata)
{
    d->polydata = polydata;
    this->modified();
    this->update();
}

void gnomonActor2DPolyData::setSliceThickness(double value)
{
    d->sliceThickness = value;
    this->modified();
}

void gnomonActor2DPolyData::setSlicePositions(double value[3])
{
    for(int i=0;i<3;i++)
        d->slicePositions[i] = value[i];
}

void gnomonActor2DPolyData::modified(void)
{
    d->modified = true;
}

void gnomonActor2DPolyData::update(void)
{
    if(!d->polydata)
        return;

    if (d->modified) {
        for (int i=0;i<3;i++) {
            d->updateSlice(i);
            if (this->GetNumberOfPaths()<=i)
                this->AddPart(d->sliceActors[i]);
        }
    }

    d->updateColorFunction();
    d->updateOpacity();
    d->updateVisibility();

    d->interactor->Render();

    d->modified = false;
}

void gnomonActor2DPolyData::hide(void)
{
    this->VisibilityOff();
    d->interactor->Render();
}

void gnomonActor2DPolyData::show(void)
{
    this->VisibilityOn();
    d->interactor->Render();
}

void gnomonActor2DPolyData::setSliceOrientation(int value)
{
    d->orientation = value;
    d->updateVisibility();
}

void gnomonActor2DPolyData::setSlice(int value)
{
    d->slicePositions[d->orientation] = value;
    d->updateSlice(d->orientation);
    d->interactor->Render();
}

void gnomonActor2DPolyData::setOpacity(double value)
{
    d->alpha = value;
    d->updateOpacity();
    d->interactor->Render();
}

void gnomonActor2DPolyData::setValueRange(const QList<double>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DPolyData::setColorMap(const QMap<double,QColor>& value)
{
    d->colormap = value;
    d->updateColorFunction();
    d->interactor->Render();
}

gnomonActor2DPolyData::gnomonActor2DPolyData(void) : d(new gnomonActor2DPolyDataPrivate)
{
    d->interactor = Q_NULLPTR;
    d->polydata = Q_NULLPTR;
    d->colorFunction = Q_NULLPTR;

    d->alpha = 1;
    d->value_range[0] = 0.;
    d->value_range[1] = 1.;
    d->colormap = QMap<double, QColor>({
        {0., QColor(0, 0, 0, 255)},
        {1., QColor(255, 255, 255, 255)} });

    d->orientation = 2;
    d->sliceThickness = 1;
    d->slicePositions[0] = 0;
    d->slicePositions[1] = 0;
    d->slicePositions[2] = 0;
}

gnomonActor2DPolyData::~gnomonActor2DPolyData(void)
{
    delete d;

    d = nullptr;
}

//
// gnomonActor2D.cpp ends here
