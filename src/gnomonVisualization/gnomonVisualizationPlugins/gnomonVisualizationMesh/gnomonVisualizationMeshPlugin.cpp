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

#include "gnomonVisualizationMeshPlugin.h"

#include "gnomonVisualizations/gnomonMesh/gnomonVisualizationMesh.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationMeshPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationMesh";
    gnomonVisualization::visualizationMesh::pluginFactory().record(plugin_name, gnomonVisualizationMeshCreator);

    if (gnomonVisualization::visualizationMesh::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationMeshPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationMesh)


//
// gnomonVisualizationMeshPlugin.cpp ends here
