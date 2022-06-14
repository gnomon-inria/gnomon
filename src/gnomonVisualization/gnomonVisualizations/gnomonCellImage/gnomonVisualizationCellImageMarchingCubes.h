#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractVisualizationCellImage.h"

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationCellImageMarchingCubes : public gnomonAbstractVisualizationCellImage
{
    Q_OBJECT

public:
     gnomonVisualizationCellImageMarchingCubes(void);
    ~gnomonVisualizationCellImageMarchingCubes(void);

public:
    virtual const QString pluginName(void) override;

public:
	void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) override;
	std::shared_ptr<gnomonCellImageSeries> cellImage(void) override;

public:
    gnomonInteractorStyle *interactorStyle(void) override;

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

public:
    long cellId(long vtkId) ; //override;
    QVariantMap cellInfo(long cellId);

public:
	class gnomonVisualizationCellImageMarchingCubesPrivate *dd;
};

inline gnomonAbstractVisualizationCellImage *gnomonVisualizationCellImageMarchingCubesCreator(void)
{
    return new gnomonVisualizationCellImageMarchingCubes();
}


//
// gnomonVisualizationCellImageMarchingCubes.h ends here
