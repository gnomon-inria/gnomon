#pragma once

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractMeshVtkVisualization.h"

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONVISUALIZATION_EXPORT gnomonMeshVtkVisualization : public gnomonAbstractMeshVtkVisualization
{
    Q_OBJECT

public:
     gnomonMeshVtkVisualization(void);
    ~gnomonMeshVtkVisualization(void);

public:
    virtual const QString pluginName(void) override;
    virtual const QString name(void) override;
    virtual QString documentation(void) override;

public:
    void setMesh(std::shared_ptr<gnomonMeshSeries> mesh) override;
    std::shared_ptr<gnomonMeshSeries> mesh(void) override;
    void setView(gnomonAbstractView* view) override;

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void) override;
    void fill(void) override;
    virtual void setVisible(bool visible) override;

public slots:
    void on2D(void) override;
    void on3D(void) override;
    void onXY(void) override;
    void onXZ(void) override;
    void onYZ(void) override;
    void onSliceChanged(double) override;
    void onSliceOrientationChanged(int) override;
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const dtkCoreParameters&) override;
    void refreshParameters(void) override;
    dtkCoreParameters parameters(void) const override;
    QMap<QString, QString> parameterGroups(void) override;

private:
    class gnomonMeshVtkVisualizationPrivate *ddd;
};

inline gnomonAbstractMeshVtkVisualization *gnomonMeshVtkVisualizationCreator(void)
{
    return new gnomonMeshVtkVisualization();
}

//
// gnomonMeshVtkVisualization.h ends here
