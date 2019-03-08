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

#include "gnomonVisualizationCellComplexPlugin.h"

#include "gnomonVisualizations/gnomonCellComplex/gnomonVisualizationCellComplex.h"

#include <dtkCore>
#include <gnomonVisualization>

void gnomonVisualizationCellComplexPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellComplex";
    gnomonVisualization::visualizationCellComplex::pluginFactory().record(plugin_name, gnomonVisualizationCellComplexCreator);

    if (gnomonVisualization::visualizationCellComplex::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellComplexPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellComplex)


//
// gnomonVisualizationCellComplexPlugin.cpp ends here
