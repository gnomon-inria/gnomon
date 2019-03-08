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

#include "gnomonVisualizationImagesSerieChannelBlendingPlugin.h"

#include "gnomonVisualizations/gnomonImagesSerie/gnomonVisualizationImagesSerieChannelBlending.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationImagesSerieChannelBlendingPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationImagesSerieChannelBlending";
    gnomonVisualization::visualizationImagesSerie::pluginFactory().record(plugin_name, gnomonVisualizationImagesSerieChannelBlendingCreator);

    if (gnomonVisualization::visualizationImagesSerie::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationImagesSerieChannelBlendingPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationImagesSerieChannelBlending)


//
// gnomonVisualizationImagesSerieChannelBlendingPlugin.cpp ends here
