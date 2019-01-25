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

class GNOMONVISUALIZATION_EXPORT gnomonActor2DImageWidget : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DImageWidget, vtkAssembly);

    static gnomonActor2DImageWidget *New(void);

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
    void setValueRange(const QList<int>& value);
    void setColorMap(const QMap<double,QColor>&);
    void setFlatRendering(bool);

protected:
     gnomonActor2DImageWidget(void);
    ~gnomonActor2DImageWidget(void);

private:
    gnomonActor2DImageWidget(const gnomonActor2DImageWidget&);
    void operator = (const gnomonActor2DImageWidget&);

private:
	class gnomonActor2DImageWidgetPrivate *d;
};


//
// gnomonActor2DImageWidget.h ends here