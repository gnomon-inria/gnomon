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

class gnomonCellComplex;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationCellComplex : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationCellComplex(gnomonViewForm *view);
    ~gnomonVisualizationCellComplex(void);

public:
	void setCellComplex(gnomonCellComplex *cellComplex);

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
    
public:
    QImage imageRendering(void);

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationCellComplexPrivate *dd;
};


//
// gnomonVisualizationCellComplex.h ends here