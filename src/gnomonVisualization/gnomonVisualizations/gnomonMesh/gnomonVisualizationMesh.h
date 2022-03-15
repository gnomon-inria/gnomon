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

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractVisualizationMesh.h"

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class gnomonViewForm;
class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationMesh : public gnomonAbstractVisualizationMesh
{
    Q_OBJECT

public:
     gnomonVisualizationMesh(void);
    ~gnomonVisualizationMesh(void);

public:
	void setMesh(gnomonMeshSeries *mesh) override;
	gnomonMeshSeries *mesh(void) override;

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
	class gnomonVisualizationMeshPrivate *dd;
};

inline gnomonAbstractVisualizationMesh *gnomonVisualizationMeshCreator(void)
{
    return new gnomonVisualizationMesh();
}

//
// gnomonVisualizationMesh.h ends here
