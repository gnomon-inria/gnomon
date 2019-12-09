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

class GNOMONVISUALIZATION_EXPORT gnomonActor2DImage : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DImage, vtkAssembly);

    static gnomonActor2DImage *New(void);

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
     gnomonActor2DImage(void);
    ~gnomonActor2DImage(void);

private:
    gnomonActor2DImage(const gnomonActor2DImage&);
    void operator = (const gnomonActor2DImage&);

private:
	class gnomonActor2DImagePrivate *d;
};


//
// gnomonActor2DImage.h ends here