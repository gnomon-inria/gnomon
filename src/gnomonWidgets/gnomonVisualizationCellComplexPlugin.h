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

#pragma once

#include <QtCore>

#include <dtkCore>
#include <gnomonAbstractVisualizationCellComplex>

class gnomonVisualizationCellComplexPlugin: public gnomonAbstractVisualizationCellComplexPlugin
{
    Q_OBJECT
    Q_INTERFACES(gnomonAbstractVisualizationCellComplexPlugin)
    Q_PLUGIN_METADATA(IID "fr.inria.gnomonVisualizationCellComplexPlugin" FILE "gnomonVisualizationCellComplexPlugin.json")

public:
     gnomonVisualizationCellComplexPlugin(void) {}
    ~gnomonVisualizationCellComplexPlugin(void) {}

public:
    void initialize(void);
    void uninitialize(void);
};

//
// gnomonVisualizationCellComplexPlugin.h ends here
