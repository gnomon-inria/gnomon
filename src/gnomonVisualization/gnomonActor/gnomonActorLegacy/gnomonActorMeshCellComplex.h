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
#include "gnomonActorMesh.h"

class gnomonCellComplex;

class GNOMONVISUALIZATION_EXPORT gnomonActorMeshCellComplex : public gnomonActorMesh
{
public:
    vtkTypeMacro(gnomonActorMeshCellComplex, vtkAssembly);

    static gnomonActorMeshCellComplex *New(void);

public:
	void setCellComplex(gnomonCellComplex *cellcomplex);

public slots:
    void update(void) override;

protected:
     gnomonActorMeshCellComplex(void);
    ~gnomonActorMeshCellComplex(void);

private:
    gnomonActorMeshCellComplex(const gnomonActorMeshCellComplex&);
    void operator = (const gnomonActorMeshCellComplex&);

private:
	class gnomonActorMeshCellComplexPrivate *dd;
};


//
// gnomonActorMeshCellComplex.h ends here