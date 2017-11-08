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

#include "gnomonActorMesh.h"
#include "gnomonActorMesh_p.h"

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshClippingObserver
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshClippingObserver : public vtkCommand
{
public:
    static gnomonActorMeshClippingObserver *New(void)
    {
        return new gnomonActorMeshClippingObserver;
    }

    virtual void Execute(vtkObject *caller, unsigned long event, void *)
    {
        if(event != vtkCommand::InteractionEvent)
            return;
    }
};


// /////////////////////////////////////////////////////////////////
// gnomonActorMesh
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMesh);

void gnomonActorMesh::setInteractor(void *interactor)
{
    d->interactor = static_cast<vtkRenderWindowInteractor *>(interactor);
}

void gnomonActorMesh::setMesh(vtkPolyData *mesh)
{
    d->mesh = mesh;

    this->update();
}

void gnomonActorMesh::update(void)
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

gnomonActorMesh::gnomonActorMesh(void) : gnomonActor(), d(new gnomonActorMeshPrivate)
{
    d->mesh = Q_NULLPTR;
}

gnomonActorMesh::~gnomonActorMesh(void)
{
    delete d;

    d = NULL;
}

//
// gnomonActorMesh.cpp ends here
