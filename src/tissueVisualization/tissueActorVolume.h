// Version: $Id: a4eb011a191f7045adf257089ea487145197909e $
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

#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

class vtkImageData;

class TISSUEVISUALIZATION_EXPORT tissueActorVolume : public tissueActor
{
public:
    vtkTypeMacro(tissueActorVolume, vtkAssembly);

public:
    static tissueActorVolume *New(void);

public:
    void setInteractor(void *interactor) override;
    void setVolume(vtkImageData *volume);

public:
    void showScalarBar(bool show);
    void showScalarBarTitle(bool show);
    void setScalarBarOrientationToVertical(bool value);

public slots:
    void update(void) override;
    void show(void) override;
    void hide(void) override;

public slots:
    void outlineNone(void);
    void outlineCorners(void);
    void outlineBox(void);
    void outlineContour(void);

public:
    void *colorTransferFunction(void);
    void *opacityTransferFunction(void);
    void *mapper(void);
    void *vol(void);
    void *volumeProperty(void);

public:
    void setColorTransferFunction(vtkColorTransferFunction *);
    void setOpacityTransferFunction(vtkPiecewiseFunction *);

protected:
     tissueActorVolume(void);
    ~tissueActorVolume(void);

private:
    tissueActorVolume(const tissueActorVolume&) = delete;
    void operator = (const tissueActorVolume&) = delete;

private:
    class tissueActorVolumePrivate *d;
};

//
// tissueActorVolume.h ends here
