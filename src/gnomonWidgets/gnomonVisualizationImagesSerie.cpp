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
    gnomonViewForm* view;
    gnomonImagesSerie *imagesSerie;

public:
    QMap<QString, gnomonCoreParameter *> parameters;

public:
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;
    // vtkSmartPointer<vtkImageViewer2> viewer = nullptr;
    vtkSmartPointer<vtkVolume> volume = nullptr;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = nullptr;

public:
    QMetaObject::Connection connectSliceOrientation;
    QMetaObject::Connection connectSlice;

// public slots:
//     void updateOpacity(void);

};

// void gnomonVisualizationImagesSeriePrivate::updateOpacity(void)
// {
//     double alpha = ((gnomonCoreParameterDouble *)this->parameters["alpha"])->value();
    
//     if(this->actor) {
//         this->actor->setOpacity(alpha);
//     }

//     if(this->actor2D) {
//         this->actor2D->setOpacity(alpha);
//     }
// }


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerie
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImagesSerie::gnomonVisualizationImagesSerie(gnomonViewForm* view) : d(new gnomonVisualizationImagesSeriePrivate)
{
    d->view = view;
    d->imagesSerie = Q_NULLPTR;

    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the image rendering");
    d->parameters["channel"] = new gnomonCoreParameterStringList("", {""}, "Image channel to be displayed");
}

gnomonVisualizationImagesSerie::~gnomonVisualizationImagesSerie(void)
{
    delete d;

    d = NULL;
}

void gnomonVisualizationImagesSerie::setImagesSerie(gnomonImagesSerie *imagesSerie)
{
    d->imagesSerie = imagesSerie;
    this->update();
}

QMap<QString, gnomonCoreParameter *> gnomonVisualizationImagesSerie::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationImagesSerie::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}


void gnomonVisualizationImagesSerie::update(void)
{
    if(!d->imagesSerie)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    QString channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();

    if(d->imagesSerie->channels().contains(channel))
        d->imagesSerie->setChannel(channel);

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(d->imagesSerie->image());
    converter->convert();
    d->image = static_cast<vtkImageData *>(converter->output());
    qDebug()<<"Visu: Image Data"<<d->image->GetDimensions()[0]<<d->image->GetDimensions()[1]<<d->image->GetDimensions()[2];

    delete converter;

    // d->image_interactor->image = image;
    // d->viewer = vtkSmartPointer<vtkImageViewer2>::New();
    // d->viewer->SetSliceOrientationToXY();
    // d->viewer->SetRenderWindow(d->view->interactor()->GetRenderWindow());
    // d->viewer->SetRenderer(d->view->renderer2D());
    // d->viewer->SetupInteractor(d->view->interactor());
    // d->viewer->GetWindowLevel()->SetOutputFormatToRGB();
    // d->viewer->SetInputData(d->image);
    // qDebug()<<"Visu: Image Viewer"<<d->viewer;

    double valueRange[2];
    d->image->GetPointData()->GetScalars()->GetRange(valueRange);

    vtkSmartPointer<vtkColorTransferFunction> color_function = vtkSmartPointer<vtkColorTransferFunction>::New();
    color_function->AddRGBPoint(valueRange[0],0,0,0);
    color_function->AddRGBPoint(valueRange[1],1,1,1);
    color_function->ClampingOn();
    color_function->Modified();

    vtkSmartPointer<vtkImageMapToColors> image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    image_color->SetLookupTable(color_function);
    image_color->SetOutputFormatToRGBA();
    image_color->SetInputData(d->image);
    image_color->Update();
    qDebug()<<"Visu: Image Color"<<image_color;

    int imageDims[3]; d->image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!d->planeWidget[i])
            d->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        d->planeWidget[i]->SetInputData(d->image);
        d->planeWidget[i]->SetPlaneOrientation(i);
        d->planeWidget[i]->RestrictPlaneToVolumeOn();
        d->planeWidget[i]->GetPlaneProperty()->SetColor(color);
        // d->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        d->planeWidget[i]->SetMarginSizeX(0);
        d->planeWidget[i]->SetMarginSizeY(0);
        d->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        d->planeWidget[i]->DisplayTextOn();
        d->planeWidget[i]->SetInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
        // d->view->renderer2D()->AddActor(d->planeWidget[i])
        d->planeWidget[i]->InteractionOn();
    }

    if(!d->volume_mapper)
        d->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    d->volume_mapper->SetInputData(d->image);
    d->volume_mapper->SetRequestedRenderModeToRayCast();
    d->volume_mapper->Modified();
    d->volume_mapper->Update();
    qDebug()<<"Visu: Volume Mapper"<<d->volume_mapper;

    if(!d->volume) {
        d->volume = vtkSmartPointer<vtkVolume>::New();
        d->view->renderer3D()->AddActor(d->volume);
    }

    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(valueRange[0],0.00);
    opacity->AddPoint(valueRange[1],alpha);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(opacity);
    property->SetColor(color_function);
    property->ShadeOff();
    property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    d->volume->SetMapper(d->volume_mapper);
    d->volume->SetProperty(property);
    d->volume->Modified();
    d->volume->Update();

    // d->view->renderer2D()->ResetCamera();
    // d->view->renderer3D()->ResetCamera();

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        d->orientation = value;
        qDebug()<<"Visu: Set orientation"<<value;
        // d->viewer->SetSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        // d->viewer->SetSlice(value);
        d->planeWidget[d->orientation]->SetSliceIndex(value/d->image->GetSpacing()[d->orientation]);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { 
        for (int o=0;o<3;o++)
            d->planeWidget[o]->Off();
        this->render(); 
    });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { 
        for (int o=0;o<3;o++)
            d->planeWidget[o]->On();
        this->render();
    });
    connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (d->image->GetDimensions()[0]-1)*d->image->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (d->image->GetDimensions()[1]-1)*d->image->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (d->image->GetDimensions()[2]-1)*d->image->GetSpacing()[2];
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationImagesSerie::render(void)
{
    // d->updateOpacity();
    d->view->render();
}


//
// gnomonVisualizationImagesSerie.cpp ends here
