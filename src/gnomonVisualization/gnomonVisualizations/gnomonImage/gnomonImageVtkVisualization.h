#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractImageVtkVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonImageVtkVisualization : public gnomonAbstractImageVtkVisualization
{
    Q_OBJECT

public:
     gnomonImageVtkVisualization(void);
    ~gnomonImageVtkVisualization(void);

public:
    virtual const QString pluginName(void) override;
    virtual const QString name(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> image) override;
    std::shared_ptr<gnomonImageSeries> image(void) override;

public slots:
    void updateOpacity(void);
    void updateChannelColorMap(void);

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
    class gnomonImageVtkVisualizationPrivate *ddd;
};

inline gnomonAbstractImageVtkVisualization *gnomonImageVtkVisualizationCreator(void)
{
    return new gnomonImageVtkVisualization();
}

//
// gnomonImageVtkVisualization.h ends here
