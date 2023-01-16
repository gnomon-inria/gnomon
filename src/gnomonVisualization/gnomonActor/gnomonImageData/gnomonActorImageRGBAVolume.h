// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

#include "gnomonActor/gnomonActor.h"

class vtkImageData;

class GNOMONVISUALIZATION_EXPORT gnomonActorImageRGBAVolume : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorImageRGBAVolume, vtkAssembly);

    static gnomonActorImageRGBAVolume *New(void);

public:
    void setInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;
    void setImage(vtkImageData *image);

public slots:
    void update(void) override;
    void modified(void);

    void setOpacity(double);

protected:
     gnomonActorImageRGBAVolume(void);
    ~gnomonActorImageRGBAVolume(void);

private:
    gnomonActorImageRGBAVolume(const gnomonActorImageRGBAVolume&);
    void operator = (const gnomonActorImageRGBAVolume&);

private:
    class gnomonActorImageRGBAVolumePrivate *d;
};


//
// gnomonActorImageRGBAVolume.h ends here
