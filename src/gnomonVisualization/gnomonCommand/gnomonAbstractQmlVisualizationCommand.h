#pragma once

#include <gnomonVisualizationExport>

#include "gnomonAbstractVisualizationCommand.h"

class gnomonAbstractView;
class gnomonAbstractQmlVisualization;

class GNOMONCORE_EXPORT gnomonAbstractQmlVisualizationCommand :  public gnomonAbstractVisualizationCommand
{

public:
    void setView(gnomonAbstractView *view) override;
};


//
// gnomonAbstractQmlVisualizationCommand.h ends here
