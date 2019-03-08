// Version: $Id: 71d7118592ad295c11438516a544baf94d12696c $
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

#include <gnomonVisualizationExport.h>

#include <vtkPolyData.h>

class GNOMONVISUALIZATION_EXPORT gnomonPolyData : public QObject, public vtkPolyData
{
public:
    virtual ~gnomonPolyData(void);

public slots:
    virtual void update(void) = 0;
};

//
// gnomonPolyData.h ends here
