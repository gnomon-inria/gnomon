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

#include "gnomonAbstractVisualizationMesh.h"

class gnomonViewForm;

class gnomonMesh;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationMesh : public gnomonAbstractVisualizationMesh
{
    Q_OBJECT

public:
     gnomonVisualizationMesh(void);
    ~gnomonVisualizationMesh(void);

public:
	void setMesh(gnomonMesh *mesh) override;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
    
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

private:
	class gnomonVisualizationMeshPrivate *dd;
};

inline gnomonAbstractVisualizationMesh *gnomonVisualizationMeshCreator(void)
{
    return new gnomonVisualizationMesh();
}

//
// gnomonVisualizationMesh.h ends here