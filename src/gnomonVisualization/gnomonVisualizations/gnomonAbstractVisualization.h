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

class gnomonViewForm;
class gnomonCoreParameter;
class vtkRenderer;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualization : public QObject
{
    Q_OBJECT

public:
     gnomonAbstractVisualization();
    ~gnomonAbstractVisualization(void);

public:
    void setView(gnomonViewForm *view);

public:
    virtual void setParameter(const QString&, const QVariant&) = 0;
    virtual void setParameters(const QMap<QString, gnomonCoreParameter *>&) = 0;

public:
    gnomonViewForm* view(void);
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const = 0;

public:
    virtual QImage imageRendering(void) = 0;

signals:
    void parametersChanged(void);

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;

public slots:
    void clearConnections(void);
    virtual void clear(void) = 0;

public slots:
    virtual void on2D(void) = 0;
    virtual void on3D(void) = 0;
    virtual void onXY(void) = 0;
    virtual void onXZ(void) = 0;
    virtual void onYZ(void) = 0;
    virtual void onSliceChanged(double) = 0;
    virtual void onSliceOrientationChanged(double) = 0;
    virtual void onTimeChanged(double) = 0;

public:
    vtkRenderer *offscreenRenderer(void);

public slots:
    void updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax);
    QImage offscreenImageRendering(void);

protected:
    class gnomonAbstractVisualizationPrivate *d;
};

//
// gnomonAbstractVisualization.h ends here
