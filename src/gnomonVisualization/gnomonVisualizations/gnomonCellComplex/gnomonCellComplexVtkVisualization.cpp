#include "gnomonCellComplexVtkVisualization.h"

#include <gnomonVisualization/gnomonCoreParameterColor>

#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractVtkVisualization_p.h"

#include "gnomonView/gnomonVtkView.h"

#include "gnomonActor/gnomonCellComplex/gnomonPolyDataCellComplex.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// /////////////////////////////////////////////////////////////////
// gnomonCellComplexVtkVisualizationPrivate
// /////////////////////////////////////////////////////////////////

class gnomonCellComplexVtkVisualizationPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> cellComplexSeries;
    std::shared_ptr<gnomonCellComplex> cellComplex;

public:
    gnomonPolyDataCellComplex *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

    gnomonCellComplexVtkVisualization *q;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
};

void gnomonCellComplexVtkVisualizationPrivate::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)q->parameters()["alpha"])->value();

    if(this->actor) {
        this->actor->setOpacity(alpha);
    }

    if(this->actor2D) {
        this->actor2D->setOpacity(alpha);
    }
}

void gnomonCellComplexVtkVisualizationPrivate::updateValueRange(void)
{
    QString property_name = ((dtk::d_inliststring *)q->parameters()["property_name"])->value();

    QMap<long, QVariant> cellProperty;
    if(this->cellComplex->elementPropertyNames(3).contains(property_name)) {
        cellProperty = this->cellComplex->elementProperty(3,property_name);
    } else {
        for (const auto& cellId : this->cellComplex->elementIds(3)) {
            cellProperty[cellId] = QVariant((double)cellId);
        }
    }

    QList<double> cellScalarPropertyValues;
    for (const auto& cellId : this->cellComplex->elementIds(3)) {
        cellScalarPropertyValues.append(cellProperty[cellId].value<double>());
    }
    auto mm = std::minmax_element(cellScalarPropertyValues.begin(),cellScalarPropertyValues.end());

    ((dtk::d_range_real *)q->parameters()["value_range"])->setBounds({*(mm.first),*(mm.second)});
}


// /////////////////////////////////////////////////////////////////
// gnomonCellComplexVtkVisualization
// /////////////////////////////////////////////////////////////////

gnomonCellComplexVtkVisualization::gnomonCellComplexVtkVisualization(void) : gnomonAbstractCellComplexVtkVisualization(), ddd(new gnomonCellComplexVtkVisualizationPrivate)
{

    ddd->q = this;

    d->parameters["property_name"] = new dtk::d_inliststring("property", "", {""}, "CellComplex property to be displayed");
    d->parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("colormap", "glasbey", "Colormap to apply to the cellComplex");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the cellComplex rendering");
    d->parameters["scale_factor"] = new dtk::d_real("scale_factor", 0.99, 0, 1, 2, "Scale for cell surface visualization");
}

gnomonCellComplexVtkVisualization::~gnomonCellComplexVtkVisualization(void)
{
    if (ddd->actor) {
        ddd->actor->Delete();
        ddd->actor = nullptr;
    }
    if (ddd->actor2D) {
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }
    delete ddd;
    ddd = NULL;
}

const QString gnomonCellComplexVtkVisualization::pluginName(void)
{
    return  "gnomonCellComplexVtkVisualization";
}

void gnomonCellComplexVtkVisualization::clear(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->actor) {
            view->renderer3D()->RemoveActor(ddd->actor);
        }
        if (ddd->actor2D) {
            view->renderer2D()->RemoveActor(ddd->actor2D);
        }
    }
}

void gnomonCellComplexVtkVisualization::fill(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        if (ddd->actor) {
            view->renderer3D()->AddActor(ddd->actor);
        }
        if (ddd->actor2D) {
            view->renderer2D()->AddActor(ddd->actor2D);
        }
    }
}

void gnomonCellComplexVtkVisualization::setVisible(bool visible)
{
    if (ddd->actor) {
        ddd->actor->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonCellComplexVtkVisualization::setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplexSeries)
{
    ddd->cellComplexSeries = cellComplexSeries;
    ddd->cellComplex = cellComplexSeries->current();

    this->setParameter("alpha",1.0);
    d->parameters["property_name"]->connect([=] (QVariant v) {
        if(!ddd->cellComplex)
            return;
        ddd->updateValueRange();
        emit parametersChanged();
    });

    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["property_name"];
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& prop : ddd->cellComplex->elementPropertyNames(3)) {
        if(ddd->cellComplex->elementProperty(3,prop)[ddd->cellComplex->elementIds(3)[0]].canConvert<double>()) {
            properties.append(prop);
        }
    }
    propertyParam->setValues(properties);
    if (properties.contains(property_name)) {
        propertyParam->setValue(property_name);
    } else {
        propertyParam->setValue(QString(""));
    }

    ddd->updateValueRange();
}

