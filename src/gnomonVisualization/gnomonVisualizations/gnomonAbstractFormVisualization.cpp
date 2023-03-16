#include "gnomonAbstractFormVisualization.h"
#include "gnomonAbstractVisualization_p.h"
#include "gnomonAbstractFormVisualization_p.h"

#include "gnomonView/gnomonViewForm.h"
#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>

// /////////////////////////////////////////////////////////////////
// gnomonAbstractFormVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractFormVisualization::gnomonAbstractFormVisualization(void) : gnomonAbstractVisualization(), dd(new gnomonAbstractFormVisualizationPrivate)
{
}

gnomonAbstractFormVisualization::~gnomonAbstractFormVisualization(void)
{
    this->clearConnections();
    delete dd;
    dd = nullptr;
}

void gnomonAbstractFormVisualization::setView(gnomonAbstractView* view)
{
    if (auto view_form = dynamic_cast<gnomonViewForm *>(view)) {
        gnomonAbstractVisualization::setView(view_form);

        this->clearConnections();

        dd->connect3D = connect(view_form, SIGNAL(switchedTo3D(void)), this, SLOT(on3D(void)));
        dd->connect2D = connect(view_form, SIGNAL(switchedTo2D(void)), this, SLOT(on2D(void)));
        dd->connectXY = connect(view_form, SIGNAL(switchedTo2DXY(void)), this, SLOT(onXY(void)));
        dd->connectYZ = connect(view_form, SIGNAL(switchedTo2DYZ(void)), this, SLOT(onYZ(void)));
        dd->connectXZ = connect(view_form, SIGNAL(switchedTo2DXZ(void)), this, SLOT(onXZ(void)));

        dd->connectSliceOrientation = connect(view_form, SIGNAL(sliceOrientationChanged(int)), this,
                                              SLOT(onSliceOrientationChanged(int)));
        dd->connectSlice = connect(view_form, SIGNAL(sliceChanged(int)), this, SLOT(onSliceChanged(int)));

        dd->connectTime = connect(view_form, SIGNAL(timeChanged(double)), this, SLOT(onTimeChanged(double)));
    }
}

gnomonInteractorStyle * gnomonAbstractFormVisualization::interactorStyle(void)
{
    return nullptr;
}

void gnomonAbstractFormVisualization::clearConnections(void)
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

vtkRenderer *gnomonAbstractFormVisualization::offscreenRenderer(void)
{
    return dd->offscreenRenderer;
}

void gnomonAbstractFormVisualization::updateOffscreenRenderer(double xMin,double xMax,double yMin,double yMax,double zMin,double zMax)
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

QImage gnomonAbstractFormVisualization::offscreenImageRendering(void)
{
    dd->offscreenRenderWindow->Render();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(dd->offscreenRenderWindow);
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
// gnomonAbstractFormVisualization.cpp ends here
