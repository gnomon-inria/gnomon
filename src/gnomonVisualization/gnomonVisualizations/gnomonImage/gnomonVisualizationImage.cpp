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

#include "gnomonVisualizationImage.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonWidgets>
#include <dtkImagingCore>

#include "gnomonView/gnomonViewForm.h"
#include "gnomonActor/gnomonImageData/gnomonActorImageVolume.h"
#include "gnomonActor/gnomonImageData/gnomonActor2DImageWidget.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImagePrivate
{
public:
    gnomonImageSeries *imageSeries;
    gnomonImage *image;

public:
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image_data = nullptr;

    gnomonActor2DImageWidget *actor2D = nullptr;
    gnomonActorImageVolume *volume = nullptr;

public:
    QMap<QString, QMap<double, QColor> > channelColormaps;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImage
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImage::gnomonVisualizationImage(void) : gnomonAbstractVisualizationImage(), dd(new gnomonVisualizationImagePrivate)
{
    dd->imageSeries = Q_NULLPTR;
    dd->image = Q_NULLPTR;

    d->parameters["channel"] = new gnomonCoreParameterString("", {""}, "Image channel to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterIntRange(0, 255, 0, 255, "Value range for display ramps");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("grey", "Colormap to apply to the image");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the image rendering");

    connect(d->parameters["channel"], &gnomonCoreParameter::valueChanged, [=] () {
        if(!dd->image)
            return;
        this->updateChannelColorMap();
        emit parametersChanged();
    });

}

gnomonVisualizationImage::~gnomonVisualizationImage(void)
{
    this->clear();

    delete dd;

    dd = NULL;
}

void gnomonVisualizationImage::clear(void)
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

    disconnect(d->connect3D);
    disconnect(d->connect2D);
    disconnect(d->connectXY);
    disconnect(d->connectXZ);
    disconnect(d->connectYZ);
}

void gnomonVisualizationImage::setImage(gnomonImageSeries *image)
{
    dd->imageSeries = image;
    dd->image = dynamic_cast<gnomonImage *>(image->current());

    this->setParameter("alpha",1.0);

    dd->channelColormaps.clear();
    if(dd->image->channels().size()==1) {
        delete d->parameters["channel"];
        d->parameters.remove("channel");
    } else {
        if((!d->parameters.contains("channel"))||(!d->parameters["channel"])) {
            d->parameters["channel"] = new gnomonCoreParameterString("", {""}, "Image channel to be displayed");
        }
        qDebug()<<Q_FUNC_INFO<<d->parameters["channel"];
        gnomonCoreParameterString *channelParam = (gnomonCoreParameterString *)d->parameters["channel"];
        channelParam->setValues(dd->image->channels());
        channelParam->setValue(dd->image->channels()[0]);
    }

    gnomonCoreParameterIntRange *valueRangeParam = (gnomonCoreParameterIntRange *)d->parameters["value_range"];
    valueRangeParam->setMinimumValue(0);

    QString channel = dd->image->channels()[0];
    if (dd->image->image(channel)->storageType() == QMetaType::UChar) {
        valueRangeParam->setMaximumValue(255);
        valueRangeParam->setValue(0,255);
    } else if (dd->image->image(channel)->storageType() == QMetaType::UShort) {
        valueRangeParam->setMaximumValue(65535);
        valueRangeParam->setValue(0,65535);
    }
}

void gnomonVisualizationImage::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();

    dd->actor2D->setOpacity(alpha);
    dd->volume->setOpacity(alpha);
}

void gnomonVisualizationImage::updateChannelColorMap(void)
{
    if(dd->image->channels().size()>1) {
        QString channel = ((gnomonCoreParameterString *)d->parameters["channel"])->value();

        if(dd->channelColormaps.contains(channel)) {
            ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(dd->channelColormaps[channel]);
        }
    }
}

QImage gnomonVisualizationImage::imageRendering(void)
{
    double bounds[6];
    dd->image_data->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(dd->volume);

    return this->offscreenImageRendering();
}

void gnomonVisualizationImage::update(void)
{
    if(!dd->image)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();

    QString channel;
    if(dd->image->channels().size()>1) {
        channel = ((gnomonCoreParameterString *)d->parameters["channel"])->value();
        dd->channelColormaps[channel] = colormap;
    } else {
        channel = "";
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(dd->image->image(channel));
    converter->convert();
    dd->image_data = static_cast<vtkImageData *>(converter->output());
    delete converter;


    if (!dd->actor2D) {
        dd->actor2D = gnomonActor2DImageWidget::New();
    }
    dd->actor2D->setImage(dd->image_data);
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
    dd->volume->setImage(dd->image_data);
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
    bounds[1] = (dd->image_data->GetDimensions()[0]-1)*dd->image_data->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (dd->image_data->GetDimensions()[1]-1)*dd->image_data->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (dd->image_data->GetDimensions()[2]-1)*dd->image_data->GetSpacing()[2];
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationImage::render(void)
{
    this->updateOpacity();
    d->view->render();
}

QMap<QString, gnomonCoreParameter *> gnomonVisualizationImage::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationImage::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationImage::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
//            d->parameters[param] = parameters[param];
            d->parameters[param]->copy(parameters[param]);
        }
    }
}

void gnomonVisualizationImage::onTimeChanged(double value)
{
    qDebug()<<Q_FUNC_INFO<<"Time changed"<<value;
    this->render();
}

//
// gnomonVisualizationImage.cpp ends here
