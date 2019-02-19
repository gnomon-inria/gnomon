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

class gnomonCellImage;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationCellImageMarchingCubes : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationCellImageMarchingCubes(void);
    ~gnomonVisualizationCellImageMarchingCubes(void);

public:
	void setCellImage(gnomonCellImage *cellImage);

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
    
public:
    QImage imageRendering(void);

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationCellImageMarchingCubesPrivate *dd;
};


//
// gnomonVisualizationCellImageMarchingCubes.h ends here