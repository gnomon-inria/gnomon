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

class GNOMONVISUALIZATION_EXPORT gnomonActor2DImageRGBA : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DImageRGBA, vtkAssembly);

    static gnomonActor2DImageRGBA *New(void);

public:
    void setInteractor(void *interactor) override;
	void setImage(vtkImageData *image);

public slots:
    void update(void) override;
    void modified(void);

public slots:
    void setSliceOrientation(int);
    void setSlice(int);

public slots:
    void hide(void) override;
    void show(void) override;

public slots:
    void setOpacity(double);

protected:
     gnomonActor2DImageRGBA(void);
    ~gnomonActor2DImageRGBA(void);

private:
    gnomonActor2DImageRGBA(const gnomonActor2DImageRGBA&);
    void operator = (const gnomonActor2DImageRGBA&);

private:
	class gnomonActor2DImageRGBAPrivate *d;
};


//
// gnomonActor2DImageRGBA.h ends here