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

#include <gnomonVisualizationExport.h>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshPrivate
// /////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonActorMeshPrivate
{
public:
    vtkSmartPointer<vtkPolyData> mesh;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    vtkRenderWindowInteractor *interactor;
};