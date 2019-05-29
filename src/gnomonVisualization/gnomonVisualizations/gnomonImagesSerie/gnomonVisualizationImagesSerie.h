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

#include "gnomonAbstractVisualizationImagesSerie.h"

class gnomonViewForm;

class gnomonImagesSerie;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationImagesSerie : public gnomonAbstractVisualizationImagesSerie
{
    Q_OBJECT

public:
     gnomonVisualizationImagesSerie(void);
    ~gnomonVisualizationImagesSerie(void);

public:
	void setImagesSerie(gnomonImagesSerie *imagesSerie) override;

public slots:
    void updateOpacity(void);
    void updateChannelColorMap(void);

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void) override;

public slots:
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

private:
	class gnomonVisualizationImagesSeriePrivate *dd;
};

inline gnomonAbstractVisualizationImagesSerie *gnomonVisualizationImagesSerieCreator(void)
{
    return new gnomonVisualizationImagesSerie();
}

//
// gnomonVisualizationImagesSerie.h ends here