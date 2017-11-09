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

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorVolume : public gnomonInspector
{
protected:
     gnomonInspectorVolume(void);
    ~gnomonInspectorVolume(void);

private:
    gnomonInspectorVolume(const gnomonInspectorVolume&) = delete;
    void operator = (const gnomonInspectorVolume&) = delete;

public:
    gnomonClutEditor *editor(void) const;

 private:
    class gnomonInspectorVolumePrivate *d;
};

//
// gnomonInspectorVolume.h ends here
