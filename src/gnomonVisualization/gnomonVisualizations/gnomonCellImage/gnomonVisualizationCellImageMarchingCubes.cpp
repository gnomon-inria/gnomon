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

#include "gnomonVisualizationCellImageMarchingCubes.h"

#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonVisualization>
// #include <gnomonWidgets>

#include <dtkImagingCore>

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonCellImage/gnomonPolyDataCellImage.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include "gnomonInteractorStyle/gnomonCellImage/gnomonInteractorStyleCellImageMarchingCubes.h"

#include <vtkCellData.h>
#include <vtkCellLocator.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointData.h>
#include <vtkProp3DCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageMarchingCubesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationCellImageMarchingCubesPrivate
{
public:
    gnomonCellImageSeries *cellImageSeries;
    gnomonCellImage *cellImage;

public:
    gnomonPolyDataCellImage *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

public:
    gnomonInteractorStyleCellImageMarchingCubes *interactor_style = nullptr;

    gnomonVisualizationCellImageMarchingCubes *q;

public:
    bool is2D = false;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
};


void gnomonVisualizationCellImageMarchingCubesPrivate::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)q->parameters()["alpha"])->value();

    if(this->actor) {
        this->actor->setOpacity(alpha);
    }

    if(this->actor2D) {
        this->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationCellImageMarchingCubesPrivate::updateValueRange(void)
{
     QString property_name = ((dtk::d_inliststring *)q->parameters()["property_name"])->value();

     QMap<long, QVariant> cellProperty;
     if(this->cellImage->cellPropertyNames().contains(property_name)) {
         cellProperty = this->cellImage->cellProperty(property_name);
     } else {
         for (const auto& cellId : this->cellImage->cellIds()) {
             cellProperty[cellId] = QVariant((double)cellId);
         }
     }

     QList<double> cellScalarPropertyValues;
     for (const auto& cellId : this->cellImage->cellIds()) {
         cellScalarPropertyValues.append(cellProperty[cellId].value<double>());
     }
     auto mm = std::minmax_element(cellScalarPropertyValues.begin(),cellScalarPropertyValues.end());

     ((dtk::d_range_real *)q->parameters()["value_range"])->setMin(*(mm.first));
     ((dtk::d_range_real *)q->parameters()["value_range"])->setMax(*(mm.second));
     ((dtk::d_range_real *)q->parameters()["value_range"])->setValue({*(mm.first),*(mm.second)});
}

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageMarchingCubes
// /////////////////////////////////////////////////////////////////

gnomonVisualizationCellImageMarchingCubes::gnomonVisualizationCellImageMarchingCubes(void) : gnomonAbstractVisualizationCellImage(), dd(new gnomonVisualizationCellImageMarchingCubesPrivate)
{
    dd->q = this;
    dd->cellImageSeries = Q_NULLPTR;
    dd->cellImage = Q_NULLPTR;

    d->parameters["property_name"] = new dtk::d_inliststring("", {""}, "CellImage property to be displayed");
    d->parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("glasbey", "Colormap to apply to the cellImage");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the cellImage rendering");
    d->parameters["resolution"] = new dtk::d_real("resolution", 1.5, 0.1, 5., 1, "Resampling voxelsize for computing the Marching Cubes");

    d->parameters["x_range"] = new dtk::d_range_real("x_range", {0., 100.}, 0., 100., "Range of x positions of cells to display");
    d->parameters["y_range"] = new dtk::d_range_real("y_range", {0., 100.}, 0., 100., "Range of y positions of cells to display");
    d->parameters["z_range"] = new dtk::d_range_real("z_range", {0., 100.}, 0., 100., "Range of z positions of cells to display");

    dd->interactor_style = new gnomonInteractorStyleCellImageMarchingCubes();
    dd->interactor_style->setVisualization(this);
}

gnomonVisualizationCellImageMarchingCubes::~gnomonVisualizationCellImageMarchingCubes(void)
{
    this->clear();

    delete dd;

    dd = NULL;
}

void gnomonVisualizationCellImageMarchingCubes::clear(void)
{
//    gnomonAbstractVisualization::clear();
    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }

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

    d->view->setInteractorStyle(nullptr);
//    dd->interactor_style->Delete();
}

void gnomonVisualizationCellImageMarchingCubes::setCellImage(gnomonCellImageSeries *cellImage)
{
    dd->cellImageSeries = cellImage;
    dd->cellImage = (gnomonCellImage *) cellImage->current();

    if(!dd->cellImage)
        return;

    this->setParameter("alpha",1.0);
    d->parameters["property_name"]->connect([=] (QVariant v) {
        if(!dd->cellImage)
            return;
        dd->updateValueRange();
        emit parametersChanged();
    });

    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["property_name"];
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& propertyName : dd->cellImage->cellPropertyNames()) {
         QVariant property_variant = dd->cellImage->cellProperty(propertyName)[dd->cellImage->cellIds()[0]];
         qDebug()<<propertyName<<property_variant.canConvert<double>()<<property_variant.canConvert<int>()<<property_variant.canConvert<long>();
         qDebug()<<property_variant<<property_variant.value<double>();
         if(property_variant.canConvert<double>() || property_variant.canConvert<int>()) {
                properties.append(propertyName);
         }
    }
    propertyParam->setValues(properties);
    if (properties.contains(property_name)) {
        propertyParam->setValue(property_name);
    } else {
        propertyParam->setValue(QString(""));
    }

    dd->updateValueRange();

    dtk::d_range_real *xRangeParam = (dtk::d_range_real *)d->parameters["x_range"];
    xRangeParam->setBounds({0,dd->cellImage->image()->xDim()*dd->cellImage->image()->spacing()[0]});

    dtk::d_range_real *yRangeParam = (dtk::d_range_real *)d->parameters["y_range"];
    yRangeParam->setBounds({0,dd->cellImage->image()->yDim()*dd->cellImage->image()->spacing()[1]});

    dtk::d_range_real *zRangeParam = (dtk::d_range_real *)d->parameters["z_range"];
    zRangeParam->setBounds({0,dd->cellImage->image()->zDim()*dd->cellImage->image()->spacing()[2]});

}

