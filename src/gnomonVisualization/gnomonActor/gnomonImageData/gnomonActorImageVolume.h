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

class GNOMONVISUALIZATION_EXPORT gnomonActorImageVolume : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorImageVolume, vtkAssembly);

    static gnomonActorImageVolume *New(void);

public:
    void setInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;
    void setImage(vtkImageData *image);

public slots:
    void update(void) override;
    void modified(void);

    void setOpacity(double);
    void setValueRange(const std::array<long long int, 2>& value);
    void setColorMap(const QMap<double,QColor>&);
    void setFlatRendering(bool);

protected:
     gnomonActorImageVolume(void);
    ~gnomonActorImageVolume(void);

private:
    gnomonActorImageVolume(const gnomonActorImageVolume&);
    void operator = (const gnomonActorImageVolume&);

private:
    class gnomonActorImageVolumePrivate *d;
};


//
// gnomonActorImageVolume.h ends here
