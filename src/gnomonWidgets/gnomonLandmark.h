#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderingCoreModule.h>
#include <vtkProperty.h>
#include <vtkTexture.h>

class gnomonLandmark : public vtkActor
{
public:
    vtkTypeMacro(gnomonLandmark, vtkActor);

    static gnomonLandmark *New();

    virtual int RenderOpaqueGeometry(vtkViewport *viewport) {
        if (!this->Mapper) return 0;

        if (this->GetIsOpaque()) {
            vtkRenderer *ren = static_cast<vtkRenderer *>(viewport);
            this->Render(ren);
            return 1;
        }
        return 0;
    }

    virtual void Render(vtkRenderer *ren){
        this->Property->Render(this, ren);
        this->device->SetProperty (this->Property);
        this->Property->Render(this, ren);
        if (this->BackfaceProperty) {
            this->BackfaceProperty->BackfaceRender(this, ren);
            this->device->SetBackfaceProperty(this->BackfaceProperty);
        }
        if (this->Texture) {
            this->Texture->Render(ren);
        }
        this->ComputeMatrix();
        this->device->SetUserMatrix(this->Matrix);
        this->device->Render(ren,this->Mapper);
    }

    void ShallowCopy(vtkProp *prop) {
        gnomonLandmark *f = gnomonLandmark::SafeDownCast(prop);
        this->vtkActor::ShallowCopy(prop);
    }
public:
gnomonLandmark(std::size_t id) : vtkActor(), m_id(id) { };

public:
    std::size_t id(void) const
    {
        return m_id;
    }

    void setId(std::size_t id)
    {
        m_id = id;
    }

protected:
    vtkActor* device = nullptr;

gnomonLandmark() : m_id(0) {
        this->device = vtkActor::New();
    }

    ~gnomonLandmark() {
        this->device->Delete();
    }

private:
    std::size_t m_id;
};

vtkStandardNewMacro(gnomonLandmark)
