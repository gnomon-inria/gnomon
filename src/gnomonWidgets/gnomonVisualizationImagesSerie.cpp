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

#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkImagePlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>


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

    vtkSmartPointer<vtkColorTransferFunction> color_function = nullptr;
    vtkSmartPointer<vtkPiecewiseFunction> opacity = nullptr;

public:
    QMap<QString, QMap<double, QColor> > channelColormaps;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerie
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImagesSerie::gnomonVisualizationImagesSerie(gnomonViewForm* view) : gnomonAbstractVisualization(view), dd(new gnomonVisualizationImagesSeriePrivate)
{
    dd->imagesSerie = Q_NULLPTR;

    d->parameters["channel"] = new gnomonCoreParameterStringList("", {""}, "Image channel to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterIntRange(0, 255, 0, 255, "Value range for display ramps");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("grey", "Colormap to apply to the image");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the image rendering");

    connect(d->parameters["channel"], &gnomonCoreParameter::valueChanged, [=] () {
        if(!dd->imagesSerie)
            return;
        this->updateChannelColorMap();
        emit parametersChanged();
    });

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
    QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    
    dd->opacity->AddPoint(value_range[0],0.00);
    dd->opacity->AddPoint(value_range[1],alpha);

    dd->volume->GetProperty()->SetScalarOpacity(dd->opacity);
}

void gnomonVisualizationImagesSerie::updateChannelColorMap(void)
{
    QString channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();

    if(dd->channelColormaps.contains(channel)) {
        ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(dd->channelColormaps[channel]);
    }
}

QImage gnomonVisualizationImagesSerie::imageRendering(void)
{
    d->updateOffscreenRenderer(dd->image->GetBounds());

    d->offscreenRenderer->AddActor(dd->volume);

    return d->offscreenImageRendering();
}

void gnomonVisualizationImagesSerie::update(void)
{
    if(!dd->imagesSerie)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    QString channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();
    QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();

    dd->channelColormaps[channel] = colormap;   

    if(dd->imagesSerie->channels().contains(channel))
        dd->imagesSerie->setChannel(channel);

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(dd->imagesSerie->image());
    converter->convert();
    dd->image = static_cast<vtkImageData *>(converter->output());

    delete converter;

    if(!dd->color_function)
        dd->color_function = vtkSmartPointer<vtkColorTransferFunction>::New();
    dd->color_function->RemoveAllPoints();
    // dd->color_function->AddRGBPoint(value_range[0],0,0,0);
    // dd->color_function->AddRGBPoint(value_range[1],1,1,1);
    for (const auto& val : colormap.keys()) {
        double node = val*value_range[1] + (1-val)*value_range[0];
        dd->color_function->AddRGBPoint(node, colormap[val].red()/255., colormap[val].green()/255., colormap[val].blue()/255.);
    }

    dd->color_function->ClampingOn();
    dd->color_function->Modified();

    if (!dd->opacity)
        dd->opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    dd->opacity->RemoveAllPoints();
    dd->opacity->AddPoint(value_range[0],0.00);
    dd->opacity->AddPoint(value_range[1],alpha);

    double image_range[2];
    dd->image->GetPointData()->GetScalars()->GetRange(image_range);

    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    int n_values = 1000;
    lut->SetNumberOfTableValues(n_values);
    for(size_t i = 0; i < n_values; ++i) {
        double val = (i*image_range[1] + (n_values-i)*image_range[0])/(n_values-1.);
        double *rgb = dd->color_function->GetColor(val);
        double a = dd->opacity->GetValue(val);
        lut->SetTableValue(i,rgb[0],rgb[1],rgb[2],a);
    }
    lut->SetTableRange(image_range);

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
        dd->planeWidget[i]->SetLookupTable(lut);
        // dd->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        dd->planeWidget[i]->SetMarginSizeX(0);
        dd->planeWidget[i]->SetMarginSizeY(0);
        dd->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        dd->planeWidget[i]->DisplayTextOn();
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

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(dd->opacity);
    property->SetColor(dd->color_function);
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
