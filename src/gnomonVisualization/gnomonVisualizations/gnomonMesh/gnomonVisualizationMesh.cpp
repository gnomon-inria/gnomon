#include "gnomonVisualizationMesh.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractFormVisualization_p.h"

#include <gnomonVisualization/gnomonCoreParameterColor.h>

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonMesh/gnomonPolyDataMesh.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMeshPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationMeshPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> meshSeries;
    std::shared_ptr<gnomonMesh> mesh;

public:
    gnomonPolyDataMesh *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActorPolyData *edge_actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;
};


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMesh
// /////////////////////////////////////////////////////////////////

gnomonVisualizationMesh::gnomonVisualizationMesh(void) : gnomonAbstractVisualizationMesh(), ddd(new gnomonVisualizationMeshPrivate)
{
    d->parameters["property_name"] = new dtk::d_inliststring("", {""}, "Mesh property to be displayed");
    d->parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the mesh");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");

    d->parameters["property_name"]->connect([=] (QVariant v) {
        if(!ddd->mesh)
            return;
        this->updateValueRange();
        emit parametersChanged();
    });
}

gnomonVisualizationMesh::~gnomonVisualizationMesh(void)
{
    this->clear();
    delete ddd;
}

const QString gnomonVisualizationMesh::pluginName(void)
{
    return  "gnomonVisualizationMesh";
}

