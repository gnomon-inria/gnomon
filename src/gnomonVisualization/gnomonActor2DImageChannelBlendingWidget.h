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

class GNOMONVISUALIZATION_EXPORT gnomonActor2DImageChannelBlendingWidget : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DImageChannelBlendingWidget, vtkAssembly);

    static gnomonActor2DImageChannelBlendingWidget *New(void);

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
     gnomonActor2DImageChannelBlendingWidget(void);
    ~gnomonActor2DImageChannelBlendingWidget(void);

private:
    gnomonActor2DImageChannelBlendingWidget(const gnomonActor2DImageChannelBlendingWidget&);
    void operator = (const gnomonActor2DImageChannelBlendingWidget&);

private:
	class gnomonActor2DImageChannelBlendingWidgetPrivate *d;
};


//
// gnomonActor2DImageChannelBlendingWidget.h ends here