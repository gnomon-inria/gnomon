// Version: $Id$
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

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

#include "gnomonActor.h"

#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// /////////////////////////////////////////////////////////////////
// gnomonActor2DCellImage
// /////////////////////////////////////////////////////////////////

class gnomonPolyDataCellImage;

class GNOMONVISUALIZATION_EXPORT gnomonActor2DCellImage : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DCellImage, vtkAssembly);

    static gnomonActor2DCellImage *New(void);

public:
    void setInteractor(void *interactor) override;
    virtual void hide(void);
    virtual void show(void);

public:
	void setPolyData(gnomonPolyDataCellImage *polydata);

    void setDimensions(int value[3]);
    void setSpacing(double value[3]);
    void setSlicePositions(int value[3]);

public slots:
    void update(void);
    void modified(void);

public slots:
    void setSliceOrientation(int);
    void setSlice(int);

public:
     gnomonActor2DCellImage(void);
    ~gnomonActor2DCellImage(void);

private:
    gnomonActor2DCellImage(const gnomonActor2DCellImage&);
    void operator = (const gnomonActor2DCellImage&);

private:
	class gnomonActor2DCellImagePrivate *d;
};

//
// gnomonActor2DCellImage.h ends here