void gnomonVisualizationMesh::clear(void)
{
    if (ddd->actor) {
        ((gnomonViewForm *) d->view)->renderer3D()->RemoveActor(ddd->actor);
        ddd->actor->Delete();
        ddd->actor = nullptr;
    }

    if (ddd->edge_actor) {
        ((gnomonViewForm *) d->view)->renderer3D()->RemoveActor(ddd->edge_actor);
        ddd->edge_actor->Delete();
        ddd->edge_actor = nullptr;
    }

    if (ddd->actor2D) {
        ((gnomonViewForm *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }
}

void gnomonVisualizationMesh::setVisible(bool visible)
{
    if (ddd->actor) {
        ddd->actor->SetVisibility(visible);
    }

    if (ddd->actor2D) {
        ddd->actor2D->SetVisibility(visible);
    }
}

void gnomonVisualizationMesh::setMesh(std::shared_ptr<gnomonMeshSeries> mesh)
{
    ddd->meshSeries = mesh;
    ddd->mesh = mesh->current();

    this->setParameter("alpha",1.0);

    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["property_name"];
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& propertyName : ddd->mesh->vertexPropertyNames()) {
        if(ddd->mesh->vertexProperty(propertyName)[ddd->mesh->vertexIds()[0]].canConvert<double>()) {
            properties.append(propertyName);
        }
    }
    propertyParam->setValues(properties);
    if (properties.contains(property_name)) {
        propertyParam->setValue(property_name);
    } else {
        propertyParam->setValue(QString(""));
    }

    this->updateValueRange();
}

std::shared_ptr<gnomonMeshSeries> gnomonVisualizationMesh::mesh(void)
{
    return ddd->meshSeries;
}

void gnomonVisualizationMesh::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    if(ddd->actor) {
        ddd->actor->setOpacity(alpha);
    }

    if(ddd->edge_actor) {
        ddd->edge_actor->setWireframe(true);
        ddd->edge_actor->setOpacity(0.99*alpha);
    }

    if(ddd->actor2D) {
        ddd->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationMesh::updateValueRange(void)
{
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QMap<long, QVariant> vertexProperty;
    if(ddd->mesh->vertexPropertyNames().contains(property_name)) {
        vertexProperty = ddd->mesh->vertexProperty(property_name);
    } else {
        for (const auto& vertexId : ddd->mesh->vertexIds()) {
            vertexProperty[vertexId] = QVariant((double)vertexId);
        }
    }

    QList<double> vertexScalarPropertyValues;
    for (const auto& vertexId : ddd->mesh->vertexIds()) {
        vertexScalarPropertyValues.append(vertexProperty[vertexId].value<double>());
    }
    auto mm = std::minmax_element(vertexScalarPropertyValues.begin(),vertexScalarPropertyValues.end());


    ((dtk::d_range_real *)d->parameters["value_range"])->setBounds({*(mm.first), *(mm.second)});
}

QImage gnomonVisualizationMesh::imageRendering(void)
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

void gnomonVisualizationMesh::update(void)
{
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
    std::array<double, 2> value_range = ((dtk::d_range_real *)d->parameters["value_range"])->value();

    if(!ddd->mesh)
        return;

    if (ddd->polydata) {
        ddd->polydata->Delete();
        ddd->polydata = nullptr;
    }

    if (!ddd->polydata)
        ddd->polydata = gnomonPolyDataMesh::New();
    ddd->polydata->setMesh(ddd->mesh);
    ddd->polydata->setPropertyName(property_name);
    ddd->polydata->update();


    if (ddd->actor) {
        ((gnomonViewForm *) d->view)->renderer3D()->RemoveActor(ddd->actor);
        ddd->actor->Delete();
        ddd->actor = nullptr;
    }

    if (!ddd->actor) {
        ddd->actor = gnomonActorPolyData::New();
        ((gnomonViewForm *) d->view)->renderer3D()->AddActor(ddd->actor);
    }
    ddd->actor->setInteractor(((gnomonViewForm *) d->view)->interactor());
    ddd->actor->setPolyData(ddd->polydata);
    ddd->actor->setColorMap(colormap);
    ddd->actor->setValueRange(value_range);

    if (ddd->edge_actor) {
        ((gnomonViewForm *) d->view)->renderer3D()->RemoveActor(ddd->edge_actor);
        ddd->edge_actor->Delete();
        ddd->edge_actor = nullptr;
    }

    if (!ddd->edge_actor) {
        ddd->edge_actor = gnomonActorPolyData::New();
        ((gnomonViewForm *) d->view)->renderer3D()->AddActor(ddd->edge_actor);
    }
    ddd->edge_actor->setInteractor(((gnomonViewForm *) d->view)->interactor());
    ddd->edge_actor->setPolyData(ddd->polydata);
    ddd->edge_actor->setColor(QColor(0,0,0));
    ddd->edge_actor->setValueRange(value_range);
    ddd->edge_actor->setWireframe(true);
    ddd->edge_actor->setLinewidth(2);

    if (ddd->actor2D) {
        ((gnomonViewForm *) d->view)->renderer2D()->RemoveActor(ddd->actor2D);
        ddd->actor2D->Delete();
        ddd->actor2D = nullptr;
    }

    if (!ddd->actor2D)
    {
        ddd->actor2D = gnomonActor2DPolyData::New();
        ((gnomonViewForm *) d->view)->renderer2D()->AddActor(ddd->actor2D);
    }
    ddd->actor2D->setInteractor(((gnomonViewForm *) d->view)->interactor());
    ddd->actor2D->setSliceThickness(0.5);
    ddd->actor2D->setPolyData(ddd->polydata);
    ddd->actor2D->setColorMap(colormap);
    ddd->actor2D->setValueRange(value_range);

    double bounds[6];
    ddd->polydata->GetBounds(bounds);
    ((gnomonViewForm *) d->view)->setBounds(bounds);

    this->render();
}

void gnomonVisualizationMesh::render(void)
{
    this->updateOpacity();
    ((gnomonViewForm *) d->view)->render();
}

dtkCoreParameters gnomonVisualizationMesh::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationMesh::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationMesh::setParameters(const dtkCoreParameters& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

QMap<QString, QString> gnomonVisualizationMesh::parameterGroups(void)
{
    QMap<QString, QString> groups;
    groups["value_range"] = "rendering";
    groups["colormap"] = "rendering";
    groups["alpha"] = "rendering";
    return groups;
}

void gnomonVisualizationMesh::onSliceOrientationChanged(int value)
{
    ddd->actor2D->setSliceOrientation(value);
}

void gnomonVisualizationMesh::onSliceChanged(int value)
{
    ddd->actor2D->setSlice(value);
    this->render();
}

void gnomonVisualizationMesh::on3D(void)
{
    this->render();
}

void gnomonVisualizationMesh::on2D(void)
{
    this->render();
}

void gnomonVisualizationMesh::onXY(void)
{
    this->render();
}

void gnomonVisualizationMesh::onYZ(void)
{
    this->render();
}

void gnomonVisualizationMesh::onXZ(void)
{
    this->render();
}

void gnomonVisualizationMesh::onTimeChanged(double value)
{
    if (ddd->meshSeries->times().contains(value)) {
        ddd->mesh = ddd->meshSeries->at(value);
        this->update();
    }
    this->render();
}

const QString gnomonVisualizationMesh::name(void) {
    return "Mesh Visualization";
}

//
// gnomonVisualizationMesh.cpp ends here
