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

#include "tissueActorMesh.h"

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// tissueActorMeshClippingObserver
// /////////////////////////////////////////////////////////////////

class tissueActorMeshClippingObserver : public vtkCommand
{
public:
    static tissueActorMeshClippingObserver *New(void)
    {
        return new tissueActorMeshClippingObserver;
    }

    virtual void Execute(vtkObject *caller, unsigned long event, void *)
    {
        if(event != vtkCommand::InteractionEvent)
            return;
    }
};

// /////////////////////////////////////////////////////////////////
// tissueActorMeshPrivate
// /////////////////////////////////////////////////////////////////

class tissueActorMeshPrivate
{
public:
    vtkSmartPointer<vtkPolyData> mesh;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    vtkRenderWindowInteractor *interactor;
};

// /////////////////////////////////////////////////////////////////
// tissueActorMesh
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(tissueActorMesh);

void tissueActorMesh::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void tissueActorMesh::setMesh(vtkPolyData *mesh)
{
    d->mesh = mesh;

    this->update();
}

void tissueActorMesh::update(void)
{
    if(!d->mesh)
        return;

    if(!d->interactor)
        return;

    if(!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(d->mesh);
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);

        this->AddPart(d->actor);
    }

    d->interactor->Render();
}

tissueActorMesh::tissueActorMesh(void) : tissueActor(), d(new tissueActorMeshPrivate)
{
    d->mesh = Q_NULLPTR;
}

tissueActorMesh::~tissueActorMesh(void)
{
    delete d;

    d = NULL;
}

//
// tissueActorMesh.cpp ends here
