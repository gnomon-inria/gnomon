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

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorVolume : public gnomonInspector
{
public:
     gnomonInspectorVolume(QWidget *parent = Q_NULLPTR);
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
