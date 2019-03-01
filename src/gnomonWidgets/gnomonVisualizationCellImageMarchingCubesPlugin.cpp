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

#include "gnomonVisualizationCellImageMarchingCubes.h"
#include "gnomonVisualizationCellImageMarchingCubesPlugin.h"

#include <dtkCore>
#include <gnomonWidgets>

void gnomonVisualizationCellImageMarchingCubesPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellImageMarchingCubes";
    gnomonWidgets::visualizationCellImage::pluginFactory().record(plugin_name, gnomonVisualizationCellImageMarchingCubesCreator);

    if (gnomonWidgets::visualizationCellImage::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellImageMarchingCubesPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellImageMarchingCubes)


//
// gnomonVisualizationCellImageMarchingCubesPlugin.cpp ends here
