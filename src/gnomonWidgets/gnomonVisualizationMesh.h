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

class gnomonMesh;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationMesh : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonVisualizationMesh(gnomonViewForm *view);
    ~gnomonVisualizationMesh(void);

public:
	void setMesh(gnomonMesh *mesh);

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
    
public:
    QImage imageRendering(void);

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationMeshPrivate *dd;
};


//
// gnomonVisualizationMesh.h ends here