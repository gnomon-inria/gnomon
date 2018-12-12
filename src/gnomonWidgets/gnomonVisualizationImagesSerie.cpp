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

#include "gnomonVisualizationImagesSerie.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonImagesSerie>
#include <gnomonCore/gnomonCoreParameter>
#include <dtkImagingCore>

#include "gnomonViewForm.h"

// #include "gnomonPolyDataImagesSerie.h"
// #include "gnomonActorPolyData.h"
// #include "gnomonActor2DPolyData.h"

#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageViewer2.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>

#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSeriePrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImagesSeriePrivate
{
public:
    gnomonImagesSerie *imagesSerie;

public:
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;
    vtkSmartPointer<vtkVolume> volume = nullptr;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = nullptr;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerie
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImagesSerie::gnomonVisualizationImagesSerie(gnomonViewForm* view) : gnomonAbstractVisualization(view), dd(new gnomonVisualizationImagesSeriePrivate)
{
    dd->imagesSerie = Q_NULLPTR;

    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the image rendering");
    d->parameters["channel"] = new gnomonCoreParameterStringList("", {""}, "Image channel to be displayed");
}

gnomonVisualizationImagesSerie::~gnomonVisualizationImagesSerie(void)
{
    delete dd;

    dd = NULL;
}

void gnomonVisualizationImagesSerie::setImagesSerie(gnomonImagesSerie *imagesSerie)
{
    dd->imagesSerie = imagesSerie;
    this->update();
}

void gnomonVisualizationImagesSerie::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    
    double valueRange[2];
    dd->image->GetPointData()->GetScalars()->GetRange(valueRange);

    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(valueRange[0],0.00);
    opacity->AddPoint(valueRange[1],alpha);

    dd->volume->GetProperty()->SetScalarOpacity(opacity);
}

void gnomonVisualizationImagesSerie::update(void)
{
    if(!dd->imagesSerie)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    QString channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();

    if(dd->imagesSerie->channels().contains(channel))
        dd->imagesSerie->setChannel(channel);

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(dd->imagesSerie->image());
    converter->convert();
    dd->image = static_cast<vtkImageData *>(converter->output());

    delete converter;

    double valueRange[2];
    dd->image->GetPointData()->GetScalars()->GetRange(valueRange);

    vtkSmartPointer<vtkColorTransferFunction> color_function = vtkSmartPointer<vtkColorTransferFunction>::New();
    color_function->AddRGBPoint(valueRange[0],0,0,0);
    color_function->AddRGBPoint(valueRange[1],1,1,1);
    color_function->ClampingOn();
    color_function->Modified();

    vtkSmartPointer<vtkImageMapToColors> image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    image_color->SetLookupTable(color_function);
    image_color->SetOutputFormatToRGBA();
    image_color->SetInputData(dd->image);
    image_color->Update();

    int imageDims[3]; dd->image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!dd->planeWidget[i])
            dd->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        dd->planeWidget[i]->SetInputData(dd->image);
        dd->planeWidget[i]->SetInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
        dd->planeWidget[i]->SetPlaneOrientation(i);
        dd->planeWidget[i]->RestrictPlaneToVolumeOn();
        dd->planeWidget[i]->GetPlaneProperty()->SetColor(color);
        // dd->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        dd->planeWidget[i]->SetMarginSizeX(0);
        dd->planeWidget[i]->SetMarginSizeY(0);
        dd->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        dd->planeWidget[i]->DisplayTextOn();
        // d->view->renderer2D()->AddActor(d->planeWidget[i])
        dd->planeWidget[i]->InteractionOn();
    }

    if(!dd->volume_mapper)
        dd->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    dd->volume_mapper->SetInputData(dd->image);
    dd->volume_mapper->SetRequestedRenderModeToRayCast();
    dd->volume_mapper->Modified();
    dd->volume_mapper->Update();

    if(!dd->volume) {
        dd->volume = vtkSmartPointer<vtkVolume>::New();
        d->view->renderer3D()->AddActor(dd->volume);
    }

    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(valueRange[0],0.00);
    opacity->AddPoint(valueRange[1],alpha);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(opacity);
    property->SetColor(color_function);
    property->ShadeOff();
    property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    dd->volume->SetMapper(dd->volume_mapper);
    dd->volume->SetProperty(property);
    dd->volume->Modified();
    dd->volume->Update();


    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->orientation = value;
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        dd->planeWidget[dd->orientation]->SetSliceIndex(value/dd->image->GetSpacing()[dd->orientation]);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { 
        for (int o=0;o<3;o++)
            dd->planeWidget[o]->Off();
        this->render(); 
    });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { 
        for (int o=0;o<3;o++)
            dd->planeWidget[o]->On();
        this->render();
    });
    connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (dd->image->GetDimensions()[0]-1)*dd->image->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (dd->image->GetDimensions()[1]-1)*dd->image->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (dd->image->GetDimensions()[2]-1)*dd->image->GetSpacing()[2];
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationImagesSerie::render(void)
{
    this->updateOpacity();
    d->view->render();
}


//
// gnomonVisualizationImagesSerie.cpp ends here
