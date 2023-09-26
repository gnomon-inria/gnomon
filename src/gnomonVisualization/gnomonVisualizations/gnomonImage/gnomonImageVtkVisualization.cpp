#include "gnomonImageVtkVisualization.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractVtkVisualization_p.h"

#include <gnomonVisualization/gnomonCoreParameterColor.h>

#include <dtkImagingCore>

#include "gnomonView/gnomonVtkView.h"
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
// gnomonImageVtkVisualizationPrivate
// /////////////////////////////////////////////////////////////////

class gnomonImageVtkVisualizationPrivate
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
// gnomonImageVtkVisualization
// /////////////////////////////////////////////////////////////////

gnomonImageVtkVisualization::gnomonImageVtkVisualization(void) : gnomonAbstractImageVtkVisualization(), ddd(new gnomonImageVtkVisualizationPrivate)
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

gnomonImageVtkVisualization::~gnomonImageVtkVisualization(void)
{
    if (ddd->volume) {
        ddd->volume->Delete();
        ddd->volume = nullptr;
    }
    if (ddd->actor2D) {
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }
    delete ddd;
}

const QString gnomonImageVtkVisualization::pluginName(void)
{
    return  "gnomonImageVtkVisualization";
}

void gnomonImageVtkVisualization::clear(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->volume) {
            view->renderer3D()->RemoveActor(ddd->volume);
        }
        if (ddd->actor2D) {
            view->renderer2D()->RemoveActor(ddd->actor2D);
        }
    }
}

void gnomonImageVtkVisualization::fill(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->volume) {
            view->renderer3D()->AddActor(ddd->volume);
        }
        if (ddd->actor2D) {
            view->renderer2D()->AddActor(ddd->actor2D);
        }
    }
}

void gnomonImageVtkVisualization::setVisible(bool visible)
{
    if (ddd->volume) {
        ddd->volume->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonImageVtkVisualization::setImage(std::shared_ptr<gnomonImageSeries> image)
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
    if (ddd->image->image(channel)->GetScalarType() == VTK_UNSIGNED_CHAR) {
        valueRangeParam->setMax(255);
        valueRangeParam->setValue({0,255});
    } else if (ddd->image->image(channel)->GetScalarType() == VTK_UNSIGNED_SHORT) {
        valueRangeParam->setMax(65535);
        valueRangeParam->setValue({0,65535});
     }
}

std::shared_ptr<gnomonImageSeries> gnomonImageVtkVisualization::image(void)
{
    return ddd->imageSeries;
}

void gnomonImageVtkVisualization::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    ddd->actor2D->setOpacity(alpha);
    ddd->volume->setOpacity(alpha);
}

void gnomonImageVtkVisualization::updateChannelColorMap(void)
{
    if(ddd->image->channels().size()>1) {
        QString channel = ((dtk::d_inliststring *)d->parameters["channel"])->value();

        if(ddd->channelColormaps.contains(channel)) {
            ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->setValue(ddd->channelColormaps[channel]);
        }
    }
}

QImage gnomonImageVtkVisualization::imageRendering(void)
{
    double bounds[6];
    ddd->image_data->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(ddd->volume);

    return this->offscreenImageRendering();
}

void gnomonImageVtkVisualization::update(void)
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

    ddd->image_data = ddd->image->image(channel);

    if (!ddd->actor2D) {
        ddd->actor2D = gnomonActor2DImageWidget::New();
    }
    ddd->actor2D->setImage(ddd->image_data);
    ddd->actor2D->setInteractor(((gnomonVtkView *) d->view)->renderer2D()->GetRenderWindow()->GetInteractor());
    ddd->actor2D->setColorMap(colormap);
    ddd->actor2D->setValueRange(value_range);
    ddd->actor2D->setOpacity(alpha);
    ddd->actor2D->update();

    if (!ddd->volume) {
        ddd->volume = gnomonActorImageVolume::New();
        ((gnomonVtkView *) d->view)->renderer3D()->AddActor(ddd->volume);
    }
    ddd->volume->setInteractor(((gnomonVtkView *) d->view)->interactor());
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
    ((gnomonVtkView *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonImageVtkVisualization::render(void)
{
    this->updateOpacity();
    ((gnomonVtkView *) d->view)->render();
}

dtkCoreParameters gnomonImageVtkVisualization::parameters(void) const
{
    return d->parameters;
}

void gnomonImageVtkVisualization::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonImageVtkVisualization::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonImageVtkVisualization::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "rendering";
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonImageVtkVisualization::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}


void gnomonImageVtkVisualization::onSliceChanged(double value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonImageVtkVisualization::on3D(void)
{
    ddd->actor2D->hide();
    this->render();
}

void gnomonImageVtkVisualization::on2D(void)
{
    ddd->actor2D->show();
    this->render();
}

void gnomonImageVtkVisualization::onXY(void)
{
    this->render();
}

void gnomonImageVtkVisualization::onYZ(void)
{
    this->render();
}

void gnomonImageVtkVisualization::onXZ(void)
{
    this->render();
}

void gnomonImageVtkVisualization::onTimeChanged(double value)
{
    qDebug()<<Q_FUNC_INFO<<"Time changed"<<value;
    this->render();
}

const QString gnomonImageVtkVisualization::name(void) {
    return "Image Visualization";
}

//
// gnomonImageVtkVisualization.cpp ends here
