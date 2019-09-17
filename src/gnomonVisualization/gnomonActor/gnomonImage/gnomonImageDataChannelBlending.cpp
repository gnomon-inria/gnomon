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

#include "gnomonImageDataChannelBlending.h"

#include <gnomonWidgets>

//TODO: Image
//#include <dtkImagingCore>

#include <QtCore>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonImageDataChannelBlendingPrivate
// /////////////////////////////////////////////////////////////////

class gnomonImageDataChannelBlendingPrivate
{
public:
    QMap<QString, vtkSmartPointer<vtkImageData> > imageChannels;

    QMap<QString, vtkSmartPointer<vtkColorTransferFunction> > colorFunctions;
    QMap<QString, vtkSmartPointer<vtkPiecewiseFunction> > opacities;

public:

    int value_range[2];
    QMap<QString, gnomonLookupTable> channelLookupTables;

    bool modified;

public slots:
    void updateOpacities(void);
    void updateColorFunctions(void);
};

void gnomonImageDataChannelBlendingPrivate::updateOpacities(void)
{
    for (const auto& channelName : this->imageChannels.keys()) {
        gnomonLookupTable lut = this->channelLookupTables[channelName];

        QList<double> value_range = lut.valueRange();

        this->opacities[channelName]->RemoveAllPoints();
        this->opacities[channelName]->AddPoint(value_range[0],0.00);
        this->opacities[channelName]->AddPoint(value_range[1],1.00);
    }
}

void gnomonImageDataChannelBlendingPrivate::updateColorFunctions(void)
{
    for (const auto& channelName : this->imageChannels.keys()) {
        gnomonLookupTable lut = this->channelLookupTables[channelName];

        QList<double> value_range = lut.valueRange();
        QMap<double, QColor> colormap = lut.colorMap();

        this->colorFunctions[channelName]->RemoveAllPoints();
        for (const auto& val : colormap.keys()) {
            double node = val*value_range[1] + (1-val)*value_range[0];
            this->colorFunctions[channelName]->AddRGBPoint(node, colormap[val].red()/255., colormap[val].green()/255., colormap[val].blue()/255.);
        }
        this->colorFunctions[channelName]->ClampingOn();
        this->colorFunctions[channelName]->Modified();
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonImageDataChannelBlending
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonImageDataChannelBlending);

void gnomonImageDataChannelBlending::setImageChannels(QMap<QString, vtkImageData *> imageChannels)
{
    d->imageChannels.clear();
    d->colorFunctions.clear();
    d->opacities.clear();

    for (const auto& channelName : imageChannels.keys()) {
        d->imageChannels[channelName] = imageChannels[channelName];
        d->colorFunctions[channelName] = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->opacities[channelName] = vtkSmartPointer<vtkPiecewiseFunction>::New();
    }

    this->modified();
}

void gnomonImageDataChannelBlending::setChannelLookupTables(const QMap<QString, gnomonLookupTable>& value)
{
    d->channelLookupTables = value;
}

void gnomonImageDataChannelBlending::modified(void)
{
    d->modified = true;
}

void gnomonImageDataChannelBlending::update(void)
{
    if(d->imageChannels.size()==0)
        return;

    if(d->channelLookupTables.size() != d->imageChannels.size())
        return;

    for (const auto& channel : d->imageChannels.keys()) {
        if(!d->channelLookupTables.contains(channel)) {
            return;
        }
    }

    d->updateColorFunctions();
    d->updateOpacities();

    double image_range[2]={65535,0};
    for (const auto& channel : d->imageChannels.keys()) {
        double channel_range[2];
        d->imageChannels[channel]->GetPointData()->GetScalars()->GetRange(channel_range);
        image_range[0] = std::min(image_range[0],channel_range[0]);
        image_range[1] = std::max(image_range[1],channel_range[1]);
    }

    QMap<QString, vtkSmartPointer<vtkLookupTable> > channelLuts;
    for (const auto& channelName : d->imageChannels.keys()) {

        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        int n_values = 1000;
        lut->SetNumberOfTableValues(n_values);
        for(size_t i = 0; i < n_values; ++i) {
            double val = (i*image_range[1] + (n_values-i)*image_range[0])/(n_values-1.);
            double *rgb = d->colorFunctions[channelName]->GetColor(val);
            double a = d->opacities[channelName]->GetValue(val);
            lut->SetTableValue(i,rgb[0],rgb[1],rgb[2],a);
        }
        lut->SetTableRange(image_range);
        channelLuts[channelName] = lut;
    }


    vtkSmartPointer<vtkImageBlend> blend = vtkSmartPointer<vtkImageBlend>::New();
    blend->SetBlendModeToNormal();

    for (const auto& channelName : d->imageChannels.keys()) {
        if(d->channelLookupTables[channelName].visibility()) {
            vtkSmartPointer<vtkImageMapToColors> colors = vtkSmartPointer<vtkImageMapToColors>::New();
            colors->SetInputData(d->imageChannels[channelName]);
            colors->SetLookupTable(channelLuts[channelName]);
            colors->SetOutputFormatToRGBA();
            colors->Update();

            blend->AddInputData(colors->GetOutput());
        }
    }
    for (int i=0;i<blend->GetNumberOfInputs();i++) {
        blend->SetOpacity(i,1.);
    }
    blend->Update();
    vtkSmartPointer<vtkImageData> blending = blend->GetOutput();

    this->DeepCopy(blending);

    d->modified = false;
}

gnomonImageDataChannelBlending::gnomonImageDataChannelBlending(void) : vtkImageData(), d(new gnomonImageDataChannelBlendingPrivate)
{
}

gnomonImageDataChannelBlending::~gnomonImageDataChannelBlending(void)
{
    delete d;
    d = NULL;
}

//
// gnomonImageDataChannelBlending.cpp ends here
