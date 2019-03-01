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

#include <gnomonCore/gnomonForm/gnomonImagesSerie>
#include <gnomonCore/gnomonCoreParameter>
#include "gnomonCoreParameterColor.h"
#include <dtkImagingCore>

#include "gnomonViewForm.h"
#include "gnomonActorImageVolume.h"
#include "gnomonActor2DImageWidget.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSeriePrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImagesSeriePrivate
{
public:
    gnomonImagesSerie *imagesSerie;

public:
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;

    gnomonActor2DImageWidget *actor2D = nullptr;
    gnomonActorImageVolume *volume = nullptr;

public:
    QMap<QString, QMap<double, QColor> > channelColormaps;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagesSerie
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImagesSerie::gnomonVisualizationImagesSerie(void) : gnomonAbstractVisualization(), dd(new gnomonVisualizationImagesSeriePrivate)
{
    dd->imagesSerie = Q_NULLPTR;

    d->parameters["channel"] = new gnomonCoreParameterString("", {""}, "Image channel to be displayed");
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
    if (dd->volume) {
        d->view->renderer3D()->RemoveActor(dd->volume);
        dd->volume->Delete();
        dd->volume = nullptr;
    }

    if (dd->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }

    delete dd;

    dd = NULL;
}

void gnomonVisualizationImagesSerie::setImagesSerie(gnomonImagesSerie *imagesSerie)
{
    dd->imagesSerie = imagesSerie;

    this->setParameter("alpha",1.0);

    dd->channelColormaps.clear();
    if(dd->imagesSerie->channels().size()==1) {
        delete d->parameters["channel"];
        d->parameters.remove("channel");
    } else {
        if((!d->parameters.contains("channel"))||(!d->parameters["channel"])) {
            d->parameters["channel"] = new gnomonCoreParameterString("", {""}, "Image channel to be displayed");
        }
        qDebug()<<Q_FUNC_INFO<<d->parameters["channel"];
        gnomonCoreParameterString *channelParam = (gnomonCoreParameterString *)d->parameters["channel"];
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

void gnomonVisualizationImagesSerie::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();

    dd->actor2D->setOpacity(alpha);
    dd->volume->setOpacity(alpha);
}

void gnomonVisualizationImagesSerie::updateChannelColorMap(void)
{
    if(dd->imagesSerie->channels().size()>1) {
        QString channel = ((gnomonCoreParameterString *)d->parameters["channel"])->value();

        if(dd->channelColormaps.contains(channel)) {
            ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(dd->channelColormaps[channel]);
        }
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
    QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
 
    QString channel;
    if(dd->imagesSerie->channels().size()>1) {
        channel = ((gnomonCoreParameterString *)d->parameters["channel"])->value();
        if(dd->imagesSerie->channels().contains(channel))
            dd->imagesSerie->setChannel(channel);
        dd->channelColormaps[channel] = colormap;
    } else {
        channel = "";
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(dd->imagesSerie->image());
    converter->convert();
    dd->image = static_cast<vtkImageData *>(converter->output());
    delete converter;

    
    if (!dd->actor2D) {
        dd->actor2D = gnomonActor2DImageWidget::New();
    }
    dd->actor2D->setImage(dd->image);
    dd->actor2D->setInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);
    dd->actor2D->setOpacity(alpha);
    dd->actor2D->update();
    
    if (!dd->volume) {
        dd->volume = gnomonActorImageVolume::New();
        d->view->renderer3D()->AddActor(dd->volume);
    }
    dd->volume->setInteractor(d->view->interactor());
    dd->volume->setImage(dd->image);
    dd->volume->setColorMap(colormap);
    dd->volume->setValueRange(value_range);

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

void gnomonVisualizationImagesSerie::render(void)
{
    this->updateOpacity();
    d->view->render();
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

void gnomonVisualizationImagesSerie::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
//            d->parameters[param] = parameters[param];
            d->parameters[param]->copy(parameters[param]);
        }
    }
}

//
// gnomonVisualizationImagesSerie.cpp ends here
