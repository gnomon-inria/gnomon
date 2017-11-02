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

#include "gnomonActor.h"

#include <gnomonVisualizationExport.h>

#include <vtkImageData.h>

class GNOMONVISUALIZATION_EXPORT gnomonActorImage : public gnomonActor
{
protected:
     gnomonActorImage(void);
    ~gnomonActorImage(void);

private:
    gnomonActorImage(const gnomonActorImage&) = delete;
    void operator = (const gnomonActorImage&) = delete;

public:
    vtkTypeMacro(gnomonActorImage, vtkAssembly);

public:
    static gnomonActorImage *New(void);

public:
    void setInteractor(void *interactor) override;
    void setImage(vtkImageData *image);

public:
    void showScalarBarTitle(bool show);

    void showPlaneX(bool value);
    void showPlaneY(bool value);
    void showPlaneZ(bool value);

    void show(void) override;
    void hide(void) override;

    void showScalarBar(bool show);

    void setScalarBarOrientationToVertical(bool value);

    void update(void) override;

public:
    vtkImageData *image(void);

private:
    class gnomonActorImagePrivate *d;
};

//
// gnomonActorImage.h ends here
