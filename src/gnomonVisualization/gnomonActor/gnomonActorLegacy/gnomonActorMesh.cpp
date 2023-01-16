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

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
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

void gnomonActorMesh::setInteractor(vtkSmartPointer<vtkRenderWindowInteractor>s*interactor)
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

    if(!d->colorFunction) {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->colorFunction->SetColorSpaceToRGB();
        d->colorFunction->RemoveAllPoints();
        d->colorFunction->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
        d->colorFunction->AddRGBPoint(0.5, 0.0, 1.0, 0.0);
        d->colorFunction->AddRGBPoint(1.0, 1.0, 0.0, 0.0);
        d->colorFunction->ClampingOn();
    }
    d->colorFunction->Modified();

    if(!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(d->mesh);
        d->mapper->SetLookupTable(d->colorFunction);
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);

        this->AddPart(d->actor);
    }

    d->interactor->Render();
}

void gnomonActorMesh::hide(void)
{
    this->VisibilityOff();

    d->interactor->Render();
}

void gnomonActorMesh::show(void)
{
    this->VisibilityOn();

    d->interactor->Render();
}

void gnomonActorMesh::setColorTransferFunction(vtkColorTransferFunction *func)
{
    d->colorFunction = func;

    this->update();
}

gnomonActorMesh::gnomonActorMesh(void) : gnomonActor(), d(new gnomonActorMeshPrivate)
{
    d->mesh = Q_NULLPTR;
    d->interactor = Q_NULLPTR;
}

gnomonActorMesh::~gnomonActorMesh(void)
{
    delete d;

    d = NULL;
}

//
// gnomonActorMesh.cpp ends here
