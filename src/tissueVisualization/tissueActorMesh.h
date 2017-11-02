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

#include <tissueVisualizationExport.h>

#include "tissueActor.h"

class vtkPolyData;

class TISSUEVISUALIZATION_EXPORT tissueActorMesh : public tissueActor
{
public:
    vtkTypeMacro(tissueActorMesh, vtkAssembly);

    static tissueActorMesh *New(void);

public:
    void setInteractor(void *interactor) override;
    void setMesh(vtkPolyData *mesh);

public slots:
    void update(void) override;

public slots:
    void outlineNone(void);
    void outlineCorners(void);
    void outlineBox(void);
    void outlineContour(void);

    void setClippingNone(bool clip);
    void setClippingBox(bool clip);
    void setClippingPlane(bool clip);
    void setClippingSphere(bool clip);

    void setDisplayVertices(bool display);
    void setDisplayEdges(bool display);
    void setDisplayFaces(bool display);
    void setDisplayTets(bool display);

    bool isDisplayVertices(void);
    bool isDisplayEdges(void);
    bool isDisplayFaces(void);
    bool isDisplayTets(void);

    void setColorVertices(const QColor& color);
    void setColorEdges(const QColor& color);
    void setColorFaces(const QColor& color);
    void setColorTets(const QColor& color);

protected:
     tissueActorMesh(void);
    ~tissueActorMesh(void);

private:
    tissueActorMesh(const tissueActorMesh&);
    void operator = (const tissueActorMesh&);

private:
    class tissueActorMeshPrivate *d;
};

//
// tissueActorMesh.h ends here
