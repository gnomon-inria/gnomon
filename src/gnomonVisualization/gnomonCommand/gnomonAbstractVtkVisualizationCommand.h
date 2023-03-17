#pragma once

#include <gnomonVisualizationExport>

#include "gnomonAbstractVisualizationCommand.h"

class gnomonAbstractView;
class gnomonAbstractFormVisualization;

class GNOMONCORE_EXPORT gnomonAbstractVtkVisualizationCommand :  public gnomonAbstractVisualizationCommand
{

public:
    void setAlgorithmName(const QString &) override;

    void setView(gnomonAbstractView *view) override;
    void setVisualizationParameters(const QVariantMap &parameters) override;
};


//
// gnomonAbstractMplVisualizationCommand.h ends here
