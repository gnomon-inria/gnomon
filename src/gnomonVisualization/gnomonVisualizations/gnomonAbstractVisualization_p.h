
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

#include <gnomonCore/gnomonCoreParameter>

#include "gnomonView/gnomonViewForm.h"


class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
#include <vtkSmartPointer.h>

class gnomonAbstractVisualizationPrivate
{

public:
    gnomonViewForm* view;
    
public:
    QMap<QString, gnomonCoreParameter *> parameters;

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
    vtkSmartPointer<vtkRenderer> offscreenRenderer;
    vtkSmartPointer<vtkRenderWindow> offscreenRenderWindow;
};


//
// gnomonAbstractVisualization_p.h ends here