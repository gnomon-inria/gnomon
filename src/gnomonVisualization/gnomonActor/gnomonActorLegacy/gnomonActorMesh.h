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

#include "gnomonActor/gnomonActor.h"

class vtkColorTransferFunction;
class vtkPolyData;

class GNOMONVISUALIZATION_EXPORT gnomonActorMesh : public gnomonActor
{
public:
    vtkTypeMacro(gnomonActorMesh, vtkAssembly);

    static gnomonActorMesh *New(void);

public:
    void setInteractor(void *interactor) override;
    void setMesh(vtkPolyData *mesh);

    virtual void hide(void) override;
    virtual void show(void) override;

public:
    void setColorTransferFunction(vtkColorTransferFunction *);

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
     gnomonActorMesh(void);
    ~gnomonActorMesh(void);

private:
    gnomonActorMesh(const gnomonActorMesh&);
    void operator = (const gnomonActorMesh&);

protected:
    class gnomonActorMeshPrivate *d;
};

//
// gnomonActorMesh.h ends here
