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
#include "gnomonActorMeshCellImage.h"

#include "gnomonActor/gnomonCellImage/gnomonPolyDataCellImage.h"

#include <dtkImagingCore>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>



// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellImagePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellImagePrivate
{
public:
    vtkSmartPointer<gnomonPolyDataCellImage> polydata;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellImage);


void gnomonActorMeshCellImage::setPolyData(gnomonPolyDataCellImage *polydata)
{
    dd->polydata = polydata;

    this->modified();
    this->update();
}

void gnomonActorMeshCellImage::modified(void)
{
    dd->modified = true;
}

void gnomonActorMeshCellImage::update(void)
{
    if(!dd->polydata)
        return;

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    vtkSmartPointer<vtkDoubleArray> cellData = (vtkDoubleArray *) dd->polydata->GetCellData()->GetArray(0);
    d->mapper->SetScalarRange(cellData->GetRange());
    d->mapper->SetInputData(dd->polydata);
    d->mapper->Update();

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);
        this->AddPart(d->actor);
    }
    d->actor->Modified();

    dd->modified = false;
}

gnomonActorMeshCellImage::gnomonActorMeshCellImage(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellImagePrivate)
{
    dd->polydata = Q_NULLPTR;
}

gnomonActorMeshCellImage::~gnomonActorMeshCellImage(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
