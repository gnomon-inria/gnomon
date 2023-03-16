#include "gnomonVisualizationImage.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractFormVisualization_p.h"

#include <gnomonVisualization/gnomonCoreParameterColor.h>

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
    std::shared_ptr<gnomonImageSeries> imageSeries;
    std::shared_ptr<gnomonImage> image;

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

gnomonVisualizationImage::gnomonVisualizationImage(void) : gnomonAbstractVisualizationImage(), ddd(new gnomonVisualizationImagePrivate)
{
    d->parameters["channel"] = new dtk::d_inliststring("", {""}, "Image channel to be displayed");
    d->parameters["value_range"] = new dtk::d_range_int("value_range", {0, 255}, 0, 255, "Value range for display ramps");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the image");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the image rendering");

    d->parameters["channel"]->connect([=] (QVariant v) {
        if(!ddd->image)
            return;
        this->updateChannelColorMap();
        emit parametersChanged();
    });

}

gnomonVisualizationImage::~gnomonVisualizationImage(void)
{
    this->clear();
    delete ddd;
}

const QString gnomonVisualizationImage::pluginName(void)
{
    return  "gnomonVisualizationImage";
}

void gnomonVisualizationImage::clear(void)
{
    if (ddd->volume) {
        ((gnomonViewForm *) d->view)->renderer3D()->RemoveActor(ddd->volume);
        ddd->volume->Delete();
        ddd->volume = nullptr;
    }

    if (ddd->actor2D) {
//        disconnect(d->connectSliceOrientation);
//        disconnect(d->connectSlice);
        ((gnomonViewForm *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }

//    disconnect(d->connect3D);
//    disconnect(d->connect2D);
//    disconnect(d->connectXY);
//    disconnect(d->connectXZ);
//    disconnect(d->connectYZ);
}

void gnomonVisualizationImage::setVisible(bool visible)
{
    if (ddd->volume) {
        ddd->volume->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonVisualizationImage::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    ddd->imageSeries = image;
    ddd->image = image->current();

    this->setParameter("alpha",1.0);

    ddd->channelColormaps.clear();
    if(ddd->image->channels().size()==1) {
        //delete d->parameters["channel"];
        d->parameters.remove("channel");
    } else {
        if((!d->parameters.contains("channel"))||(!d->parameters["channel"])) {
            d->parameters["channel"] = new dtk::d_inliststring("", {""}, "Image channel to be displayed");
        }
        qDebug()<<Q_FUNC_INFO<<d->parameters["channel"];
        dtk::d_inliststring *channelParam = (dtk::d_inliststring *)d->parameters["channel"];
        channelParam->setValues(ddd->image->channels());
        channelParam->setValue(ddd->image->channels()[0]);
    }

    dtk::d_range_int *valueRangeParam = (dtk::d_range_int *)d->parameters["value_range"];
    valueRangeParam->setMin(0);

    QString channel = ddd->image->channels()[0];
    if (ddd->image->image(channel)->storageType() == QMetaType::UChar) {
        valueRangeParam->setMax(255);
        valueRangeParam->setValue({0,255});
    } else if (ddd->image->image(channel)->storageType() == QMetaType::UShort) {
        valueRangeParam->setMax(65535);
        valueRangeParam->setValue({0,65535});
     }
}

std::shared_ptr<gnomonImageSeries> gnomonVisualizationImage::image(void)
{
    return ddd->imageSeries;
}

void gnomonVisualizationImage::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    ddd->actor2D->setOpacity(alpha);
    ddd->volume->setOpacity(alpha);
}

void gnomonVisualizationImage::updateChannelColorMap(void)
{
    if(ddd->image->channels().size()>1) {
        QString channel = ((dtk::d_inliststring *)d->parameters["channel"])->value();

        if(ddd->channelColormaps.contains(channel)) {
            ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(ddd->channelColormaps[channel]);
        }
    }
}

QImage gnomonVisualizationImage::imageRendering(void)
{
    double bounds[6];
    ddd->image_data->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(ddd->volume);

    return this->offscreenImageRendering();
}

void gnomonVisualizationImage::update(void)
{
    if(!ddd->image)
        return;

    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();
    std::array<long long int, 2> value_range = ((dtk::d_range_int *)d->parameters["value_range"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();

    QString channel;
    if(ddd->image->channels().size()>1) {
        channel = ((dtk::d_inliststring *)d->parameters["channel"])->value();
        ddd->channelColormaps[channel] = colormap;
    } else {
        channel = "";
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(ddd->image->image(channel));
    converter->convert();
    ddd->image_data = static_cast<vtkImageData *>(converter->output());
    delete converter;

    if (!ddd->actor2D) {
        ddd->actor2D = gnomonActor2DImageWidget::New();
    }
    ddd->actor2D->setImage(ddd->image_data);
    ddd->actor2D->setInteractor(((gnomonViewForm *) d->view)->renderer2D()->GetRenderWindow()->GetInteractor());
    ddd->actor2D->setColorMap(colormap);
    ddd->actor2D->setValueRange(value_range);
    ddd->actor2D->setOpacity(alpha);
    ddd->actor2D->update();

    if (!ddd->volume) {
        ddd->volume = gnomonActorImageVolume::New();
        ((gnomonViewForm *) d->view)->renderer3D()->AddActor(ddd->volume);
    }
    ddd->volume->setInteractor(((gnomonViewForm *) d->view)->interactor());
    ddd->volume->setImage(ddd->image_data);
    ddd->volume->setColorMap(colormap);
    ddd->volume->setValueRange(value_range);

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (ddd->image_data->GetDimensions()[0]-1)*ddd->image_data->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (ddd->image_data->GetDimensions()[1]-1)*ddd->image_data->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (ddd->image_data->GetDimensions()[2]-1)*ddd->image_data->GetSpacing()[2];
    ((gnomonViewForm *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonVisualizationImage::render(void)
{
    this->updateOpacity();
    ((gnomonViewForm *) d->view)->render();
}

dtkCoreParameters gnomonVisualizationImage::parameters(void) const
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

void gnomonVisualizationImage::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonVisualizationImage::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "rendering";
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonVisualizationImage::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}


void gnomonVisualizationImage::onSliceChanged(int value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonVisualizationImage::on3D(void)
{
    ddd->actor2D->hide();
    this->render();
}

void gnomonVisualizationImage::on2D(void)
{
    ddd->actor2D->show();
    this->render();
}

void gnomonVisualizationImage::onXY(void)
{
    this->render();
}

void gnomonVisualizationImage::onYZ(void)
{
    this->render();
}

void gnomonVisualizationImage::onXZ(void)
{
    this->render();
}

void gnomonVisualizationImage::onTimeChanged(double value)
{
    qDebug()<<Q_FUNC_INFO<<"Time changed"<<value;
    this->render();
}

const QString gnomonVisualizationImage::name(void) {
    return "Image Visualization";
}

//
// gnomonVisualizationImage.cpp ends here
