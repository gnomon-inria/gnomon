#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include "gnomonView/gnomonViewForm.h"
#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

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

//dtkCoreParameters gnomonAbstractVisualization::parameters(void) const
//{
//    return d->parameters;
//}
//
//void gnomonAbstractVisualization::setParameter(const QString& parameter, const QVariant& value)
//{
//    if (d->parameters.contains(parameter)) {
//        d->parameters[parameter]->setValue(value);
//    }
//    else
//        qWarning()<<parameter<<"is not a valid parameter!";
//}
//
//void gnomonAbstractVisualization::setParameters(const dtkCoreParameters& parameters)
//{
////    d->parameters = parameters;
//    for (const auto& param : parameters.keys()) {
//        if (d->parameters.contains(param)) {
////            d->parameters[param] = parameters[param];
//            d->parameters[param]->copy(parameters[param]);
//        }
//    }
//}

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

void gnomonAbstractVisualization::updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax)
{
    if(!d->offscreenRenderer) {
        d->offscreenRenderer = vtkSmartPointer<vtkRenderer>::New();
    }

    if(!d->offscreenRenderWindow) {
        d->offscreenRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    }
    d->offscreenRenderWindow->AddRenderer(d->offscreenRenderer);
    d->offscreenRenderWindow->SetOffScreenRendering(1);
    d->offscreenRenderWindow->SetSize(1500, 1500);

    d->offscreenRenderer->SetBackground(0,0,0);

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
    d->offscreenRenderWindow->Render();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(d->offscreenRenderWindow);
    windowToImageFilter->SetInputBufferTypeToRGBA();
    // windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkSmartPointer<vtkImageData> renderedImage = windowToImageFilter->GetOutput();
    int width = renderedImage->GetDimensions()[0];
    int height = renderedImage->GetDimensions()[1];
    QImage image( width, height, QImage::Format_RGB32);

    QRgb *rgbPtr = reinterpret_cast<QRgb *>(image.bits());
    for(int col = 0; col < width; ++col) {
        for(int row = 0; row < height; ++row) {
            double r, g, b;
            r = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row, width-col-1, 0))[0];
            g = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row, width-col-1, 0))[1];
            b = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row, width-col-1, 0))[2];
            *(rgbPtr) = QColor(r,g,b).rgb();
            ++rgbPtr;
        }
    }

    return image;
}

//
// gnomonAbstractVisualization.cpp ends here
