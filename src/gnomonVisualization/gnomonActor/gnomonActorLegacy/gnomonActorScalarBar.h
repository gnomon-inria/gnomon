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

#include "gnomonActor/gnomonActor.h"

#include <gnomonVisualizationExport.h>

#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

class vtkImageData;

class GNOMONVISUALIZATION_EXPORT gnomonActorScalarBar : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorScalarBar, vtkAssembly);

public:
    static gnomonActorScalarBar *New(void);

public:
    void setInteractor(void *interactor) override;
    void setData(vtkImageData *data);

public:
    void showTitle(bool show);
    void setOrientationToVertical(bool value);

public slots:
    void update(void) override;
    void show(void) override;
    void hide(void) override;

public:
    void *colorTransferFunction(void);

public:
    void setColorTransferFunction(vtkColorTransferFunction *);

protected:
     gnomonActorScalarBar(void);
    ~gnomonActorScalarBar(void);

private:
    gnomonActorScalarBar(const gnomonActorScalarBar&) = delete;
    void operator = (const gnomonActorScalarBar&) = delete;

private:
    class gnomonActorScalarBarPrivate *d;
};

//
// gnomonActorScalarBar.h ends here
