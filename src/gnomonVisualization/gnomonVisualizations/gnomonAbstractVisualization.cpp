#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include "gnomonView/gnomonViewForm.h"
#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

#include <xVis/xVisViewer.hpp>

// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVisualization::gnomonAbstractVisualization(void) : d(new gnomonAbstractVisualizationPrivate)
{
}

gnomonAbstractVisualization::~gnomonAbstractVisualization(void)
{
    delete d;

    d = NULL;
}

// dtkCoreParameters gnomonAbstractVisualization::parameters(void) const
// {
//     return d->parameters;
// }
//
// void gnomonAbstractVisualization::setParameter(const QString& parameter, const QVariant& value)
// {
//     if (d->parameters.contains(parameter)) {
//         d->parameters[parameter]->setValue(value);
//     }
//     else
//         qWarning()<<parameter<<"is not a valid parameter!";
// }
//
// void gnomonAbstractVisualization::setParameters(const dtkCoreParameters& parameters)
// {
//     d->parameters = parameters;
//     for (const auto& param : parameters.keys()) {
//         if (d->parameters.contains(param)) {
//             d->parameters[param] = parameters[param];
//             d->parameters[param]->copy(parameters[param]);
//         }
//     }
// }

void gnomonAbstractVisualization::setView(gnomonViewForm* view)
{
    d->view = view;

    this->clearConnections();

    d->connect3D = connect(d->view, SIGNAL(switchedTo3D(void)), this , SLOT(on3D(void)));
    d->connect2D = connect(d->view, SIGNAL(switchedTo2D(void)), this , SLOT(on2D(void)));
    d->connectXY = connect(d->view, SIGNAL(switchedTo2DXY(void)), this , SLOT(onXY(void)));
    d->connectYZ= connect(d->view, SIGNAL(switchedTo2DYZ(void)), this , SLOT(onYZ(void)));
    d->connectXZ = connect(d->view, SIGNAL(switchedTo2DXZ(void)), this , SLOT(onXZ(void)));

    d->connectSliceOrientation = connect(d->view, SIGNAL(sliceOrientationChanged(int)), this, SLOT(onSliceOrientationChanged(int)));
    d->connectSlice = connect(d->view, SIGNAL(sliceChanged(int)), this, SLOT(onSliceChanged(int)));

    d->connectTime = connect(d->view, SIGNAL(timeChanged(double)), this, SLOT(onTimeChanged(double)));
}

gnomonViewForm* gnomonAbstractVisualization::view(void)
{
    return d->view;
}

gnomonInteractorStyle * gnomonAbstractVisualization::interactorStyle(void)
{
    return nullptr;
}

void gnomonAbstractVisualization::clearConnections(void)
{
    disconnect(d->connect3D);
    disconnect(d->connect2D);

    disconnect(d->connectXY);
    disconnect(d->connectYZ);
    disconnect(d->connectXZ);

    disconnect(d->connectSliceOrientation);
    disconnect(d->connectSlice);
    disconnect(d->connectTime);
}

vtkRenderer *gnomonAbstractVisualization::offscreenRenderer(void)
{
    return d->offscreenRenderer;
}

void gnomonAbstractVisualization::setOffscreenRenderWindow(xVisViewer *viewer)
{
    d->offscreenRenderWindow = viewer;
}

void gnomonAbstractVisualization::updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax)
{
    if(!d->offscreenRenderer) {
        d->offscreenRenderer = vtkSmartPointer<vtkRenderer>::New();
    }

    d->offscreenRenderer->DrawOn();
    d->offscreenRenderer->InteractiveOn();
    d->offscreenRenderWindow->GetRenderWindow()->AddRenderer(d->offscreenRenderer);

    vtkSmartPointer<vtkCamera> cam = d->offscreenRenderer->GetActiveCamera();
    cam->ParallelProjectionOn();
    cam->SetParallelScale(1);
    cam->SetFocalPoint((xMin+xMax)/2,(yMin+yMax)/2,(zMin+zMax)/2);
    cam->SetPosition((xMin+xMax)/2,(yMin+yMax)/2,zMin);
    cam->SetViewUp(0,1,0);

    double focus = 0.8;
    double xMinFocus = (focus)*xMin+(1.-focus)*xMax;
    double xMaxFocus = (1.-focus)*xMin+(focus)*xMax;
    double yMinFocus = (focus)*yMin+(1.-focus)*yMax;
    double yMaxFocus = (1.-focus)*yMin+(focus)*yMax;

    d->offscreenRenderer->ResetCamera(xMinFocus,xMaxFocus,yMinFocus,yMaxFocus,zMin,zMax);
}

QImage gnomonAbstractVisualization::offscreenImageRendering(void)
{
    return QImage();
}

void gnomonAbstractVisualization::cleanup(void)
{
    d->offscreenRenderWindow->GetRenderWindow()->RemoveRenderer(d->offscreenRenderer);
}

//
// gnomonAbstractVisualization.cpp ends here
