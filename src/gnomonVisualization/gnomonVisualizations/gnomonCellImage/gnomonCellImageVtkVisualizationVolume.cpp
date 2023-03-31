#include "gnomonCellImageVtkVisualizationVolume.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractVtkVisualization_p.h"

#include <dtkImagingCore>

#include <gnomonVisualization/gnomonCoreParameterColor>

#include "gnomonView/gnomonVtkView.h"

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
// gnomonCellImageVtkVisualizationVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonCellImageVtkVisualizationVolumePrivate
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
// gnomonCellImageVtkVisualizationVolume
// /////////////////////////////////////////////////////////////////

gnomonCellImageVtkVisualizationVolume::gnomonCellImageVtkVisualizationVolume(void) : gnomonAbstractCellImageVtkVisualization(), ddd(new gnomonCellImageVtkVisualizationVolumePrivate)
{
    d->parameters["value_range"] = new dtk::d_range_int("value_range", {0, 255}, 0, 255, "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("colormap", "glasbey", "Colormap to apply to the cellImage");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the cellImage rendering");
}

gnomonCellImageVtkVisualizationVolume::~gnomonCellImageVtkVisualizationVolume(void)
{
    this->clear();
    delete ddd;
}

const QString gnomonCellImageVtkVisualizationVolume::pluginName(void)
{
    return  "gnomonCellImageVtkVisualizationVolume";
}

