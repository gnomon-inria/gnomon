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

#include "gnomonInspector.h"

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonInspectorImage : public gnomonInspector
{
protected:
    gnomonInspectorImage(void);
    ~gnomonInspectorImage(void);

private:
    gnomonInspectorImage(const gnomonInspectorImage&) = delete;
    void operator = (const gnomonInspectorImage&) = delete;

 private:
    class gnomonInspectorImagePrivate *d;
};

//
// gnomonInspectorImage.h ends here
