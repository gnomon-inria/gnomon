// Version: $Id: 6a307e715418948aafce8e5b8ffbd3b9f63b9ff0 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include "gnomonActor/gnomonActor.h"

#include <gnomonVisualizationExport.h>

class vtkImageData;
class vtkColorTransferFunction;

class GNOMONVISUALIZATION_EXPORT gnomonActorImage : public gnomonActor
{
 public:
    vtkTypeMacro(gnomonActorImage, vtkAssembly);

protected:
     gnomonActorImage(void);
    ~gnomonActorImage(void);

private:
    gnomonActorImage(const gnomonActorImage&) = delete;
    void operator = (const gnomonActorImage&) = delete;

public:
    static gnomonActorImage *New(void);

public:
    void setInteractor(vtkSmartPointer<vtkRenderWindowInteractor>*interactor) override;
    void setImage(vtkImageData *image);

public:
    void showPlaneX(bool value);
    void showPlaneY(bool value);
    void showPlaneZ(bool value);

    void show(void) override;
    void hide(void) override;

    void *colorTransferFunction(void);

    void setColorTransferFunction(vtkColorTransferFunction *);

    void update(void) override;

    void setXPlanePos(int);
    void setYPlanePos(int);
    void setZPlanePos(int);

    void setXPlaneOpacity(double);
    void setYPlaneOpacity(double);
    void setZPlaneOpacity(double);

public:
    vtkImageData *image(void);

private:
    class gnomonActorImagePrivate *d;
};

//
// gnomonActorImage.h ends here
