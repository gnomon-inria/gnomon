#pragma once

#include <gnomonVisualizationExport>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>
#include <vtkProperty.h>
#include <vtkTexture.h>

class GNOMONVISUALIZATION_EXPORT gnomonLandmarkActor : public vtkActor
{
public:
    vtkTypeMacro(gnomonLandmarkActor, vtkActor);

    static gnomonLandmarkActor *New();

    virtual int RenderOpaqueGeometry(vtkViewport *viewport) override;

    void ShallowCopy(vtkProp *prop) override;

public:
    gnomonLandmarkActor(std::size_t id);

public:
    std::size_t id(void) const;

    void setId(std::size_t id);

protected:
    vtkActor* device = nullptr;

    gnomonLandmarkActor();
    ~gnomonLandmarkActor();

private:
    virtual void render(vtkRenderer *ren);
    std::size_t m_id;
};
