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

#include <dtkCore>
#include "gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h"

class gnomonVisualizationMeshPlugin: public gnomonAbstractVisualizationMeshPlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationMeshPlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationMeshPlugin" FILE "gnomonVisualizationMeshPlugin.json")

public:
     gnomonVisualizationMeshPlugin(void) {}
    ~gnomonVisualizationMeshPlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationMeshPlugin.h ends here
