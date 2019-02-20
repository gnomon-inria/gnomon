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

#include "gnomonVisualizationCellComplex.h"
#include "gnomonVisualizationCellComplexPlugin.h"

#include <dtkCore>
#include <gnomonWidgets>

void gnomonVisualizationCellComplexPlugin::initialize(void)
{
    QString plugin_name = "gnomonVisualizationCellComplex";
    gnomonWidgets::visualizationCellComplex::pluginFactory().record(plugin_name, gnomonVisualizationCellComplexCreator);

    if (gnomonWidgets::visualizationCellComplex::pluginFactory().keys().contains(plugin_name)) {
        qDebug()<<"C++ plugin"<<plugin_name<<"has been successfully loaded!";
    }

}

void gnomonVisualizationCellComplexPlugin::uninitialize(void)
{

}


DTK_DEFINE_PLUGIN(gnomonVisualizationCellComplex)


//
// gnomonVisualizationCellComplexPlugin.cpp ends here
