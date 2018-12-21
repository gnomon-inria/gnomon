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

class GNOMONWIDGETS_EXPORT gnomonVisualizationImagesSerieChannelBlending : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationImagesSerieChannelBlending(gnomonViewForm *view);
    ~gnomonVisualizationImagesSerieChannelBlending(void);

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
	class gnomonVisualizationImagesSerieChannelBlendingPrivate *dd;
};


//
// gnomonVisualizationImagesSerieChannelBlending.h ends here