#include "gnomonImageVtkVisualizationChannelBlending.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractVtkVisualization_p.h"

#include "gnomonView/gnomonVtkView.h"
#include "gnomonActor/gnomonImage/gnomonImageDataChannelBlending.h"
#include "gnomonActor/gnomonImageData/gnomonActorImageRGBAVolume.h"
#include "gnomonActor/gnomonImageData/gnomonActor2DImageRGBA.h"

#include <dtkLog.h>
#include <gnomonVisualization/gnomonCoreParameterLookupTable>
#include <gnomonVisualization/gnomonLookupTable.h>

#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonImageVtkVisualizationChannelBlendingPrivate
// /////////////////////////////////////////////////////////////////

class gnomonImageVtkVisualizationChannelBlendingPrivate
{
public:
    gnomonImageVtkVisualizationChannelBlending *qq = nullptr;

public:
    std::shared_ptr<gnomonImageSeries> imageSeries;
    std::shared_ptr<gnomonImage> image;

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

void gnomonImageVtkVisualizationChannelBlendingPrivate::reset(void)
{
    this->imageSeries.reset();
    this->image.reset();

    this->channelLookupTables.clear();
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
// gnomonImageVtkVisualizationChannelBlending
// /////////////////////////////////////////////////////////////////

gnomonImageVtkVisualizationChannelBlending::gnomonImageVtkVisualizationChannelBlending(void) : gnomonAbstractImageVtkVisualization(), ddd(new gnomonImageVtkVisualizationChannelBlendingPrivate)
{
    ddd->qq = this;

    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the image rendering");

    ddd->defaultColormaps[0] = "gray";
    ddd->defaultColormaps[1] = "0CMY_cyan";
    ddd->defaultColormaps[2] = "0CMY_magenta";
    ddd->defaultColormaps[3] = "0CMY_yellow";
    ddd->defaultColormaps[4] = "0RGB_green";
    ddd->defaultColormaps[5] = "0RGB_red";
    ddd->defaultColormaps[6] = "0RGB_blue";
}

gnomonImageVtkVisualizationChannelBlending::~gnomonImageVtkVisualizationChannelBlending(void)
{
    ddd->reset();
    ddd->qq = nullptr;

    if (ddd->volume) {
        ddd->volume->Delete();
        ddd->volume = nullptr;
    }
    if (ddd->actor2D) {
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }
    if (ddd->blending) {
        ddd->blending->Delete();
        ddd->blending = nullptr;
    }
    delete ddd;
    ddd = nullptr;
}

const QString gnomonImageVtkVisualizationChannelBlending::pluginName(void)
{
    return "gnomonImageVtkVisualizationChannelBlending";
}
void gnomonImageVtkVisualizationChannelBlending::clear(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->actor2D) {
            view->renderer2D()->RemoveActor(ddd->actor2D);
        }
        if (ddd->volume) {
            view->renderer3D()->RemoveActor(ddd->volume);
        }
    }
}

void gnomonImageVtkVisualizationChannelBlending::fill(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->actor2D) {
            view->renderer2D()->AddActor(ddd->actor2D);
        }
        if (ddd->volume) {
            view->renderer3D()->AddActor(ddd->volume);
        }
    }
}

