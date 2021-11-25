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

#include "gnomonVisualizationMesh.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonVisualization>
//#include <gnomonWidgets>

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonMesh/gnomonPolyDataMesh.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMeshPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationMeshPrivate
{
public:
    gnomonMeshSeries *meshSeries;
    gnomonMesh *mesh;


public:
    gnomonPolyDataMesh *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActorPolyData *edge_actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

};


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMesh
// /////////////////////////////////////////////////////////////////

gnomonVisualizationMesh::gnomonVisualizationMesh(void) : gnomonAbstractVisualizationMesh(), dd(new gnomonVisualizationMeshPrivate)
{
    dd->mesh = Q_NULLPTR;

    d->parameters["property_name"] = new dtk::d_inliststring("", {""}, "Mesh property to be displayed");
    d->parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("gray", "Colormap to apply to the mesh");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");

    d->parameters["property_name"]->connect([=] (QVariant v) {
        if(!dd->mesh)
            return;
        this->updateValueRange();
        emit parametersChanged();
    });
}

gnomonVisualizationMesh::~gnomonVisualizationMesh(void)
{
    this->clear();

    delete dd;

    dd = NULL;
}

void gnomonVisualizationMesh::clear(void)
{
    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (dd->edge_actor) {
        d->view->renderer3D()->RemoveActor(dd->edge_actor);
        dd->edge_actor->Delete();
        dd->edge_actor = nullptr;
    }

    if (dd->actor2D) {
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }
}

void gnomonVisualizationMesh::setMesh(gnomonMeshSeries *mesh)
{
    dd->meshSeries = mesh;
    dd->mesh = (gnomonMesh *) mesh->current();

    this->setParameter("alpha",1.0);

    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["property_name"];
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& propertyName : dd->mesh->vertexPropertyNames()) {
        if(dd->mesh->vertexProperty(propertyName)[dd->mesh->vertexIds()[0]].canConvert<double>()) {
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

gnomonMeshSeries *gnomonVisualizationMesh::mesh(void)
{
    return dd->meshSeries;
}

void gnomonVisualizationMesh::updateOpacity(void)
{
    double alpha = ((dtk::d_real *)d->parameters["alpha"])->value();

    if(dd->actor) {
        dd->actor->setOpacity(alpha);
    }

    if(dd->edge_actor) {
        dd->edge_actor->setWireframe(true);
        dd->edge_actor->setOpacity(0.99*alpha);
    }

    if(dd->actor2D) {
        dd->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationMesh::updateValueRange(void)
{
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();

    QMap<long, QVariant> vertexProperty;
    if(dd->mesh->vertexPropertyNames().contains(property_name)) {
        vertexProperty = dd->mesh->vertexProperty(property_name);
    } else {
        for (const auto& vertexId : dd->mesh->vertexIds()) {
            vertexProperty[vertexId] = QVariant((double)vertexId);
        }
    }

    QList<double> vertexScalarPropertyValues;
    for (const auto& vertexId : dd->mesh->vertexIds()) {
        vertexScalarPropertyValues.append(vertexProperty[vertexId].value<double>());
    }
    auto mm = std::minmax_element(vertexScalarPropertyValues.begin(),vertexScalarPropertyValues.end());


    ((dtk::d_range_real *)d->parameters["value_range"])->setBounds({*(mm.first), *(mm.second)});
}

QImage gnomonVisualizationMesh::imageRendering(void)
{
    double bounds[6];
    dd->polydata->GetBounds(bounds);

    if (bounds[4]==bounds[5]) {
        double size = ((bounds[1]-bounds[0])+(bounds[3]-bounds[2]))/4;
        bounds[4] = bounds[4] - size/2.;
        bounds[5] = bounds[5] + size/2.;
    }

    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    this->offscreenRenderer()->AddActor(dd->actor);

    return this->offscreenImageRendering();
}

void gnomonVisualizationMesh::update(void)
{
    QString property_name = ((dtk::d_inliststring *)d->parameters["property_name"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
    std::array<double, 2> value_range = ((dtk::d_range_real *)d->parameters["value_range"])->value();

    if(!dd->mesh)
        return;

    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }

    if (!dd->polydata)
        dd->polydata = gnomonPolyDataMesh::New();
    dd->polydata->setMesh((gnomonMesh *)dd->mesh->clone());
    dd->polydata->setPropertyName(property_name);
    dd->polydata->update();


    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (!dd->actor) {
        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    }
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setPolyData(dd->polydata);
    dd->actor->setColorMap(colormap);
    dd->actor->setValueRange(value_range);

    if (dd->edge_actor) {
        d->view->renderer3D()->RemoveActor(dd->edge_actor);
        dd->edge_actor->Delete();
        dd->edge_actor = nullptr;
    }

    if (!dd->edge_actor) {
        dd->edge_actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->edge_actor);
    }
    dd->edge_actor->setInteractor(d->view->interactor());
    dd->edge_actor->setPolyData(dd->polydata);
    dd->edge_actor->setColor(QColor(0,0,0));
    dd->edge_actor->setValueRange(value_range);
    dd->edge_actor->setWireframe(true);
    dd->edge_actor->setLinewidth(2);

    if (dd->actor2D) {
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }

    if (!dd->actor2D)
    {
        dd->actor2D = gnomonActor2DPolyData::New();
        d->view->renderer2D()->AddActor(dd->actor2D);
    }
    dd->actor2D->setInteractor(d->view->interactor());
    dd->actor2D->setSliceThickness(0.5);
    dd->actor2D->setPolyData(dd->polydata);
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);

    double bounds[6];
    dd->polydata->GetBounds(bounds);
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationMesh::render(void)
{
    this->updateOpacity();
    d->view->render();
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

void gnomonVisualizationMesh::onSliceOrientationChanged(int value)
{
    dd->actor2D->setSliceOrientation(value);
}

void gnomonVisualizationMesh::onSliceChanged(int value)
{
    dd->actor2D->setSlice(value);
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
    if (dd->meshSeries->times().contains(value)) {
        dd->mesh = (gnomonMesh *) dd->meshSeries->at(value);
        this->update();
    }
    this->render();
}

//
// gnomonVisualizationMesh.cpp ends here
