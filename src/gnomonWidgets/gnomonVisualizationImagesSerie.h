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

#include <gnomonWidgetsExport.h>

#include "gnomonAbstractVisualization.h"

class gnomonViewForm;

class gnomonImagesSerie;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationImagesSerie : public gnomonAbstractVisualization
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
    QImage imageRendering(void);

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationImagesSeriePrivate *dd;
};


//
// gnomonVisualizationImagesSerie.h ends here