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
#include "gnomonActorMesh.h"

class gnomonCellImage;

class GNOMONVISUALIZATION_EXPORT gnomonActorMeshCellImage : public gnomonActorMesh
{
public:
    vtkTypeMacro(gnomonActorMeshCellImage, vtkAssembly);

    static gnomonActorMeshCellImage *New(void);

public:
	void setCellImage(gnomonCellImage *cellimage);

public slots:
    void update(void) override;
    void modified(void);

protected:
     gnomonActorMeshCellImage(void);
    ~gnomonActorMeshCellImage(void);

private:
    gnomonActorMeshCellImage(const gnomonActorMeshCellImage&);
    void operator = (const gnomonActorMeshCellImage&);

private:
	class gnomonActorMeshCellImagePrivate *dd;
};


//
// gnomonActorMeshCellImage.h ends here