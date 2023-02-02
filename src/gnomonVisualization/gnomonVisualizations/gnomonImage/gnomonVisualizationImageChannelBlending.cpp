#include "gnomonVisualizationImageChannelBlending.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <dtkImagingCore>

#include "gnomonView/gnomonViewForm.h"
#include "gnomonActor/gnomonImage/gnomonImageDataChannelBlending.h"
#include "gnomonActor/gnomonImageData/gnomonActorImageRGBAVolume.h"
#include "gnomonActor/gnomonImageData/gnomonActor2DImageRGBA.h"

#include <gnomonVisualization/gnomonCoreParameterLookupTable>
#include <gnomonVisualization/gnomonLookupTable.h>

#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImageChannelBlendingPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationImageChannelBlendingPrivate
{
public:
    gnomonVisualizationImageChannelBlending *qq = nullptr;

public:
    std::shared_ptr<gnomonImageSeries> imageSeries;
    std::shared_ptr<gnomonImage> image;
    QHash<QString, std::shared_ptr<dtkImage>> dtk_img_by_channel;

public:
    int orientation = 2;

public:
    QMap<QString, vtkSmartPointer<vtkImageData>> vtk_img_by_channel;
    vtkImageData *vtk_img = nullptr;

    gnomonImageDataChannelBlending *blending = nullptr;

    gnomonActor2DImageRGBA *actor2D = nullptr;
    gnomonActorImageRGBAVolume *volume = nullptr;

public:
    QMap<QString, gnomonLookupTable> channelLookupTables;

public:
    QMap<int, QString> defaultColormaps;

public:
    void reset(void);
};

void gnomonVisualizationImageChannelBlendingPrivate::reset(void)
{
    this->imageSeries.reset();
    this->image.reset();

    this->channelLookupTables.clear();
    this->dtk_img_by_channel.clear();
    this->vtk_img_by_channel.clear();

    /*
    auto it = this->qq->d->parameters.begin();
    auto it_end = this->qq->d->parameters.end();
    while (it != it_end) {
        if (auto p = dynamic_cast<gnomonCoreParameterLookupTable*>(it.value())) {
            p->disconnect();
            delete p;
            it = this->qq->d->parameters.erase(it);
        } else {
            ++it;
        }
    }
    */

    qq->clear();
}

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationImageChannelBlending
// /////////////////////////////////////////////////////////////////

gnomonVisualizationImageChannelBlending::gnomonVisualizationImageChannelBlending(void) : gnomonAbstractVisualizationImage(), dd(new gnomonVisualizationImageChannelBlendingPrivate)
{
    dd->qq = this;

    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the image rendering");

    dd->defaultColormaps[0] = "gray";
    dd->defaultColormaps[1] = "0CMY_cyan";
    dd->defaultColormaps[2] = "0CMY_magenta";
    dd->defaultColormaps[3] = "0CMY_yellow";
    dd->defaultColormaps[4] = "0RGB_green";
    dd->defaultColormaps[5] = "0RGB_red";
    dd->defaultColormaps[6] = "0RGB_blue";
}

gnomonVisualizationImageChannelBlending::~gnomonVisualizationImageChannelBlending(void)
{
    dd->reset();
    dd->qq = nullptr;
    delete dd;
    dd = nullptr;
}

const QString gnomonVisualizationImageChannelBlending::pluginName(void)
{
    return "gnomonVisualizationImageChannelBlending";
}

void gnomonVisualizationImageChannelBlending::clear(void)
{
    if (dd->volume) {
        d->view->renderer3D()->RemoveActor(dd->volume);
        dd->volume->Delete();
        dd->volume = nullptr;
    }

    if (dd->actor2D) {
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }

    if (dd->blending) {
        dd->blending->Delete();
        dd->blending = nullptr;
    }
}

void gnomonVisualizationImageChannelBlending::setVisible(bool visible)
{
    if (dd->volume) {
        dd->volume->SetVisibility(visible);
    }

    if (dd->actor2D) {
        dd->actor2D->SetVisibility(visible);
    }
}

