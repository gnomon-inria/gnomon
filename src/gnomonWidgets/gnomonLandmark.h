#pragma once

#include <gnomonWidgetsExport>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>
#include <vtkProperty.h>
#include <vtkTexture.h>

class GNOMONWIDGETS_EXPORT gnomonLandmark : public vtkActor
{
public:
    vtkTypeMacro(gnomonLandmark, vtkActor);

    static gnomonLandmark *New();

    virtual int RenderOpaqueGeometry(vtkViewport *viewport) override;

    void ShallowCopy(vtkProp *prop) override;

public:
    gnomonLandmark(std::size_t id);

public:
    std::size_t id(void) const;

    void setId(std::size_t id);

protected:
    vtkActor* device = nullptr;

    gnomonLandmark();
    ~gnomonLandmark();

private:
    virtual void render(vtkRenderer *ren);
    std::size_t m_id;
};
