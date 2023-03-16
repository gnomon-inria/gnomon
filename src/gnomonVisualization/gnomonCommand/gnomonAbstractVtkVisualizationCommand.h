#pragma once

#include <gnomonVisualizationExport>

#include "gnomonAbstractVisualizationCommand.h"

class gnomonAbstractView;
class gnomonAbstractFormVisualization;

class GNOMONCORE_EXPORT gnomonAbstractVtkVisualizationCommand :  public gnomonAbstractVisualizationCommand
{

public:
    void setView(gnomonAbstractView *view) override;
};


//
// gnomonAbstractMplVisualizationCommand.h ends here
