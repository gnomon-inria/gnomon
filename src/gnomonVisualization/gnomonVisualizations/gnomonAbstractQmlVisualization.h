#pragma once

#include <gnomonVisualizationExport.h>

#include <QtCore>

#include <dtkCore/dtkCoreParameters>

#include "gnomonAbstractVisualization.h"

class gnomonQmlView;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractQmlVisualization : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractQmlVisualization();
    ~gnomonAbstractQmlVisualization(void);

public:
    gnomonQmlView *qmlView(void);
};

//
// gnomonAbstractQmlVisualization.h ends here
