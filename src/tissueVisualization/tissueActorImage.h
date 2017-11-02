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

#include "tissueActor.h"

#include <tissueVisualizationExport.h>

#include <vtkImageData.h>

class TISSUEVISUALIZATION_EXPORT tissueActorImage : public tissueActor
{
protected:
     tissueActorImage(void);
    ~tissueActorImage(void);

private:
    tissueActorImage(const tissueActorImage&) = delete;
    void operator = (const tissueActorImage&) = delete;

public:
    vtkTypeMacro(tissueActorImage, vtkAssembly);

public:
    static tissueActorImage *New(void);

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
    class tissueActorImagePrivate *d;
};

//
// tissueActorImage.h ends here
