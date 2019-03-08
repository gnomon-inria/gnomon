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

#include "gnomonVisualizationImagesSeriePlugin.h"

#include "gnomonVisualizations/gnomonImagesSerie/gnomonVisualizationImagesSerie.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationImagesSeriePlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationImagesSerie";
    gnomonVisualization::visualizationImagesSerie::pluginFactory().record(plugin_name, gnomonVisualizationImagesSerieCreator);

    if (gnomonVisualization::visualizationImagesSerie::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationImagesSeriePlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationImagesSerie)


//
// gnomonVisualizationImagesSeriePlugin.cpp ends here
