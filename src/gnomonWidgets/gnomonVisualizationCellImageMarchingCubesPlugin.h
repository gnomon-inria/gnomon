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
#include <gnomonAbstractVisualizationCellImage>

class gnomonVisualizationCellImageMarchingCubesPlugin: public gnomonAbstractVisualizationCellImagePlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationCellImagePlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationCellImageMarchingCubesPlugin" FILE "gnomonVisualizationCellImageMarchingCubesPlugin.json")

public:
     gnomonVisualizationCellImageMarchingCubesPlugin(void) {}
    ~gnomonVisualizationCellImageMarchingCubesPlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationCellImageMarchingCubesPlugin.h ends here
