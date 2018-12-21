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

#include "gnomonVisualizationImagesSerieChannelBlending.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonImagesSerie>
#include <gnomonCore/gnomonCoreParameter>
#include <dtkImagingCore>

#include "gnomonViewForm.h"
#include "gnomonImageDataChannelBlending.h"
#include "gnomonActorImageRGBAVolume.h"
#include "gnomonActor2DImageRGBAWidget.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerieChannelBlendingPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImagesSerieChannelBlendingPrivate
{
public:
    gnomonImagesSerie *imagesSerie;

public:
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;

    gnomonActor2DImageRGBAWidget *actor2D = nullptr; 
    gnomonActorImageRGBAVolume *volume = nullptr;

public:
    QMap<QString, QMap<double, QColor> > channelColormaps;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerieChannelBlending
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImagesSerieChannelBlending::gnomonVisualizationImagesSerieChannelBlending(gnomonViewForm* view) : gnomonAbstractVisualization(view), dd(new gnomonVisualizationImagesSerieChannelBlendingPrivate)
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

gnomonVisualizationImagesSerieChannelBlending::~gnomonVisualizationImagesSerieChannelBlending(void)
{
    delete dd;

    dd = NULL;
}

void gnomonVisualizationImagesSerieChannelBlending::setImagesSerie(gnomonImagesSerie *imagesSerie)
{
    dd->imagesSerie = imagesSerie;

    this->setParameter("alpha",1.0);

    dd->channelColormaps.clear();
    if(dd->imagesSerie->channels().size()==1) {
        delete d->parameters["channel"];
        d->parameters.remove("channel");
    } else {
        if((!d->parameters.contains("channel"))||(!d->parameters["channel"])) {
            d->parameters["channel"] = new gnomonCoreParameterStringList("", {""}, "Image channel to be displayed");
        }
        qDebug()<<Q_FUNC_INFO<<d->parameters["channel"];
        gnomonCoreParameterStringList *channelParam = (gnomonCoreParameterStringList *)d->parameters["channel"];
        channelParam->setValues(dd->imagesSerie->channels());
        channelParam->setValue(dd->imagesSerie->channel());
    }

    gnomonCoreParameterIntRange *valueRangeParam = (gnomonCoreParameterIntRange *)d->parameters["value_range"];
    valueRangeParam->setMinimumValue(0);
    if (dd->imagesSerie->image()->storageType() == QMetaType::UChar) {
        valueRangeParam->setMaximumValue(255);
        valueRangeParam->setValue(0,255);
    } else if (dd->imagesSerie->image()->storageType() == QMetaType::UShort) {
        valueRangeParam->setMaximumValue(65535);
        valueRangeParam->setValue(0,65535);
    }
}

void gnomonVisualizationImagesSerieChannelBlending::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();

    dd->actor2D->setOpacity(alpha);
    dd->volume->setOpacity(alpha);
}

void gnomonVisualizationImagesSerieChannelBlending::updateChannelColorMap(void)
{
    if(dd->imagesSerie->channels().size()>1) {
        QString channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();

        if(dd->channelColormaps.contains(channel)) {
            ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(dd->channelColormaps[channel]);
        }
    }
}

QImage gnomonVisualizationImagesSerieChannelBlending::imageRendering(void)
{
    d->updateOffscreenRenderer(dd->image->GetBounds());

    d->offscreenRenderer->AddActor(dd->volume);

    return d->offscreenImageRendering();
}

void gnomonVisualizationImagesSerieChannelBlending::update(void)
{
    if(!dd->imagesSerie)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
 
    QString channel;
    if(dd->imagesSerie->channels().size()>1) {
        channel = ((gnomonCoreParameterStringList *)d->parameters["channel"])->value();
        if(dd->imagesSerie->channels().contains(channel))
            dd->imagesSerie->setChannel(channel);
        dd->channelColormaps[channel] = colormap;
    } else {
        channel = "";
    }

    QMap<QString,vtkImageData *> channelImages;
    for (const auto& channelName : dd->imagesSerie->channels()) {
        qDebug()<<Q_FUNC_INFO<<channelName;
        dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
        converter->setInput(dd->imagesSerie->image(channelName));
        converter->convert();
        channelImages[channelName] = static_cast<vtkImageData *>(converter->output());
        delete converter;
        qDebug()<<Q_FUNC_INFO<<channelName<<channelImages[channelName];
    }

    gnomonImageDataChannelBlending *blending = gnomonImageDataChannelBlending::New();
    blending->setImages(channelImages);
    blending->setColorMap(colormap);
    blending->setValueRange(value_range);
    blending->update();
    dd->image = blending;
    
    if (!dd->actor2D) {
        dd->actor2D = gnomonActor2DImageRGBAWidget::New();
    }
    dd->actor2D->setImage(dd->image);
    dd->actor2D->setInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
    dd->actor2D->setOpacity(alpha);
    dd->actor2D->update();
    
    if (!dd->volume) {
        dd->volume = gnomonActorImageRGBAVolume::New();
        d->view->renderer3D()->AddActor(dd->volume);
    }
    dd->volume->setInteractor(d->view->interactor());
    dd->volume->setImage(dd->image);

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->actor2D->setSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        dd->actor2D->setSlice(value);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { 
        dd->actor2D->hide();
        this->render(); 
    });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { 
        dd->actor2D->show();
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

void gnomonVisualizationImagesSerieChannelBlending::render(void)
{
    this->updateOpacity();
    d->view->render();
}


//
// gnomonVisualizationImagesSerieChannelBlending.cpp ends here
