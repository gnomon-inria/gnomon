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

#include "gnomonInspectorImage.h"

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorSlicePlanes : public gnomonInspectorImage
{
    Q_OBJECT

public:
     gnomonInspectorSlicePlanes(void);
    ~gnomonInspectorSlicePlanes(void);

private:
    gnomonInspectorSlicePlanes(const gnomonInspectorSlicePlanes&) = delete;
    void operator = (const gnomonInspectorSlicePlanes&) = delete;

 public:
    double opacity();
    void setOpacity(double);

 signals:
    void opacityChanged(double);

 private:
    class gnomonInspectorSlicePlanesPrivate *d;
};

//
// gnomonInspectorSlicePlanes.h ends here
