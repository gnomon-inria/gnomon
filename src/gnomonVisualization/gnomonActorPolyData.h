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

class gnomonPolyData;

class GNOMONVISUALIZATION_EXPORT gnomonActorPolyData : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorPolyData, vtkAssembly);

    static gnomonActorPolyData *New(void);

public:
    void setInteractor(void *interactor) override;
	void setPolyData(gnomonPolyData *polydata);

public slots:
    void update(void) override;
    void modified(void);

    void setOpacity(double);
    void setColorMap(const QMap<double,QColor>&);

protected:
     gnomonActorPolyData(void);
    ~gnomonActorPolyData(void);

private:
    gnomonActorPolyData(const gnomonActorPolyData&);
    void operator = (const gnomonActorPolyData&);

private:
	class gnomonActorPolyDataPrivate *d;
};


//
// gnomonActorPolyData.h ends here