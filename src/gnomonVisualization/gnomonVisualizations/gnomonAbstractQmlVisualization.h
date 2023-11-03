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
    virtual ~gnomonAbstractQmlVisualization(void);

public:
    gnomonQmlView *qmlView(void);

public:
    QImage imageRendering(void) override;
};

//
// gnomonAbstractQmlVisualization.h ends here
