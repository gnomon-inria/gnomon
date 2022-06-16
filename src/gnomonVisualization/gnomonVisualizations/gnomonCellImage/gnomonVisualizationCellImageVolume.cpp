#include "gnomonVisualizationCellImageVolume.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <dtkImagingCore>

#include <gnomonVisualization/gnomonCoreParameterColor>

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonImageData/gnomonActorImageVolume.h"
#include "gnomonActor/gnomonImageData/gnomonActor2DImage.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationCellImageVolumePrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> cellImageSeries = nullptr;
    std::shared_ptr<gnomonCellImage> cellImage;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;

    gnomonActorImageVolume *actor = nullptr;
    gnomonActor2DImage *actor2D = nullptr;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageVolume
// /////////////////////////////////////////////////////////////////

gnomonVisualizationCellImageVolume::gnomonVisualizationCellImageVolume(void) : gnomonAbstractVisualizationCellImage(), dd(new gnomonVisualizationCellImageVolumePrivate)
{
    d->parameters["value_range"] = new dtk::d_range_int("value_range", {0, 255}, 0, 255, "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("colormap", "glasbey", "Colormap to apply to the cellImage");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the cellImage rendering");
}

gnomonVisualizationCellImageVolume::~gnomonVisualizationCellImageVolume(void)
{
    this->clear();
    delete dd;
}

const QString gnomonVisualizationCellImageVolume::pluginName(void)
{
    return  "gnomonVisualizationCellImageVolume";
}

void gnomonVisualizationCellImageVolume::clear(void)
{
//    gnomonAbstractVisualization::clear();

    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (dd->actor2D) {
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }
}

void gnomonVisualizationCellImageVolume::setVisible(bool visible)
{
    if (dd->actor) {
        dd->actor->SetVisibility(visible);
    }

    if (dd->actor2D) {
        dd->actor2D->SetVisibility(visible);
    }
}

void gnomonVisualizationCellImageVolume::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    dd->cellImageSeries = cellImage;
    dd->cellImage = cellImage->current();
    this->setParameter("alpha",1.0);

    if (dd->cellImage)
        this->updateValueRange();
}

std::shared_ptr<gnomonCellImageSeries> gnomonVisualizationCellImageVolume::cellImage(void)
{
    return dd->cellImageSeries;
}

void gnomonVisualizationCellImageVolume::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    if(dd->actor) {
        dd->actor->setOpacity(alpha);
    }

    if(dd->actor2D) {
        dd->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationCellImageVolume::updateValueRange(void)
{
     QList<long> cellIds = dd->cellImage->cellIds();
     auto mm = std::minmax_element(cellIds.begin(),cellIds.end());

     ((dtk::d_range_int *)d->parameters["value_range"])->setBounds({*(mm.first),*(mm.second)});
}

QImage gnomonVisualizationCellImageVolume::imageRendering(void)
{
    double bounds[6];
    dd->image->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    d->view->renderer3D()->RemoveActor(dd->actor);
    this->offscreenRenderer()->AddActor(dd->actor);

    QImage image = this->offscreenImageRendering();

    this->offscreenRenderer()->RemoveActor(dd->actor);
    d->view->renderer3D()->AddActor(dd->actor);
    dd->actor->setInteractor(d->view->interactor());

    return image;
}

void gnomonVisualizationCellImageVolume::update(void)
{
     QString colormap_name = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->name();
     QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
     std::array<long long int, 2> value_range = ((dtk::d_range_int *)d->parameters["value_range"])->value();

    if(!dd->cellImage)
        return;

    if (dd->image) {
        dd->image->Delete();
        dd->image = nullptr;
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if(!converter) {
        dtkWarn() << Q_FUNC_INFO << "cannot instanciate a dtkVtkImageConverter, please check that dtk-plugins-imaging is installed!";
        return;
    }
    converter->setInput(dd->cellImage->image());
    converter->convert();
    dd->image = static_cast<vtkImageData *>(converter->output());
    delete converter;

    if (colormap_name=="glasbey") {
        int shape[3];
        dd->image->GetDimensions(shape);
        for (int z=0; z<shape[2]; ++z)
        {
            for (int y=0; y<shape[1]; ++y)
            {
                for (int x=0; x<shape[0]; ++x)
                {
                    int label = int(dd->image->GetScalarComponentAsFloat(x,y,z,0))%256;
                    dd->image->SetScalarComponentFromFloat(x,y,z,0,label);
                }
            }
        }

        value_range[0] = 0;
        value_range[1] = 255;
    }

   if (dd->actor) {
       d->view->renderer3D()->RemoveActor(dd->actor);
       dd->actor->Delete();
       dd->actor = nullptr;
   }

    if (!dd->actor)
        dd->actor = gnomonActorImageVolume::New();
        d->view->renderer3D()->AddActor(dd->actor);
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setImage(dd->image);
    dd->actor->setColorMap(colormap);
    dd->actor->setValueRange(value_range);
    dd->actor->setFlatRendering(true);

   if (dd->actor2D) {
       d->view->renderer2D()->RemoveActor(dd->actor2D);
       dd->actor2D->Delete();
       dd->actor2D = nullptr;
   }

    if (!dd->actor2D)
    {
        dd->actor2D = gnomonActor2DImage::New();
        d->view->renderer2D()->AddActor(dd->actor2D);
    }
    dd->actor2D->setImage(dd->image);
    dd->actor2D->setInteractor(d->view->renderer2D()->GetRenderWindow()->GetInteractor());
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);
    dd->actor2D->setFlatRendering(true);
    dd->actor2D->update();

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

void gnomonVisualizationCellImageVolume::render(void)
{
    this->updateOpacity();
    d->view->render();
}


dtkCoreParameters gnomonVisualizationCellImageVolume::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationCellImageVolume::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationCellImageVolume::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonVisualizationCellImageVolume::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "rendering";
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonVisualizationCellImageVolume::onSliceOrientationChanged(int value)
{
    dd->actor2D->setSliceOrientation(value);
}

void gnomonVisualizationCellImageVolume::onSliceChanged(int value)
{
    dd->actor2D->setSlice(value);
    this->render();
}

void gnomonVisualizationCellImageVolume::on3D(void)
{
    dd->actor2D->hide();
    this->render();
}

void gnomonVisualizationCellImageVolume::on2D(void)
{
    dd->actor2D->show();
    this->render();
}

void gnomonVisualizationCellImageVolume::onXY(void)
{
    this->render();
}

void gnomonVisualizationCellImageVolume::onYZ(void)
{
    this->render();
}

void gnomonVisualizationCellImageVolume::onXZ(void)
{
    this->render();
}

void gnomonVisualizationCellImageVolume::onTimeChanged(double value)
{
    if (dd->cellImageSeries->times().contains(value)) {
        dd->cellImage = dd->cellImageSeries->at(value);
        this->update();
    }
    this->render();
}

//
// gnomonVisualizationCellImageVolume.cpp ends here
