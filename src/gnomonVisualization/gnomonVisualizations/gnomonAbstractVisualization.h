#pragma once

#include <QtCore>

#include <dtkCore/dtkCoreParameters>
#include <gnomonVisualizationExport.h>
#include <dtkImagingCore>

class gnomonViewForm;
class gnomonInteractorStyle;

class vtkGenericOpenGLRenderWindow;
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
    virtual const QString pluginName(void) = 0;
    virtual const QString name(void) = 0;

public:
    virtual void setParameter(const QString&, const QVariant&) = 0;
    virtual void setParameters(const dtkCoreParameters&) = 0;
    virtual void connectParameter(const QString& parameter_name);
    virtual void refreshParameters(void);
    virtual void updateParameters(const QString& sender_name = "");

public:
    gnomonViewForm* view(void);
    virtual gnomonInteractorStyle * interactorStyle(void);
    virtual dtkCoreParameters parameters(void) const = 0;
    virtual QMap<QString, QString> parameterGroups(void) { return QMap<QString, QString>(); };

public:
    virtual QImage imageRendering(void) = 0;

public:

signals:
    void parametersChanged(void);

public slots:
    virtual void update(void) = 0;
    virtual void render(void) = 0;

public slots:
    void clearConnections(void);
    virtual void clear(void) = 0;
    virtual void setVisible(bool visible) = 0;

public slots:
    virtual void on2D(void) = 0;
    virtual void on3D(void) = 0;
    virtual void onXY(void) = 0;
    virtual void onXZ(void) = 0;
    virtual void onYZ(void) = 0;
    virtual void onSliceChanged(int) = 0;
    virtual void onSliceOrientationChanged(int) = 0;
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
