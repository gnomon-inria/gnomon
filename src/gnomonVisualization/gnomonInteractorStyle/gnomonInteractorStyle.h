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

#include <QtCore>

#include <gnomonVisualizationExport>

#include <vtkInteractorStyleTrackballCamera.h>

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
public:
    ~gnomonInteractorStyle(void);
};

//
// gnomonInteractorStyle.h ends here
