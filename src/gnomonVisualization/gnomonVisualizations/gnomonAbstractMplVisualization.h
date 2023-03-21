#pragma once

#include <gnomonVisualizationExport.h>

#include <QtCore>

#include <dtkCore/dtkCoreParameters>

#include "gnomonAbstractVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMplVisualization : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonAbstractMplVisualization();
    ~gnomonAbstractMplVisualization(void);

public:
    int figureNumber(void);
};

//
// gnomonAbstractMplVisualization.h ends here
