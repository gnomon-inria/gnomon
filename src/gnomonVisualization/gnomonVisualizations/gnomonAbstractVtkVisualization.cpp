#include "gnomonAbstractVtkVisualization.h"
#include "gnomonAbstractVisualization_p.h"
#include "gnomonAbstractVtkVisualization_p.h"

#include "gnomonView/gnomonVtkView.h"
#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>

// /////////////////////////////////////////////////////////////////
// gnomonAbstractVtkVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVtkVisualization::gnomonAbstractVtkVisualization(void) : gnomonAbstractVisualization(), dd(new gnomonAbstractVtkVisualizationPrivate)
{
}

gnomonAbstractVtkVisualization::~gnomonAbstractVtkVisualization(void)
{
    this->clearConnections();
    delete dd;
    dd = nullptr;
}

void gnomonAbstractVtkVisualization::setView(gnomonAbstractView* view)
{
    gnomonAbstractVisualization::setView(view);
    if (auto vtk_view = dynamic_cast<gnomonVtkView *>(view)) {

        this->clearConnections();

        dd->connect3D = connect(vtk_view, SIGNAL(switchedTo3D(void)), this, SLOT(on3D(void)));
        dd->connect2D = connect(vtk_view, SIGNAL(switchedTo2D(void)), this, SLOT(on2D(void)));
        dd->connectXY = connect(vtk_view, SIGNAL(switchedTo2DXY(void)), this, SLOT(onXY(void)));
        dd->connectYZ = connect(vtk_view, SIGNAL(switchedTo2DYZ(void)), this, SLOT(onYZ(void)));
        dd->connectXZ = connect(vtk_view, SIGNAL(switchedTo2DXZ(void)), this, SLOT(onXZ(void)));

        dd->connectSliceOrientation = connect(vtk_view, SIGNAL(sliceOrientationChanged(int)), this,
                                              SLOT(onSliceOrientationChanged(int)));
        dd->connectSlice = connect(vtk_view, SIGNAL(sliceChanged(int)), this, SLOT(onSliceChanged(int)));

        dd->connectTime = connect(vtk_view, SIGNAL(timeChanged(double)), this, SLOT(onTimeChanged(double)));
    }
}

gnomonVtkView *gnomonAbstractVtkVisualization::vtkView(void)
{
    if (auto vtk_view = dynamic_cast<gnomonVtkView *>(d->view)) {
        return vtk_view;
    } else {
        return nullptr;
    }
}

gnomonInteractorStyle * gnomonAbstractVtkVisualization::interactorStyle(void)
{
    return nullptr;
}

void gnomonAbstractVtkVisualization::clearConnections(void)
{
    disconnect(dd->connect3D);
    disconnect(dd->connect2D);

    disconnect(dd->connectXY);
    disconnect(dd->connectYZ);
    disconnect(dd->connectXZ);

    disconnect(dd->connectSliceOrientation);
    disconnect(dd->connectSlice);
    disconnect(dd->connectTime);
}

vtkRenderer *gnomonAbstractVtkVisualization::offscreenRenderer(void)
{
    return dd->offscreenRenderer;
}

void gnomonAbstractVtkVisualization::updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax)
{
    if(!dd->offscreenRenderer)
        dd->offscreenRenderer = vtkSmartPointer<vtkRenderer>::New();
    dd->offscreenRenderer->SetBackground(0,0,0);

    if(!dd->offscreenRenderWindow) {
        dd->offscreenRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        dd->offscreenRenderWindow->AddRenderer(dd->offscreenRenderer);
    }
    dd->offscreenRenderWindow->SetOffScreenRendering(1);
    dd->offscreenRenderWindow->SetSize(1500, 1500);

    vtkSmartPointer<vtkCamera> cam = dd->offscreenRenderer->GetActiveCamera();
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

    dd->offscreenRenderer->ResetCamera(xMinFocus,xMaxFocus,yMinFocus,yMaxFocus,zMin,zMax);
}

QImage gnomonAbstractVtkVisualization::offscreenImageRendering(void)
{
    dd->offscreenRenderWindow->Render();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(dd->offscreenRenderWindow);
    windowToImageFilter->SetInputBufferTypeToRGBA();
    // windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkSmartPointer<vtkImageData> renderedImage = windowToImageFilter->GetOutput();
    int height = renderedImage->GetDimensions()[0];
    int width = renderedImage->GetDimensions()[1];
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
// gnomonAbstractVtkVisualization.cpp ends here
