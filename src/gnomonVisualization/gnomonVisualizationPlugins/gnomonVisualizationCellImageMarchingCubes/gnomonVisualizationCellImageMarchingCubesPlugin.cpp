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

#include "gnomonVisualizationCellImageMarchingCubesPlugin.h"

#include "gnomonVisualizations/gnomonCellImage/gnomonVisualizationCellImageMarchingCubes.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationCellImageMarchingCubesPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellImageMarchingCubes";
    gnomonVisualization::visualizationCellImage::pluginFactory().record(plugin_name, gnomonVisualizationCellImageMarchingCubesCreator);

    if (gnomonVisualization::visualizationCellImage::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellImageMarchingCubesPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellImageMarchingCubes)


//
// gnomonVisualizationCellImageMarchingCubesPlugin.cpp ends here
