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

#include "gnomonVisualizationCellImageVolume.h"
#include "gnomonVisualizationCellImageVolumePlugin.h"

#include <dtkCore>
#include <gnomonWidgets>

void gnomonVisualizationCellImageVolumePlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellImageVolume";
    gnomonWidgets::visualizationCellImage::pluginFactory().record(plugin_name, gnomonVisualizationCellImageVolumeCreator);

    if (gnomonWidgets::visualizationCellImage::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellImageVolumePlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellImageVolume)


//
// gnomonVisualizationCellImageVolumePlugin.cpp ends here