void gnomonVisualizationImageChannelBlending::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    dd->reset();

    dd->imageSeries = image;
    dd->image = image->current();

    this->setParameter("alpha", 1.0);

    // Fill all the maps
    QList<double> valueRange = {0, 1};
    QList<double> channelRange(2);
    int channel_id = 0;
    auto img_channels = dd->image->channels();
    for (auto channel : img_channels) {
        auto dtk_img = dd->image->image(channel);

        if (dtk_img->storageType() == QMetaType::UChar) {
            valueRange[1] = 255;
        } else if (dtk_img->storageType() == QMetaType::UShort) {
            valueRange[1] = 65535;
        }
        channelRange[0] = dd->image->minValue(channel);
        channelRange[1] = dd->image->maxValue(channel);

        dd->channelLookupTables[channel] = gnomonLookupTable(dd->defaultColormaps[channel_id], channelRange, valueRange, true);
        auto param = new gnomonCoreParameterLookupTable(channel+"\nLUT", dd->channelLookupTables[channel], "Lookuptable to apply to the " + channel + " image channel");

        if (channel.isEmpty()) {
            param->setLabel("LUT");
            param->setDocumentation("Lookuptable to apply to the image");
            d->parameters["lookuptable"] = param;

        } else {
            d->parameters[channel+"\nlookuptable"] = param;
        }
        ++channel_id;
    }

    this->updateChannelImages();

    emit parametersChanged();
}

void gnomonVisualizationImageChannelBlending::updateChannelImages(void)
{
    if (dd->image) {
        dd->dtk_img_by_channel.clear();
        dd->vtk_img_by_channel.clear();

        auto img_channels = dd->image->channels();
        for (auto channel : img_channels) {
            auto dtk_img = dd->image->image(channel);
            dd->dtk_img_by_channel[channel].reset(dtk_img);

            // Fill vtk maps
            dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
            converter->setInput(dtk_img);
            converter->convert();
            dd->vtk_img_by_channel[channel] = static_cast<vtkImageData *>(converter->output());
            delete converter;
        }
    }
}

std::shared_ptr<gnomonImageSeries> gnomonVisualizationImageChannelBlending::image(void)
{
    return dd->imageSeries;
}

void gnomonVisualizationImageChannelBlending::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    dd->actor2D->setOpacity(alpha);
    dd->volume->setOpacity(alpha);
}

QImage gnomonVisualizationImageChannelBlending::imageRendering(void)
{
    double bounds[6];
    dd->vtk_img->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(dd->volume);

    return this->offscreenImageRendering();
}

void gnomonVisualizationImageChannelBlending::update(void)
{
    if(!dd->image)
        return;

    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

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
    dd->blending->setImageChannels(dd->vtk_img_by_channel);
    dd->blending->setChannelLookupTables(dd->channelLookupTables);

    dd->blending->update();
    dd->vtk_img = dd->blending;

    if (!dd->actor2D) {
        dd->actor2D = gnomonActor2DImageRGBA::New();
        d->view->renderer2D()->AddActor(dd->actor2D);
    }
    dd->actor2D->setImage(dd->vtk_img);
    dd->actor2D->setInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
    dd->actor2D->setOpacity(alpha);
    dd->actor2D->update();

    if (!dd->volume) {
        dd->volume = gnomonActorImageRGBAVolume::New();
        d->view->renderer3D()->AddActor(dd->volume);
    }
    dd->volume->setInteractor(d->view->interactor());
    dd->volume->setImage(dd->vtk_img);

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (dd->vtk_img->GetDimensions()[0]-1)*dd->vtk_img->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (dd->vtk_img->GetDimensions()[1]-1)*dd->vtk_img->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (dd->vtk_img->GetDimensions()[2]-1)*dd->vtk_img->GetSpacing()[2];
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationImageChannelBlending::render(void)
{
    this->updateOpacity();
    d->view->render();
}


dtkCoreParameters gnomonVisualizationImageChannelBlending::parameters(void) const
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

void gnomonVisualizationImageChannelBlending::setParameters(const dtkCoreParameters& parameters)
{
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonVisualizationImageChannelBlending::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonVisualizationImageChannelBlending::onSliceOrientationChanged(int value)
{
    dd->actor2D->setSliceOrientation(value);
}

void gnomonVisualizationImageChannelBlending::onSliceChanged(int value)
{
    dd->actor2D->setSlice(value);
    this->render();
}

void gnomonVisualizationImageChannelBlending::on3D(void)
{
    dd->actor2D->hide();
    this->render();
}

void gnomonVisualizationImageChannelBlending::on2D(void)
{
    dd->actor2D->show();
    this->render();
}

void gnomonVisualizationImageChannelBlending::onXY(void)
{
    this->render();
}

void gnomonVisualizationImageChannelBlending::onYZ(void)
{
    this->render();
}

void gnomonVisualizationImageChannelBlending::onXZ(void)
{
    this->render();
}

void gnomonVisualizationImageChannelBlending::onTimeChanged(double value)
{
    if (dd->imageSeries->times().contains(value)) {
        dd->image = dd->imageSeries->at(value);
        this->updateChannelImages();
        this->update();
    }
    this->render();
}

const QString gnomonVisualizationImageChannelBlending::name(void) {
    return "Channel Blending";
}

//
// gnomonVisualizationImageChannelBlending.cpp ends here
