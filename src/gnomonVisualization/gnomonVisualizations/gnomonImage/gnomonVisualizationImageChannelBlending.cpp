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

#include "gnomonVisualizationImageChannelBlending.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonWidgets>
#include <dtkImagingCore>

#include "gnomonView/gnomonViewForm.h"
#include "gnomonActor/gnomonImage/gnomonImageDataChannelBlending.h"
#include "gnomonActor/gnomonImageData/gnomonActorImageRGBAVolume.h"
#include "gnomonActor/gnomonImageData/gnomonActor2DImageRGBAWidget.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImageChannelBlendingPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImageChannelBlendingPrivate
{
public:
    gnomonImageSeries *imageSeries;
    gnomonImage *image;

public:
    int orientation = 2;

public:
    vtkSmartPointer<vtkImageData> image_data = nullptr;

    gnomonImageDataChannelBlending *blending = nullptr;

    gnomonActor2DImageRGBAWidget *actor2D = nullptr;
    gnomonActorImageRGBAVolume *volume = nullptr;

public:
    QMap<QString, gnomonLookupTable> channelLookupTables;

public:
    QMap<int, QString> defaultColormaps;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImageChannelBlending
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImageChannelBlending::gnomonVisualizationImageChannelBlending(void) : gnomonAbstractVisualizationImage(), dd(new gnomonVisualizationImageChannelBlendingPrivate)
{

    dd->image = Q_NULLPTR;

    // d->parameters["channel"] = new gnomonCoreParameterString("", {""}, "Image channel to be displayed");
    // d->parameters["value_range"] = new gnomonCoreParameterIntRange(0, 255, 0, 255, "Value range for display ramps");
    // d->parameters["colormap"] = new gnomonCoreParameterLookupTable(new gnomonLookupTable("Greys"), "Colormap to apply to the image channel");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the image rendering");


    dd->defaultColormaps[0] = "0CMY_cyan";
    dd->defaultColormaps[1] = "0CMY_yellow";
    dd->defaultColormaps[2] = "0CMY_magenta";
    dd->defaultColormaps[3] = "0RGB_green";
    dd->defaultColormaps[4] = "0RGB_red";
    dd->defaultColormaps[5] = "0RGB_blue";

}

gnomonVisualizationImageChannelBlending::~gnomonVisualizationImageChannelBlending(void)
{
    this->clear();

    delete dd;

    dd = NULL;
}

void gnomonVisualizationImageChannelBlending::clear(void)
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

void gnomonVisualizationImageChannelBlending::setImage(gnomonImageSeries *image)
{
    dd->imageSeries = image;
    dd->image = dynamic_cast<gnomonImage *>(image->current());

    this->setParameter("alpha",1.0);

    dd->channelLookupTables.clear();

    QList<QString> parameterNames = d->parameters.keys();

    for (const auto& parameterName : parameterNames) {
        if(parameterName.contains("lookuptable")) {
            delete d->parameters[parameterName];
            d->parameters.remove(parameterName);
        }
    }

    QString channel = dd->image->channels()[0];
    QList<double> valueRange = {0,1};
    if (dd->image->image(channel)->storageType() == QMetaType::UChar) {
        valueRange[1] = 255;
    } else if (dd->image->image(channel)->storageType() == QMetaType::UShort) {
        valueRange[1] = 65535;
    }

    qDebug()<<Q_FUNC_INFO<<dd->image->channels();

    if(dd->image->channels().size()==1) {
        if (dd->channelLookupTables.contains("")) {
            dd->channelLookupTables.remove("");
        }
        dd->channelLookupTables[""] = gnomonLookupTable("grey", valueRange, true);
        d->parameters["lookuptable"] = new gnomonCoreParameterLookupTable(dd->channelLookupTables[""], "Lookuptable to apply to the image");
    } else {
        int iChannel = 0;
        for (const auto& channelName : dd->image->channels()) {
            qDebug()<<Q_FUNC_INFO<<"Channel"<<channelName;
            if (dd->channelLookupTables.contains(channelName)) {
                dd->channelLookupTables.remove(channelName);
            }
            dd->channelLookupTables[channelName] = gnomonLookupTable(dd->defaultColormaps[iChannel], valueRange, true);
            d->parameters[channelName+"\nlookuptable"] = new gnomonCoreParameterLookupTable(dd->channelLookupTables[channelName], "Lookuptable to apply to the "+channelName+" image channel");
            iChannel++;
        }
    }
}

void gnomonVisualizationImageChannelBlending::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();

    dd->actor2D->setOpacity(alpha);
    dd->volume->setOpacity(alpha);
}

