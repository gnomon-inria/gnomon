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


#pragma once


#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshPrivate
{
public:
    vtkSmartPointer<vtkPolyData> mesh;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    vtkRenderWindowInteractor *interactor;
};