void gnomonCellImageVtkVisualizationVolume::clear(void)
{
//    gnomonAbstractVisualization::clear();

    if (ddd->actor) {
        ((gnomonVtkView *) d->view)->renderer3D()->RemoveActor(ddd->actor);
        ddd->actor->Delete();
        ddd->actor = nullptr;
    }

    if (ddd->actor2D) {
        ((gnomonVtkView *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }
}

void gnomonCellImageVtkVisualizationVolume::setVisible(bool visible)
{
    if (ddd->actor) {
        ddd->actor->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonCellImageVtkVisualizationVolume::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    ddd->cellImageSeries = cellImage;
    ddd->cellImage = cellImage->current();
    this->setParameter("alpha",1.0);

    if (ddd->cellImage)
        this->updateValueRange();
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageVtkVisualizationVolume::cellImage(void)
{
    return ddd->cellImageSeries;
}

void gnomonCellImageVtkVisualizationVolume::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    if(ddd->actor && ddd->actor->isVisible()) {
        ddd->actor->setOpacity(alpha);
    }

    if(ddd->actor2D) {
        ddd->actor2D->setOpacity(alpha);
    }
}

void gnomonCellImageVtkVisualizationVolume::updateValueRange(void)
{
     QList<long> cellIds = ddd->cellImage->cellIds();
     auto mm = std::minmax_element(cellIds.begin(),cellIds.end());

     ((dtk::d_range_int *)d->parameters["value_range"])->setBounds({*(mm.first),*(mm.second)});
}

QImage gnomonCellImageVtkVisualizationVolume::imageRendering(void)
{
    double bounds[6];
    ddd->image->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    ddd->actor->show();
    ((gnomonVtkView *) d->view)->renderer3D()->RemoveActor(ddd->actor);
    this->offscreenRenderer()->AddActor(ddd->actor);

    QImage image = this->offscreenImageRendering();

    this->offscreenRenderer()->RemoveActor(ddd->actor);
    ((gnomonVtkView *) d->view)->renderer3D()->AddActor(ddd->actor);
    ddd->actor->setInteractor(((gnomonVtkView *) d->view)->interactor());

    return image;
}

void gnomonCellImageVtkVisualizationVolume::update(void)
{
     QString colormap_name = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->name();
     QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
     std::array<long long int, 2> value_range = ((dtk::d_range_int *)d->parameters["value_range"])->value();

    if(!ddd->cellImage)
        return;

    if (ddd->image) {
        ddd->image->Delete();
        ddd->image = nullptr;
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if(!converter) {
        dtkWarn() << Q_FUNC_INFO << "cannot instanciate a dtkVtkImageConverter, please check that dtk-plugins-imaging is installed!";
        return;
    }
    converter->setInput(ddd->cellImage->image());
    converter->convert();
    ddd->image = static_cast<vtkImageData *>(converter->output());
    delete converter;

    if (colormap_name=="glasbey") {
        int shape[3];
        ddd->image->GetDimensions(shape);
        for (int z=0; z<shape[2]; ++z)
        {
            for (int y=0; y<shape[1]; ++y)
            {
                for (int x=0; x<shape[0]; ++x)
                {
                    int label = int(ddd->image->GetScalarComponentAsFloat(x,y,z,0))%256;
                    ddd->image->SetScalarComponentFromFloat(x,y,z,0,label);
                }
            }
        }

        value_range[0] = 0;
        value_range[1] = 255;
    }

   if (ddd->actor) {
       ((gnomonVtkView *) d->view)->renderer3D()->RemoveActor(ddd->actor);
       ddd->actor->Delete();
       ddd->actor = nullptr;
   }

    if (!ddd->actor)
        ddd->actor = gnomonActorImageVolume::New();
        ((gnomonVtkView *) d->view)->renderer3D()->AddActor(ddd->actor);
    ddd->actor->setInteractor(((gnomonVtkView *) d->view)->interactor());
    ddd->actor->setImage(ddd->image);
    ddd->actor->setColorMap(colormap);
    ddd->actor->setValueRange(value_range);
    ddd->actor->setFlatRendering(true);

   if (ddd->actor2D) {
       ((gnomonVtkView *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
       ddd->actor2D->Delete();
       ddd->actor2D = nullptr;
   }

    if (!ddd->actor2D)
    {
        ddd->actor2D = gnomonActor2DImage::New();
        ((gnomonVtkView *) d->view)->renderer2D()->AddActor(ddd->actor2D);
    }
    ddd->actor2D->setImage(ddd->image);
    ddd->actor2D->setInteractor(((gnomonVtkView *) d->view)->interactor());
    //ddd->actor2D->setInteractor(((gnomonVtkView *) d->view)->renderer2D()->GetRenderWindow()->GetInteractor());
    ddd->actor2D->setColorMap(colormap);
    ddd->actor2D->setValueRange(value_range);
    ddd->actor2D->setFlatRendering(true);
    ddd->actor2D->update();

    double bounds[6];
    bounds[0] = 0;
    bounds[1] = (ddd->image->GetDimensions()[0]-1)*ddd->image->GetSpacing()[0];
    bounds[2] = 0;
    bounds[3] = (ddd->image->GetDimensions()[1]-1)*ddd->image->GetSpacing()[1];
    bounds[4] = 0;
    bounds[5] = (ddd->image->GetDimensions()[2]-1)*ddd->image->GetSpacing()[2];
    ((gnomonVtkView *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonCellImageVtkVisualizationVolume::render(void)
{
    this->updateOpacity();
    ((gnomonVtkView *) d->view)->render();
}


dtkCoreParameters gnomonCellImageVtkVisualizationVolume::parameters(void) const
{
    return d->parameters;
}

void gnomonCellImageVtkVisualizationVolume::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonCellImageVtkVisualizationVolume::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonCellImageVtkVisualizationVolume::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "general";
    groups["alpha"] = "general";
    return groups;
}

void gnomonCellImageVtkVisualizationVolume::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}

void gnomonCellImageVtkVisualizationVolume::onSliceChanged(int value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonCellImageVtkVisualizationVolume::on3D(void)
{
    ddd->actor->show();
    ddd->actor2D->hide();
    //this->render();
}

void gnomonCellImageVtkVisualizationVolume::on2D(void)
{
    ddd->actor->hide();
    ddd->actor2D->show();
    //this->render();
}

void gnomonCellImageVtkVisualizationVolume::onXY(void)
{
    this->render();
}

void gnomonCellImageVtkVisualizationVolume::onYZ(void)
{
    this->render();
}

void gnomonCellImageVtkVisualizationVolume::onXZ(void)
{
    this->render();
}

void gnomonCellImageVtkVisualizationVolume::onTimeChanged(double value)
{
    if (ddd->cellImageSeries->times().contains(value)) {
        ddd->cellImage = ddd->cellImageSeries->at(value);
        this->update();
    }
    this->render();
}

const QString gnomonCellImageVtkVisualizationVolume::name(void) {
    return "Cell Image Volume";
}

//
// gnomonCellImageVtkVisualizationVolume.cpp ends here
