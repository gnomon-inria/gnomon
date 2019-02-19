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

#include "gnomonAbstractVisualizationCellComplex.h"

class gnomonViewForm;

class gnomonCellComplex;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationCellComplex : public gnomonAbstractVisualizationCellComplex
{
    Q_OBJECT

public:
     gnomonVisualizationCellComplex(gnomonViewForm *view);
    ~gnomonVisualizationCellComplex(void);

public:
	void setCellComplex(gnomonCellComplex *cellComplex) override;

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

private:
	class gnomonVisualizationCellComplexPrivate *dd;
};


//
// gnomonVisualizationCellComplex.h ends here