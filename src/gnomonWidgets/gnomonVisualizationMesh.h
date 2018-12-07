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

class gnomonViewForm;

class gnomonMesh;
class gnomonCoreParameter;

class GNOMONWIDGETS_EXPORT gnomonVisualizationMesh : public QObject
{
    Q_OBJECT

public:
     gnomonVisualizationMesh(gnomonViewForm *view);
    ~gnomonVisualizationMesh(void);

public:
	void setMesh(gnomonMesh *mesh);
    void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public slots:
    void update(void);
    void render(void);

private:
	class gnomonVisualizationMeshPrivate *d;
};


//
// gnomonVisualizationMesh.h ends here