std::shared_ptr<gnomonCellComplexSeries> gnomonCellComplexVtkVisualization::cellComplex(void)
{
    return ddd->cellComplexSeries;
}

QImage gnomonCellComplexVtkVisualization::imageRendering(void)
{
    double bounds[6];
    ddd->polydata->GetBounds(bounds);

    if (bounds[4]==bounds[5]) {
        double size = ((bounds[1]-bounds[0])+(bounds[3]-bounds[2]))/4;
        bounds[4] = bounds[4] - size/2.;
        bounds[5] = bounds[5] + size/2.;
    }

    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(ddd->actor);

    return this->offscreenImageRendering();
}

void gnomonCellComplexVtkVisualization::update(void)
{
    // TODO: later
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();
    QString colormap_name = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->name();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
    std::array<double, 2> value_range = ((dtk::d_range_real *)d->parameters["value_range"])->value();
    double scale = ((dtk::d_real *)d->parameters["scale_factor"])->value();

    if(!ddd->cellComplex)
        return;

    if (ddd->polydata) {
        ddd->polydata->Delete();
        ddd->polydata = nullptr;
    }

    if (!ddd->polydata)
        ddd->polydata = gnomonPolyDataCellComplex::New();
    ddd->polydata->setCellComplex(ddd->cellComplex);
    ddd->polydata->set8Bit(colormap_name=="glasbey");
    ddd->polydata->setPropertyName(property_name);
    ddd->polydata->setScaleFactor(scale);
    ddd->polydata->update();

    if (colormap_name == "glasbey") {
        value_range[0] = 0;
        value_range[1] = 255;
    }

//    if (ddd->actor) {
//        ((gnomonVtkView *) d->view)->renderer3D()->RemoveActor(ddd->actor);
//        ddd->actor->Delete();
//        ddd->actor = nullptr;
//    }

    if (!ddd->actor) {
        ddd->actor = gnomonActorPolyData::New();
        ((gnomonVtkView *) d->view)->renderer3D()->AddActor(ddd->actor);
    }
    ddd->actor->setInteractor(((gnomonVtkView *) d->view)->interactor());
    ddd->actor->setPolyData(ddd->polydata);
    ddd->actor->setColorMap(colormap);
    ddd->actor->setValueRange(value_range);

//    if (ddd->actor2D) {
//        ((gnomonVtkView *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
//        ddd->actor2D->Delete();
//        ddd->actor2D = nullptr;
//    }

    if (!ddd->actor2D)
    {
        ddd->actor2D = gnomonActor2DPolyData::New();
        ((gnomonVtkView *) d->view)->renderer2D()->AddActor(ddd->actor2D);
    }
    ddd->actor2D->setInteractor(((gnomonVtkView *) d->view)->interactor());
    ddd->actor2D->setSliceThickness(0.5);
    ddd->actor2D->setPolyData(ddd->polydata);
    ddd->actor2D->setColorMap(colormap);
    ddd->actor2D->setValueRange(value_range);

    double bounds[6];
    ddd->polydata->GetBounds(bounds);
    ((gnomonVtkView *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonCellComplexVtkVisualization::render(void)
{
    ddd->updateOpacity();
    ((gnomonVtkView *) d->view)->render();
}

dtkCoreParameters gnomonCellComplexVtkVisualization::parameters(void) const
{
    return d->parameters;
}

void gnomonCellComplexVtkVisualization::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonCellComplexVtkVisualization::setParameters(const dtkCoreParameters& parameters)
{
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonCellComplexVtkVisualization::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "rendering";
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonCellComplexVtkVisualization::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}

void gnomonCellComplexVtkVisualization::onSliceChanged(double value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonCellComplexVtkVisualization::on3D(void)
{
    this->render();
}

void gnomonCellComplexVtkVisualization::on2D(void)
{
    this->render();
}

void gnomonCellComplexVtkVisualization::onXY(void)
{
    this->render();
}

void gnomonCellComplexVtkVisualization::onYZ(void)
{
    this->render();
}

void gnomonCellComplexVtkVisualization::onXZ(void)
{
    this->render();
}

void gnomonCellComplexVtkVisualization::onTimeChanged(double value)
{
    if(this->cellComplex()) {
        if (ddd->cellComplexSeries->times().contains(value)) {
            ddd->cellComplex = ddd->cellComplexSeries->at(value);
            this->update();
        }
        this->render();
    }
}

const QString gnomonCellComplexVtkVisualization::name(void) {
    return "Cell Complex Visualization";
}

QString gnomonCellComplexVtkVisualization::documentation(void) {
    return "Visualize a CellComplex as a surface mesh.";
}

//
// gnomonCellComplexVtkVisualization.cpp ends here
