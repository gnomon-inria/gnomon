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

#include "gnomonAbstractVisualizationCellImage.h"

class gnomonViewForm;

class gnomonCellImage;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationCellImageMarchingCubes : public gnomonAbstractVisualizationCellImage
{
    Q_OBJECT

public:
     gnomonVisualizationCellImageMarchingCubes(void);
    ~gnomonVisualizationCellImageMarchingCubes(void);

public:
	void setCellImage(gnomonCellImageSeries *cellImage) override;
    
public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;
    void clear(void) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    long cellId(long vtkId);
    QMap<QString, QVariant> cellInfo(long cellId);

public:
	class gnomonVisualizationCellImageMarchingCubesPrivate *dd;
};

inline gnomonAbstractVisualizationCellImage *gnomonVisualizationCellImageMarchingCubesCreator(void)
{
    return new gnomonVisualizationCellImageMarchingCubes();
}


//
// gnomonVisualizationCellImageMarchingCubes.h ends here