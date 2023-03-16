#pragma once

#include <gnomonVisualizationExport.h>

#include <QtCore>

#include <dtkCore/dtkCoreParameters>

#include "gnomonAbstractVisualization.h"
#include <gnomonVisualization/gnomonView/gnomonAbstractView.h>

class gnomonInteractorStyle;

class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractFormVisualization : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonAbstractFormVisualization();
    ~gnomonAbstractFormVisualization(void);

public:
    void setView(gnomonAbstractView* view) override;
public:
    virtual gnomonInteractorStyle * interactorStyle(void);

public slots:
    void clearConnections(void);

public slots:
    virtual void on2D(void) = 0;
    virtual void on3D(void) = 0;
    virtual void onXY(void) = 0;
    virtual void onXZ(void) = 0;
    virtual void onYZ(void) = 0;
    virtual void onSliceChanged(int) = 0;
    virtual void onSliceOrientationChanged(int) = 0;
    virtual void onTimeChanged(double) = 0;

// Convenience accessors for Python plugins
public:
    vtkRenderer *renderer3D(void);
    vtkRenderer *renderer2D(void);
    vtkRenderer *offscreenRenderer(void);

    vtkRenderWindowInteractor *interactor(void);

public:
    void setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
    double currentTime(void) const;

public slots:
    void updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax);
    QImage offscreenImageRendering(void);

protected:
    class gnomonAbstractFormVisualizationPrivate *dd;
};

//
// gnomonAbstractFormVisualization.h ends here
