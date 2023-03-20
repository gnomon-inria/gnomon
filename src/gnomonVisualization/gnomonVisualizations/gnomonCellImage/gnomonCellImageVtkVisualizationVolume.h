#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractCellImageVtkVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonCellImageVtkVisualizationVolume : public gnomonAbstractCellImageVtkVisualization
{
    Q_OBJECT

public:
     gnomonCellImageVtkVisualizationVolume(void);
    ~gnomonCellImageVtkVisualizationVolume(void);

public:
    virtual const QString pluginName(void) override;
    virtual const QString name(void) override;

public:
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) override;
    std::shared_ptr<gnomonCellImageSeries> cellImage(void) override;

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
    virtual void setVisible(bool visible) override;

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
    QMap<QString, QString> parameterGroups(void) override;

private:
    class gnomonCellImageVtkVisualizationVolumePrivate *ddd;
};


inline gnomonAbstractCellImageVtkVisualization *gnomonCellImageVtkVisualizationVolumeCreator(void)
{
    return new gnomonCellImageVtkVisualizationVolume();
}


//
// gnomonCellImageVtkVisualization.h ends here