void gnomonImageVtkVisualizationChannelBlending::setVisible(bool visible)
{
    if (ddd->volume) {
        ddd->volume->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonImageVtkVisualizationChannelBlending::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    ddd->reset();

    ddd->imageSeries = image;
    ddd->image = image->current();

    this->setParameter("alpha", 1.0);

    // Fill all the maps
    QList<double> valueRange = {0, 1};
    QList<double> channelRange(2);
    int channel_id = 0;
    auto img_channels = ddd->image->channels();
    for (auto channel : img_channels) {
        auto vtk_img = ddd->image->image(channel);

        if (vtk_img->GetScalarType() == VTK_UNSIGNED_CHAR) {
            valueRange[1] = 255;
        } else if (vtk_img->GetScalarType() == VTK_UNSIGNED_SHORT) {
            valueRange[1] = 65535;
        }
        channelRange[0] = ddd->image->minValue(channel);
        channelRange[1] = ddd->image->maxValue(channel);

        ddd->channelLookupTables[channel] = gnomonLookupTable(ddd->defaultColormaps[channel_id], channelRange, valueRange, true);
        auto param = new gnomonCoreParameterLookupTable(channel+"\nLUT", ddd->channelLookupTables[channel], "Lookuptable to apply to the " + channel + " image channel");

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

void gnomonImageVtkVisualizationChannelBlending::updateChannelImages(void)
{
    if (ddd->image) {
        ddd->vtk_img_by_channel.clear();

        auto img_channels = ddd->image->channels();
        for (auto channel : img_channels) {
            vtkSmartPointer<vtkImageData> vtk_img(ddd->image->image(channel));
            ddd->vtk_img_by_channel[channel] = vtk_img;
        }
    }
}

std::shared_ptr<gnomonImageSeries> gnomonImageVtkVisualizationChannelBlending::image(void)
{
    return ddd->imageSeries;
}

void gnomonImageVtkVisualizationChannelBlending::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    ddd->actor2D->setOpacity(alpha);
    ddd->volume->setOpacity(alpha);
}

QImage gnomonImageVtkVisualizationChannelBlending::imageRendering(void)
{
    double bounds[6];
    ddd->vtk_img->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(ddd->volume);

    return this->offscreenImageRendering();
}

void gnomonImageVtkVisualizationChannelBlending::update(void)
{
    if(!ddd->image || !((gnomonVtkView *) d->view)->renderer2D()->GetRenderWindow())
        return;

    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    ddd->channelLookupTables.clear();
    if(ddd->image->channels().size()==1) {
        ddd->channelLookupTables[""] = ((gnomonCoreParameterLookupTable *)d->parameters["lookuptable"])->value();
    } else {
        for (const auto& channelName : ddd->image->channels()) {
            ddd->channelLookupTables[channelName] = ((gnomonCoreParameterLookupTable *)d->parameters[channelName+"\nlookuptable"])->value();
        }
    }

    if (!ddd->blending) {
        ddd->blending = gnomonImageDataChannelBlending::New();
    }
    ddd->blending->setImageChannels(ddd->vtk_img_by_channel);
    ddd->blending->setChannelLookupTables(ddd->channelLookupTables);

    ddd->blending->update();
    ddd->vtk_img = ddd->blending;

    if (!ddd->actor2D) {
        ddd->actor2D = gnomonActor2DImageRGBA::New();
        ((gnomonVtkView *) d->view)->renderer2D()->AddActor(ddd->actor2D);
    }
    ddd->actor2D->setImage(ddd->vtk_img);
    ddd->actor2D->setInteractor(((gnomonVtkView *) d->view)->renderer2D()->GetRenderWindow()->GetInteractor());
    ddd->actor2D->setOpacity(alpha);
    ddd->actor2D->update();

    if (!ddd->volume) {
        ddd->volume = gnomonActorImageRGBAVolume::New();
        ((gnomonVtkView *) d->view)->renderer3D()->AddActor(ddd->volume);
    }
    ddd->volume->setInteractor(((gnomonVtkView *) d->view)->interactor());
    ddd->volume->setImage(ddd->vtk_img);

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (ddd->vtk_img->GetDimensions()[0]-1)*ddd->vtk_img->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (ddd->vtk_img->GetDimensions()[1]-1)*ddd->vtk_img->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (ddd->vtk_img->GetDimensions()[2]-1)*ddd->vtk_img->GetSpacing()[2];
    ((gnomonVtkView *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::render(void)
{
    this->updateOpacity();
    ((gnomonVtkView *) d->view)->render();
}


dtkCoreParameters gnomonImageVtkVisualizationChannelBlending::parameters(void) const
{
    return d->parameters;
}

void gnomonImageVtkVisualizationChannelBlending::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonImageVtkVisualizationChannelBlending::setParameters(const dtkCoreParameters& parameters)
{
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonImageVtkVisualizationChannelBlending::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonImageVtkVisualizationChannelBlending::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}

void gnomonImageVtkVisualizationChannelBlending::onSliceChanged(double value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::on3D(void)
{
    ddd->actor2D->hide();
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::on2D(void)
{
    ddd->actor2D->show();
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::onXY(void)
{
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::onYZ(void)
{
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::onXZ(void)
{
    this->render();
}

void gnomonImageVtkVisualizationChannelBlending::onTimeChanged(double value)
{
    if(this->image()) {
        if (ddd->imageSeries->times().contains(value)) {
            ddd->image = ddd->imageSeries->at(value);
            this->updateChannelImages();
            this->update();
        }
        this->render();
    }
}

const QString gnomonImageVtkVisualizationChannelBlending::name(void) {
    return "Channel Blending";
}

QString gnomonImageVtkVisualizationChannelBlending::documentation(void) {
    return "Visualize an Image by blending its channels.";
}

//
// gnomonImageVtkVisualizationChannelBlending.cpp ends here
