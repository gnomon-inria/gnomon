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

#include "gnomonVisualizationCellImageVolumePlugin.h"

#include "gnomonVisualizations/gnomonCellImage/gnomonVisualizationCellImageVolume.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationCellImageVolumePlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellImageVolume";
    gnomonVisualization::visualizationCellImage::pluginFactory().record(plugin_name, gnomonVisualizationCellImageVolumeCreator);

    if (gnomonVisualization::visualizationCellImage::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellImageVolumePlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellImageVolume)


//
// gnomonVisualizationCellImageVolumePlugin.cpp ends here
