#pragma once

#include <gnomonVisualizationExport.h>

#include <QtCore>

#include <dtkCore/dtkCoreParameters>

#include "gnomonAbstractVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualization : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonAbstractMatplotlibVisualization();
    ~gnomonAbstractMatplotlibVisualization(void);

public:
    void setView(gnomonAbstractView *view) override;

public:
    int figureNumber(void);
};

//
// gnomonAbstractMatplotlibVisualization.h ends here
