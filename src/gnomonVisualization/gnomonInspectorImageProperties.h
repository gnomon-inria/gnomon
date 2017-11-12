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

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorImageProperties : public gnomonInspectorImage
{
public:
     gnomonInspectorImageProperties(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorImageProperties(void);

private:
    gnomonInspectorImageProperties(const gnomonInspectorImageProperties&) = delete;
    void operator = (const gnomonInspectorImageProperties&) = delete;

 private:
    class gnomonInspectorImagePropertiesPrivate *d;
};

//
// gnomonInspectorImageProperties.h ends here
