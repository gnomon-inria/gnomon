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
    QMap<QString, vtkSmartPointer<vtkImageData> > images;
    
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

    QMap<QString, vtkSmartPointer<vtkColorTransferFunction> > colorFunctions;
    vtkSmartPointer<vtkPiecewiseFunction> opacity = nullptr;

    vtkRenderWindowInteractor *interactor;

public:
    int orientation;
    
    double alpha;
    int value_range[2];
    // QMap<double,QColor> colormap;
    QMap<QString, QColor> colors;

    bool modified;

public slots:
    void updateOpacity(void);
    void updateColorFunction(void);
};

void gnomonActor2DImageChannelBlendingWidgetPrivate::updateOpacity(void)
{
    if (!this->opacity)
        return;

    this->opacity->RemoveAllPoints();
    this->opacity->AddPoint(this->value_range[0],0.00);
    this->opacity->AddPoint(this->value_range[1],this->alpha);
}

void gnomonActor2DImageChannelBlendingWidgetPrivate::updateColorFunction(void)
{
    int channelId = 0;
    for (const auto& channel : this->images.keys()) {
        QColor color;
        switch(channelId) {
            case 0 : color = QColor(255, 255, 255, 255); break;
            case 1 : color = QColor(255, 0  , 0  , 255); break;
            case 2 : color = QColor(0  , 255, 0  , 255); break;
            case 3 : color = QColor(0  , 0  , 255, 255); break;
            case 4 : color = QColor(255, 255, 0  , 255); break;
            case 5 : color = QColor(255, 0  , 255, 255); break;
            case 6 : color = QColor(0  , 255, 255, 255); break;
        }

        this->colorFunctions[channel]->RemoveAllPoints();
        this->colorFunctions[channel]->AddRGBPoint(this->value_range[0], 0, 0, 0);
        // this->colorFunctions[channel]->AddRGBPoint(0.75*this->value_range[1] + (1-0.75)*this->value_range[0], color.red()/255., color.green()/255., color.blue()/255.);
        // this->colorFunctions[channel]->AddRGBPoint(this->value_range[1], 1, 1, 1);
        this->colorFunctions[channel]->AddRGBPoint(this->value_range[1], color.red()/255., color.green()/255., color.blue()/255.);

        this->colorFunctions[channel]->ClampingOn();
        this->colorFunctions[channel]->Modified();

        channelId++;
    }
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

void gnomonActor2DImageChannelBlendingWidget::setImages(QMap<QString, vtkImageData *> images)
{
    d->images.clear();
    d->colorFunctions.clear();

    for (const auto& channel : images.keys()) {
        qDebug()<<Q_FUNC_INFO<<"Add channel"<<channel<<images[channel];
        d->images[channel] = images[channel];
        d->colorFunctions[channel] = vtkSmartPointer<vtkColorTransferFunction>::New();
    }

    this->modified();
    this->update();
}

void gnomonActor2DImageChannelBlendingWidget::modified(void)
{
    d->modified = true;
}

void gnomonActor2DImageChannelBlendingWidget::update(void)
{
    if(d->images.size()==0)
        return;

    // if(!d->colorFunction)
    //     d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    d->updateColorFunction();

    if (!d->opacity)
        d->opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    d->updateOpacity();

    double image_range[2]={65535,0};
    for (const auto& channel : d->images.keys()) {
        double channel_range[2];
        d->images[channel]->GetPointData()->GetScalars()->GetRange(channel_range);
        image_range[0] = std::min(image_range[0],channel_range[0]);
        image_range[1] = std::max(image_range[1],channel_range[1]);
    }


    QMap<QString, vtkSmartPointer<vtkLookupTable> > channelLuts;
    for (const auto& channel : d->images.keys()) {

        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        int n_values = 1000;
        lut->SetNumberOfTableValues(n_values);
        for(size_t i = 0; i < n_values; ++i) {
            double val = (i*image_range[1] + (n_values-i)*image_range[0])/(n_values-1.);
            double *rgb = d->colorFunctions[channel]->GetColor(val);
            double a = d->opacity->GetValue(val);
            lut->SetTableValue(i,rgb[0],rgb[1],rgb[2],a);
        }
        lut->SetTableRange(image_range);
        channelLuts[channel] = lut;
    }


    vtkSmartPointer<vtkImageBlend> blend = vtkSmartPointer<vtkImageBlend>::New();
    blend->SetBlendModeToNormal();
    // blend->SetBlendModeToCompound();

    for (const auto& channel : d->images.keys()) {
        if (blend->GetNumberOfInputs()<7) {
            vtkSmartPointer<vtkImageMapToColors> colors = vtkSmartPointer<vtkImageMapToColors>::New();
            colors->SetInputData(d->images[channel]);
            colors->SetLookupTable(channelLuts[channel]);
            colors->SetOutputFormatToRGBA();
            colors->Update();

            blend->AddInputData(colors->GetOutput());
        }
        qDebug()<<Q_FUNC_INFO<<channel<<":"<<blend->GetNumberOfInputs();
    }
    for (int i=0;i<blend->GetNumberOfInputs();i++) {
        blend->SetOpacity(i,1.);
    }
    blend->Update();
    vtkSmartPointer<vtkImageData> blending = blend->GetOutput();

    qDebug()<<Q_FUNC_INFO<<blending;
    qDebug()<<Q_FUNC_INFO<<blending->GetDimensions()[0]<<blending->GetDimensions()[1]<<blending->GetDimensions()[2];
    qDebug()<<Q_FUNC_INFO<<blending->GetSpacing()[0]<<blending->GetSpacing()[1]<<blending->GetSpacing()[2];
    qDebug()<<Q_FUNC_INFO<<blending->GetPointData()->GetNumberOfComponents();
    qDebug()<<Q_FUNC_INFO<<blending->GetScalarPointer(0,0,0);

    int imageDims[3]; d->images.values()[0]->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        if(!d->planeWidget[i])
            d->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        d->planeWidget[i]->SetInputData(blend->GetOutput());
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
    d->planeWidget[d->orientation]->SetSliceIndex(value/d->images.values()[0]->GetSpacing()[d->orientation]);
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

void gnomonActor2DImageChannelBlendingWidget::setValueRange(const QList<int>& value)
{
    d->value_range[0] = value[0];
    d->value_range[1] = value[1];
    d->updateOpacity();
    d->updateColorFunction();
    d->interactor->Render();
}

void gnomonActor2DImageChannelBlendingWidget::setColorMap(const QMap<double,QColor>& value)
{
    // d->colormap = value;
    // d->updateColorFunction();
    // d->interactor->Render();
}

gnomonActor2DImageChannelBlendingWidget::gnomonActor2DImageChannelBlendingWidget(void) : gnomonActor(), d(new gnomonActor2DImageChannelBlendingWidgetPrivate)
{
    // d->image = Q_NULLPTR;

    // d->colorFunction = Q_NULLPTR;
    d->opacity = Q_NULLPTR;

    d->orientation = 2;

    d->alpha = 1;
    d->value_range[0] = 0.;
    d->value_range[1] = 1.;
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
