// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonImagesSerie;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationImagesSerie : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationImagesSerie(void);
    ~gnomonVisualizationImagesSerie(void);

public:
	void setImagesSerie(gnomonImagesSerie *imagesSerie);

public slots:
    void updateOpacity(void);
    void updateChannelColorMap(void);

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

private:
	class gnomonVisualizationImagesSeriePrivate *dd;
};


//
// gnomonVisualizationImagesSerie.h ends here