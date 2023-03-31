#pragma once

#include <gnomonVisualizationExport>

#include "gnomonAbstractVisualizationCommand.h"

class gnomonAbstractView;
class gnomonAbstractVtkVisualization;

class GNOMONCORE_EXPORT gnomonAbstractVtkVisualizationCommand :  public gnomonAbstractVisualizationCommand
{

public:
    void setView(gnomonAbstractView *view) override;

public:
    void clear(void) override;
};


//
// gnomonAbstractMplVisualizationCommand.h ends here
