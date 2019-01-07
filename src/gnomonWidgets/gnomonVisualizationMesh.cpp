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
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCoreParameter>
#include "gnomonCoreParameterColor.h"

#include "gnomonViewForm.h"

#include "gnomonPolyDataMesh.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

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

gnomonVisualizationMesh::gnomonVisualizationMesh(gnomonViewForm* view) : gnomonAbstractVisualization(view), dd(new gnomonVisualizationMeshPrivate)
{
    dd->mesh = Q_NULLPTR;

    d->parameters["property_name"] = new gnomonCoreParameterStringList("", {""}, "Mesh property to be displayed");
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
    delete dd;

    dd = NULL;
}

void gnomonVisualizationMesh::setMesh(gnomonMesh *mesh)
{
    dd->mesh = mesh;

    this->setParameter("alpha",1.0);
    
    gnomonCoreParameterStringList *propertyParam = (gnomonCoreParameterStringList *)d->parameters["property_name"];
    QStringList properties = {""};
    for (const auto& propertyName : dd->mesh->vertexPropertyNames()) {
        if(dd->mesh->vertexProperty(propertyName)[dd->mesh->vertexIds()[0]].canConvert<double>()) {
            properties.append(propertyName);
        }
    }
    propertyParam->setValues(properties);
    propertyParam->setValue(QString(""));
    
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
    qDebug()<<Q_FUNC_INFO;

    QString property_name = ((gnomonCoreParameterStringList *)d->parameters["property_name"])->value();

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
    QString property_name = ((gnomonCoreParameterStringList *)d->parameters["property_name"])->value();
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

//
// gnomonVisualizationMesh.cpp ends here
