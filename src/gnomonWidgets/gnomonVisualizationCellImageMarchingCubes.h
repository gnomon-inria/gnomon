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
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

private:
	class gnomonVisualizationCellImageMarchingCubesPrivate *dd;
};


//
// gnomonVisualizationCellImageMarchingCubes.h ends here