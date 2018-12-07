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

#include <QtWidgets>

#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewForm.h"

#include "gnomonPolyDataMesh.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMeshPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationMeshPrivate
{
public:
    gnomonViewForm* view;
    gnomonMesh *mesh;

public:
    QMap<QString, gnomonCoreParameter *> parameters;

public:
    gnomonPolyDataMesh *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

public:
    QMetaObject::Connection connectSliceOrientation;
    QMetaObject::Connection connectSlice;
public slots:
    void updateOpacity(void);

};

void gnomonVisualizationMeshPrivate::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)this->parameters["alpha"])->value();
    
    if(this->actor) {
        this->actor->setOpacity(alpha);
    }

    if(this->actor2D) {
        this->actor2D->setOpacity(alpha);
    }
}


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationMesh
// /////////////////////////////////////////////////////////////////

gnomonVisualizationMesh::gnomonVisualizationMesh(gnomonViewForm* view) : d(new gnomonVisualizationMeshPrivate)
{
    d->view = view;
    d->mesh = Q_NULLPTR;

    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the mesh rendering");
}

gnomonVisualizationMesh::~gnomonVisualizationMesh(void)
{
    delete d;

    d = NULL;
}

void gnomonVisualizationMesh::setMesh(gnomonMesh *mesh)
{
    d->mesh = mesh;
    this->update();
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

void gnomonVisualizationMesh::update(void)
{
    if(!d->mesh)
        return;

    if (d->polydata) {
        d->polydata->Delete();
        d->polydata = nullptr;
    }

    if (!d->polydata)
        d->polydata = gnomonPolyDataMesh::New();
    d->polydata->setMesh((gnomonMesh *)d->mesh->clone());

    if (d->actor) {
        d->view->renderer3D()->RemoveActor(d->actor);
        d->actor->Delete();
        d->actor = nullptr;
    }

    if (!d->actor)
        d->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(d->actor);
    d->actor->setInteractor(d->view->interactor());
    d->actor->setPolyData(d->polydata);

    if (d->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
        d->view->renderer2D()->RemoveActor(d->actor2D);
        d->actor2D->Delete();
        d->actor2D = nullptr;
    }

    if (!d->actor2D)
    {
        d->actor2D = gnomonActor2DPolyData::New();
        d->view->renderer2D()->AddActor(d->actor2D);
    }
    d->actor2D->setInteractor(d->view->interactor());
    d->actor2D->setSliceThickness(0.5);
    d->actor2D->setPolyData(d->polydata);

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        d->actor2D->setSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        d->actor2D->setSlice(value);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

    double bounds[6];
    d->polydata->GetBounds(bounds);
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationMesh::render(void)
{
    d->updateOpacity();
    d->view->render();
}


//
// gnomonVisualizationMesh.cpp ends here
