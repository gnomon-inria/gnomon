
#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractVisualizationBinaryImage.h"

class gnomonViewForm;

class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationBinaryImage : public gnomonAbstractVisualizationBinaryImage
{
    Q_OBJECT

public:
    gnomonVisualizationBinaryImage(void);
    ~gnomonVisualizationBinaryImage(void);

public:
    void setImage(gnomonBinaryImageSeries *image) override;
    gnomonBinaryImageSeries *image(void) override;

public slots:
    void updateOpacity(void);

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
    void setParameters(const dtkCoreParameters&) override;
    dtkCoreParameters parameters(void) const override;

private:
    class gnomonVisualizationBinaryImagePrivate *dd;
};

inline gnomonAbstractVisualizationBinaryImage *gnomonVisualizationBinaryImageCreator(void)
{
    return new gnomonVisualizationBinaryImage();
}

//
// gnomonVisualizationImage.h ends here
