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

class GNOMONWIDGETS_EXPORT gnomonVisualizationCellImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationCellImage(void);
    ~gnomonVisualizationCellImage(void);

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
	class gnomonVisualizationCellImagePrivate *dd;
};


//
// gnomonVisualizationCellImage.h ends here