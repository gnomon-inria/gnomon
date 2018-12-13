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

    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the mesh rendering");
}

gnomonVisualizationMesh::~gnomonVisualizationMesh(void)
{
    delete dd;

    dd = NULL;
}

void gnomonVisualizationMesh::setMesh(gnomonMesh *mesh)
{
    dd->mesh = mesh;
    this->update();
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

void gnomonVisualizationMesh::update(void)
{
    if(!dd->mesh)
        return;

    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }

    if (!dd->polydata)
        dd->polydata = gnomonPolyDataMesh::New();
    dd->polydata->setMesh((gnomonMesh *)dd->mesh->clone());

    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (!dd->actor)
        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setPolyData(dd->polydata);

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
