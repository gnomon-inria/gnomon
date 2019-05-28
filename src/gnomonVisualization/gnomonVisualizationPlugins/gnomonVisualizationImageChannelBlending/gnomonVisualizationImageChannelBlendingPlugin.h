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

#include "gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h"

class gnomonVisualizationImageChannelBlendingPlugin: public gnomonAbstractVisualizationImagePlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationImagePlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationImageChannelBlendingPlugin" FILE "gnomonVisualizationImageChannelBlendingPlugin.json")

public:
     gnomonVisualizationImageChannelBlendingPlugin(void) {}
    ~gnomonVisualizationImageChannelBlendingPlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationImageChannelBlendingPlugin.h ends here
