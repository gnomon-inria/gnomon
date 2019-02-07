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

#include "gnomonActor.h"

class vtkImageData;

class GNOMONVISUALIZATION_EXPORT gnomonActor2DImageRGBAWidget : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DImageRGBAWidget, vtkAssembly);

    static gnomonActor2DImageRGBAWidget *New(void);

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
     gnomonActor2DImageRGBAWidget(void);
    ~gnomonActor2DImageRGBAWidget(void);

private:
    gnomonActor2DImageRGBAWidget(const gnomonActor2DImageRGBAWidget&);
    void operator = (const gnomonActor2DImageRGBAWidget&);

private:
	class gnomonActor2DImageRGBAWidgetPrivate *d;
};


//
// gnomonActor2DImageRGBAWidget.h ends here