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

#include "gnomonActor.h"

#include <gnomonVisualizationExport.h>

#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

class vtkImageData;

class GNOMONVISUALIZATION_EXPORT gnomonActorVolume : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorVolume, vtkAssembly);

public:
    static gnomonActorVolume *New(void);

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
    double rangeMin() const;
    double rangeMax() const;

    const QList<int>& histogram() const;

public:
    void setColorTransferFunction(vtkColorTransferFunction *);
    void setOpacityTransferFunction(vtkPiecewiseFunction *);

protected:
     gnomonActorVolume(void);
    ~gnomonActorVolume(void);

private:
    gnomonActorVolume(const gnomonActorVolume&) = delete;
    void operator = (const gnomonActorVolume&) = delete;

private:
    class gnomonActorVolumePrivate *d;
};

//
// gnomonActorVolume.h ends here
