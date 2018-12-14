
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


#include <gnomonWidgetsExport.h>

#include <QtWidgets>

#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewForm.h"


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
    vtkSmartPointer<vtkRenderer> offscreenRenderer;
    vtkSmartPointer<vtkRenderWindow> offscreenRenderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> offscreenRenderWindowInteractor;

public slots:
    void updateOffscreenRenderer(double bounds[6]);
    QImage offscreenImageRendering(void);
};


//
// gnomonAbstractVisualization_p.h ends here