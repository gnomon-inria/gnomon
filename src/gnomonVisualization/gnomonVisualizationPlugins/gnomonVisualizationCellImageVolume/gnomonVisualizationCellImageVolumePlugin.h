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

#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h"

class gnomonVisualizationCellImageVolumePlugin: public gnomonAbstractVisualizationCellImagePlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationCellImagePlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationCellImageVolumePlugin" FILE "gnomonVisualizationCellImageVolumePlugin.json")

public:
     gnomonVisualizationCellImageVolumePlugin(void) {}
    ~gnomonVisualizationCellImageVolumePlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationCellImageVolumePlugin.h ends here
