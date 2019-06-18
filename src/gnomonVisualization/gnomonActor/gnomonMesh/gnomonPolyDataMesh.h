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
#include "gnomonActor/gnomonPolyData/gnomonPolyData.h"

class gnomonMesh;

class GNOMONVISUALIZATION_EXPORT gnomonPolyDataMesh : public gnomonPolyData
{
public:
    vtkTypeMacro(gnomonPolyDataMesh, vtkPolyData);

    static gnomonPolyDataMesh *New(void);

public:
	void setMesh(gnomonMesh *cellimage);
    void setPropertyName(const QString& property_name);

public slots:
    void update(void) override;
    void modified(void);

public:
     gnomonPolyDataMesh(void);
    ~gnomonPolyDataMesh(void);

private:
    gnomonPolyDataMesh(const gnomonPolyDataMesh&);
    void operator = (const gnomonPolyDataMesh&);

private:
	class gnomonPolyDataMeshPrivate *d;
};


//
// gnomonPolyDataMesh.h ends here