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
// gnomonActor2DPolyData
// /////////////////////////////////////////////////////////////////

class gnomonPolyData;

class GNOMONVISUALIZATION_EXPORT gnomonActor2DPolyData : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActor2DPolyData, vtkAssembly);

    static gnomonActor2DPolyData *New(void);

public:
    void setInteractor(void *interactor) override;
    virtual void hide(void);
    virtual void show(void);

public:
	void setPolyData(gnomonPolyData *polydata);

    void setSliceThickness(double value);
    void setSlicePositions(double value[3]);

public slots:
    void update(void);
    void modified(void);

public slots:
    void setSliceOrientation(int);
    void setSlice(int);
    void setOpacity(double);
    void setValueRange(const QList<double>& value);
    void setColorMap(const QMap<double,QColor>&);

public:
     gnomonActor2DPolyData(void);
    ~gnomonActor2DPolyData(void);

private:
    gnomonActor2DPolyData(const gnomonActor2DPolyData&);
    void operator = (const gnomonActor2DPolyData&);

private:
	class gnomonActor2DPolyDataPrivate *d;
};

//
// gnomonActor2DPolyData.h ends here
