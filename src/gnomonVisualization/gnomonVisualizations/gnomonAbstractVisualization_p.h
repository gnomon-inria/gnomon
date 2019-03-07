
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
    vtkSmartPointer<vtkRenderer> offscreenRenderer;
    vtkSmartPointer<vtkRenderWindow> offscreenRenderWindow;

public slots:
    void updateOffscreenRenderer(double bounds[6]);
    QImage offscreenImageRendering(void);
};


//
// gnomonAbstractVisualization_p.h ends here