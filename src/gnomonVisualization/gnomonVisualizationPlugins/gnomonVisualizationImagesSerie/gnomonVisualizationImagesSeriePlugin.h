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

class gnomonVisualizationImagesSeriePlugin: public gnomonAbstractVisualizationImagesSeriePlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationImagesSeriePlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationImagesSeriePlugin" FILE "gnomonVisualizationImagesSeriePlugin.json")

public:
     gnomonVisualizationImagesSeriePlugin(void) {}
    ~gnomonVisualizationImagesSeriePlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationImagesSeriePlugin.h ends here
