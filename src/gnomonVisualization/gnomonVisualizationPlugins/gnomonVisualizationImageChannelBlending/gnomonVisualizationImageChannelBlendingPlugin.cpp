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

#include "gnomonVisualizationImageChannelBlendingPlugin.h"

#include "gnomonVisualizations/gnomonImage/gnomonVisualizationImageChannelBlending.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationImageChannelBlendingPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationImageChannelBlending";
    gnomonVisualization::visualizationImage::pluginFactory().record(plugin_name, gnomonVisualizationImageChannelBlendingCreator);

    if (gnomonVisualization::visualizationImage::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationImageChannelBlendingPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationImageChannelBlending)


//
// gnomonVisualizationImageChannelBlendingPlugin.cpp ends here
