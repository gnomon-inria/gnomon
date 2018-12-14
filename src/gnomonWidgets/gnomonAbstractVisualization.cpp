// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewForm.h"

#include "gnomonPolyDataMesh.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualizationPrivate
// /////////////////////////////////////////////////////////////////

void gnomonAbstractVisualizationPrivate::updateOffscreenRenderer(double bounds[6])
{
    if(!this->offscreenRenderer) {
        this->offscreenRenderer = vtkSmartPointer<vtkRenderer>::New();
    }

    if(!this->offscreenRenderWindow) {
        this->offscreenRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    }
    this->offscreenRenderWindow->AddRenderer(this->offscreenRenderer);
    this->offscreenRenderWindow->SetOffScreenRendering(1);
    this->offscreenRenderWindow->SetSize(300, 300);

    if(!this->offscreenRenderWindowInteractor) {
        this->offscreenRenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    }
    this->offscreenRenderWindowInteractor->SetRenderWindow(this->offscreenRenderWindow);
    
    this->offscreenRenderer->SetBackground(0,0,0);
    
    vtkSmartPointer<vtkCamera> cam = this->offscreenRenderer->GetActiveCamera();
    cam->ParallelProjectionOn();
    cam->SetParallelScale(1);
    cam->SetFocalPoint((bounds[0]+bounds[1])/2,(bounds[2]+bounds[3])/2,(bounds[4]+bounds[5])/2);
    cam->SetPosition((bounds[0]+bounds[1])/2,(bounds[2]+bounds[3])/2,bounds[4]);
    cam->SetViewUp(0,1,0);
    
    double focus = 0.8;
    double xMin = (focus)*bounds[0]+(1.-focus)*bounds[1];
    double xMax = (1.-focus)*bounds[0]+(focus)*bounds[1];
    double yMin = (focus)*bounds[2]+(1.-focus)*bounds[3];
    double yMax = (1.-focus)*bounds[2]+(focus)*bounds[3];
    this->offscreenRenderer->ResetCamera(xMin,xMax,yMin,yMax,bounds[4],bounds[5]);
}

QImage gnomonAbstractVisualizationPrivate::offscreenImageRendering(void)
{
    // this->offscreenRenderWindowInteractor->Render();
    this->offscreenRenderWindow->Render();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(this->offscreenRenderWindow);
    windowToImageFilter->SetInputBufferTypeToRGBA(); 
    windowToImageFilter->ReadFrontBufferOff(); 
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

// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVisualization::gnomonAbstractVisualization(gnomonViewForm* view) : d(new gnomonAbstractVisualizationPrivate)
{
    d->view = view;
}

gnomonAbstractVisualization::~gnomonAbstractVisualization(void)
{
    delete d;

    d = NULL;
}

QMap<QString, gnomonCoreParameter *> gnomonAbstractVisualization::parameters(void) const
{
    return d->parameters;
}

void gnomonAbstractVisualization::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

//
// gnomonAbstractVisualization.cpp ends here
