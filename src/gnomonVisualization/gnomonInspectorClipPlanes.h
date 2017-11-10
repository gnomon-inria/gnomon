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

class GNOMONVISUALIZATION_EXPORT gnomonInspectorClipPlanes : public gnomonInspectorImage
{
public:
     gnomonInspectorClipPlanes(void);
    ~gnomonInspectorClipPlanes(void);

private:
    gnomonInspectorClipPlanes(const gnomonInspectorClipPlanes&) = delete;
    void operator = (const gnomonInspectorClipPlanes&) = delete;

public:
    gnomonClutEditor *editor(void) const;

 private:
    class gnomonInspectorClipPlanesPrivate *d;
};

//
// gnomonInspectorClipPlanes.h ends here
