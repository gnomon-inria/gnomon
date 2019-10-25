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

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationCellImageVolume : public gnomonAbstractVisualizationCellImage
{
    Q_OBJECT

public:
     gnomonVisualizationCellImageVolume(void);
    ~gnomonVisualizationCellImageVolume(void);

public:
	void setCellImage(gnomonCellImageSeries *cellImage) override;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void) override;

public slots:
    void on2D(void) override;
    void on3D(void) override;
    void onXY(void) override;
    void onXZ(void) override;
    void onYZ(void) override;
    void onSliceChanged(int) override;
    void onSliceOrientationChanged(int) override;
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;


private:
	class gnomonVisualizationCellImageVolumePrivate *dd;
};


inline gnomonAbstractVisualizationCellImage *gnomonVisualizationCellImageVolumeCreator(void)
{
    return new gnomonVisualizationCellImageVolume();
}


//
// gnomonVisualizationCellImage.h ends here