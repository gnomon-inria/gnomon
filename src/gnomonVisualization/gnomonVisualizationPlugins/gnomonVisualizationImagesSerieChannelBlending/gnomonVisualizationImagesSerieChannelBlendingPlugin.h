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

#include "gnomonVisualizations/gnomonImagesSerie/gnomonAbstractVisualizationImagesSerie.h"

class gnomonVisualizationImagesSerieChannelBlendingPlugin: public gnomonAbstractVisualizationImagesSeriePlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationImagesSeriePlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationImagesSerieChannelBlendingPlugin" FILE "gnomonVisualizationImagesSerieChannelBlendingPlugin.json")

public:
     gnomonVisualizationImagesSerieChannelBlendingPlugin(void) {}
    ~gnomonVisualizationImagesSerieChannelBlendingPlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationImagesSerieChannelBlendingPlugin.h ends here
