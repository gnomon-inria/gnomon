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


#include <gnomonPolyDataCellImage>
#include <gnomonCellImage>

#include <dtkImagingCore>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
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
    gnomonCellImage *cellimage;

    vtkSmartPointer<gnomonPolyDataCellImage> polydata;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

    bool modified;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellImage
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellImage);


void gnomonActorMeshCellImage::setCellImage(gnomonCellImage *cellimage)
{
    dd->cellimage = cellimage;

    this->modified();
    this->update();
}

void gnomonActorMeshCellImage::modified(void)
{
    dd->modified = true;
}

void gnomonActorMeshCellImage::update(void)
{
    if(!dd->cellimage)
        return;

    if(!dd->polydata)
        dd->polydata = gnomonPolyDataCellImage::New();
    dd->polydata->setCellImage(dd->cellimage);
    dd->polydata->update();

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetScalarRange(0, dd->cellimage->cellCount()-1);
    }
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
    dd->cellimage = Q_NULLPTR;
}

gnomonActorMeshCellImage::~gnomonActorMeshCellImage(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
