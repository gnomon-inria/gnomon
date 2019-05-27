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
#include <gnomonWidgets>

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
    gnomonMesh *mesh;

public:
    gnomonPolyDataMesh *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

};


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMesh
// /////////////////////////////////////////////////////////////////

gnomonVisualizationMesh::gnomonVisualizationMesh(void) : gnomonAbstractVisualizationMesh(), dd(new gnomonVisualizationMeshPrivate)
{
    dd->mesh = Q_NULLPTR;

    d->parameters["property_name"] = new gnomonCoreParameterString("", {""}, "Mesh property to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterDoubleRange(0., 1., 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("grey", "Colormap to apply to the mesh");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the mesh rendering");

    connect(d->parameters["property_name"], &gnomonCoreParameter::valueChanged, [=] () {
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

    if (dd->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }
}

void gnomonVisualizationMesh::setMesh(gnomonMesh *mesh)
{
    dd->mesh = mesh;

    this->setParameter("alpha",1.0);

    gnomonCoreParameterString *propertyParam = (gnomonCoreParameterString *)d->parameters["property_name"];
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();

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

void gnomonVisualizationMesh::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    
    if(dd->actor) {
        dd->actor->setOpacity(alpha);
    }

    if(dd->actor2D) {
        dd->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationMesh::updateValueRange(void)
{
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();

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


    ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->setMinimumValue(*(mm.first));
    ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->setMaximumValue(*(mm.second));
}

QImage gnomonVisualizationMesh::imageRendering(void)
{
    d->updateOffscreenRenderer(dd->polydata->GetBounds());

    d->offscreenRenderer->AddActor(dd->actor);

    return d->offscreenImageRendering();
}

void gnomonVisualizationMesh::update(void)
{
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
    QList<double> value_range = ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->value();

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

    if (dd->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
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

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->actor2D->setSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        dd->actor2D->setSlice(value);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

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

QMap<QString, gnomonCoreParameter *> gnomonVisualizationMesh::parameters(void) const
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

void gnomonVisualizationMesh::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
//            d->parameters[param] = parameters[param];
            d->parameters[param]->copy(parameters[param]);
        }
    }
}

void gnomonVisualizationMesh::onTimeChanged(double value)
{
    qDebug()<<Q_FUNC_INFO<<"Time changed"<<value;
    this->render();
}

//
// gnomonVisualizationMesh.cpp ends here
