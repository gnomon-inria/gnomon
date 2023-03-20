#pragma once

#include <gnomonVisualizationExport>

#include "gnomonAbstractVisualizationCommand.h"

class gnomonAbstractView;
class gnomonAbstractMplVisualization;

class GNOMONCORE_EXPORT gnomonAbstractMplVisualizationCommand :  public gnomonAbstractVisualizationCommand
{

public:
    void setView(gnomonAbstractView *view) override;
};


//
// gnomonAbstractMplVisualizationCommand.h ends here
