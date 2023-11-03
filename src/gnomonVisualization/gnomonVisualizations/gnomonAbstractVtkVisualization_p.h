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

#include <dtkCore/dtkCoreParameters>

#include "gnomonView/gnomonVtkView.h"

class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

#include <vtkSmartPointer.h>

class gnomonAbstractVtkVisualizationPrivate
{
public:
    dtkCoreParameters parameters;

public:
    QMetaObject::Connection connectSliceOrientation;
    QMetaObject::Connection connectSlice;

public:
    QMetaObject::Connection connect3D;
    QMetaObject::Connection connect2D;
    QMetaObject::Connection connectXY;
    QMetaObject::Connection connectXZ;
    QMetaObject::Connection connectYZ;

public:
    vtkSmartPointer<vtkRenderer>     offscreenRenderer;
    vtkSmartPointer<vtkRenderWindow> offscreenRenderWindow;
};

//
// gnomonAbstractVtkVisualization_p.h ends here
