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

#include <QtGui>

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

class gnomonMeshEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorMesh : public gnomonInspector
{
public:
     gnomonInspectorMesh(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorMesh(void);

private:
    gnomonInspectorMesh(const gnomonInspectorMesh&) = delete;
    void operator = (const gnomonInspectorMesh&) = delete;

 private:
    class gnomonInspectorMeshPrivate *d;
};

//
// gnomonInspectorMesh.h ends here