QImage gnomonVisualizationImageChannelBlending::imageRendering(void)
{
    double bounds[6];
    dd->image_data->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(dd->volume);

    return this->offscreenImageRendering();
}

void gnomonVisualizationImageChannelBlending::update(void)
{
    if(!dd->image)
        return;

    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    // QList<int> value_range = ((gnomonCoreParameterIntRange *)d->parameters["value_range"])->value();
    // QMap<double, QColor> colormap = ((gnomonCoreParameterLookupTable *)d->parameters["colormap"])->value()->colorMap();


    qDebug()<<Q_FUNC_INFO<<dd->image->channels();
    QMap<QString,vtkImageData *> channelImages;
    for (const auto& channelName : dd->image->channels()) {
        dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
        converter->setInput(dd->image->image(channelName));
        converter->convert();
        channelImages[channelName] = static_cast<vtkImageData *>(converter->output());
        delete converter;
    }
    qDebug()<<Q_FUNC_INFO<<channelImages.keys();


    dd->channelLookupTables.clear();
    if(dd->image->channels().size()==1) {
        dd->channelLookupTables[""] = ((gnomonCoreParameterLookupTable *)d->parameters["lookuptable"])->value();
    } else {
        for (const auto& channelName : dd->image->channels()) {
            dd->channelLookupTables[channelName] = ((gnomonCoreParameterLookupTable *)d->parameters[channelName+"\nlookuptable"])->value();
        }
    }

    if (!dd->blending) {
        dd->blending = gnomonImageDataChannelBlending::New();
    }
    dd->blending->setImageChannels(channelImages);
    dd->blending->setChannelLookupTables(dd->channelLookupTables);
    // blending->setColorMap(colormap);
    // blending->setValueRange(value_range);
    dd->blending->update();
    dd->image_data = dd->blending;

    if (!dd->actor2D) {
        dd->actor2D = gnomonActor2DImageRGBAWidget::New();
    }
    dd->actor2D->setImage(dd->image_data);
    dd->actor2D->setInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
    dd->actor2D->setOpacity(alpha);
    dd->actor2D->update();

    if (!dd->volume) {
        dd->volume = gnomonActorImageRGBAVolume::New();
        d->view->renderer3D()->AddActor(dd->volume);
    }
    dd->volume->setInteractor(d->view->interactor());
    dd->volume->setImage(dd->image_data);

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->actor2D->setSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        dd->actor2D->setSlice(value);
        this->render();
    });

    d->connect3D = connect(d->view, &gnomonViewForm::switchedTo3D, [=] () {
        dd->actor2D->hide();
        this->render();
    });
    d->connect2D = connect(d->view, &gnomonViewForm::switchedTo2D, [=] () {
        dd->actor2D->show();
        this->render();
    });

    d->connectXY = connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    d->connectYZ = connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    d->connectXZ = connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

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

void gnomonVisualizationImageChannelBlending::render(void)
{
    this->updateOpacity();
    d->view->render();
}


QMap<QString, gnomonCoreParameter *> gnomonVisualizationImageChannelBlending::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationImageChannelBlending::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationImageChannelBlending::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
//            d->parameters[param] = parameters[param];
            d->parameters[param]->copy(parameters[param]);
        }
    }
}

void gnomonVisualizationImageChannelBlending::onTimeChanged(double value)
{
    qDebug()<<Q_FUNC_INFO<<"Time changed"<<value;
    if (dd->imageSeries->times().contains(value)) {
        dd->image = dynamic_cast<gnomonImage *>(dd->imageSeries->at(value));
        this->update();
    }
    this->render();
}

//
// gnomonVisualizationImageChannelBlending.cpp ends here
