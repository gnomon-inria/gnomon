// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:


#include <gnomonVisualizationExport.h>

#include <QtWidgets>

#include <dtkCore/dtkCoreParameters>

#include "gnomonView/gnomonViewForm.h"

class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

#include <vtkSmartPointer.h>

class xVisViewer;

class gnomonAbstractVisualizationPrivate
{

public:
    gnomonViewForm* view;

public:
    dtkCoreParameters parameters;

public:
    QMetaObject::Connection connectSliceOrientation;
    QMetaObject::Connection connectSlice;

public:
    QMetaObject::Connection connectTime;

public:
    QMetaObject::Connection connect3D;
    QMetaObject::Connection connect2D;
    QMetaObject::Connection connectXY;
    QMetaObject::Connection connectXZ;
    QMetaObject::Connection connectYZ;

public:
    vtkSmartPointer <vtkRenderer> offscreenRenderer;

    xVisViewer *offscreenRenderWindow = 0;
};

//
// gnomonAbstractVisualization_p.h ends here
