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

#include "gnomonViewVolumic.h"

class gnomonMesh;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationMesh : public QObject
{
    Q_OBJECT

public:
     gnomonVisualizationMesh(gnomonViewVolumic *view);
    ~gnomonVisualizationMesh(void);

public:
	void setMesh(gnomonMesh *mesh);
    void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public slots:
    void render(void);

private:
	class gnomonVisualizationMeshPrivate *d;
};


//
// gnomonVisualizationMesh.h ends here