gnomonInteractorStyle *gnomonVisualizationCellImageMarchingCubes::interactorStyle(void)
{
    return dd->interactor_style;
}

QImage gnomonVisualizationCellImageMarchingCubes::imageRendering(void)
{
    double bounds[6];
    dd->polydata->GetBounds(bounds);
    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(dd->actor);

    return this->offscreenImageRendering();
}

void gnomonVisualizationCellImageMarchingCubes::update(void)
{
     QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();
     QString colormap_name = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->name();
     QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
     std::array<double, 2> value_range = ((dtk::d_range_real *)d->parameters["value_range"])->value();
     std::array<double, 2> x_range = ((dtk::d_range_real *)d->parameters["x_range"])->value();
     std::array<double, 2> y_range = ((dtk::d_range_real *)d->parameters["y_range"])->value();
     std::array<double, 2> z_range = ((dtk::d_range_real *)d->parameters["z_range"])->value();

    if(!dd->cellImage)
        return;

    double resampling_voxelsize =  ((dtk::d_real *)d->parameters["resolution"])->value();

    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }
    if (!dd->polydata) {
        dd->polydata = gnomonPolyDataCellImage::New();
    }
    dd->polydata->setCellImage(dd->cellImage);
    dd->polydata->set8Bit(colormap_name=="glasbey");
    dd->polydata->setPropertyName(property_name);
    dd->polydata->setResamplingSpacing(resampling_voxelsize);
    dd->polydata->setSliceRanges(x_range, y_range, z_range);
    dd->polydata->update();

    if (colormap_name=="glasbey") {
        value_range[0] = 0;
        value_range[1] = 255;
    }

    if (!dd->actor) {

        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    }
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setPolyData(dd->polydata);
    dd->actor->setColorMap(colormap);
    dd->actor->setValueRange(value_range);

    dd->interactor_style->setActor(dd->actor);

    if (!dd->actor2D)
    {
        dd->actor2D = gnomonActor2DPolyData::New();
        d->view->renderer2D()->AddActor(dd->actor2D);
    }
    dd->actor2D->setInteractor(d->view->interactor());
    dd->actor2D->setSliceThickness(0.1);
    dd->actor2D->setPolyData(dd->polydata);
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);

    double bounds[6];
    dd->polydata->GetBounds(bounds);
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::render(void)
{
   if (!dd->is2D) {
       dd->interactor_style->SetDefaultRenderer(d->view->renderer3D());
       d->view->interactor()->SetInteractorStyle(dd->interactor_style);
       d->view->interactor()->Enable();
   }
   d->view->setInteractorStyle(dd->interactor_style);

    dd->updateOpacity();
    d->view->render();
}

dtkCoreParameters gnomonVisualizationCellImageMarchingCubes::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationCellImageMarchingCubes::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationCellImageMarchingCubes::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

long gnomonVisualizationCellImageMarchingCubes::cellId(long vtkId)
{
    return dd->polydata->cellId(vtkId);
}

QVariantMap gnomonVisualizationCellImageMarchingCubes::cellInfo(long cellId)
{
    QVariantMap info;
//    info["label"] = QVariant(int(cellId));
    for (const auto& propertyName : dd->cellImage->cellPropertyNames()) {
        QMap<long, QVariant> property = dd->cellImage->cellProperty(propertyName);
        if (property.contains(cellId)) {
            info[propertyName] = property[cellId];
        }
    }

    return info;
}

void gnomonVisualizationCellImageMarchingCubes::onSliceOrientationChanged(int value)
{
    dd->actor2D->setSliceOrientation(value);
}

void gnomonVisualizationCellImageMarchingCubes::onSliceChanged(int value)
{
    dd->actor2D->setSlice(value);
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::on3D(void)
{
    dd->is2D=false;
    dd->interactor_style->setActor(dd->actor);
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::on2D(void)
{
    dd->is2D=true;
    dd->interactor_style->setActor(dd->actor2D);
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::onXY(void)
{
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::onYZ(void)
{
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::onXZ(void)
{
    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::onTimeChanged(double value)
{
    if (dd->cellImageSeries->times().contains(value)) {
        dd->cellImage = (gnomonCellImage *) dd->cellImageSeries->at(value);
        this->update();
    }
    this->render();
}

//
// gnomonVisualizationCellImageMarchingCubes.cpp ends here
