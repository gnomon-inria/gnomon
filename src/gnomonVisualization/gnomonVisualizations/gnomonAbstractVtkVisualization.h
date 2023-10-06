#pragma once

#include <gnomonVisualizationExport.h>

#include <QtCore>

#include <dtkCore/dtkCoreParameters>

#include "gnomonAbstractVisualization.h"
#include <gnomonVisualization/gnomonView/gnomonAbstractView.h>

class gnomonVtkView;
class gnomonInteractorStyle;

class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVtkVisualization : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
     gnomonAbstractVtkVisualization();
    virtual ~gnomonAbstractVtkVisualization(void);

public:
    void setView(gnomonAbstractView* view) override;
    gnomonVtkView *vtkView(void);

public:
    virtual gnomonInteractorStyle * interactorStyle(void);

public slots:
    void clearConnections(void) override;

public slots:
    virtual void on2D(void) = 0;
    virtual void on3D(void) = 0;
    virtual void onXY(void) = 0;
    virtual void onXZ(void) = 0;
    virtual void onYZ(void) = 0;
    virtual void onSliceChanged(double) = 0;
    virtual void onSliceOrientationChanged(int) = 0;
    virtual void onTimeChanged(double) = 0;

public:
    vtkRenderer *offscreenRenderer(void);

public slots:
    void updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax);
    QImage offscreenImageRendering(void);

protected:
    class gnomonAbstractVtkVisualizationPrivate *dd;
};

//
// gnomonAbstractVtkVisualization.h ends here
