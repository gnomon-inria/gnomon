#include "gnomonLandmarkActor.h"

int gnomonLandmarkActor::RenderOpaqueGeometry(vtkViewport *viewport)
{
    if (!this->Mapper) return 0;

    if (this->GetIsOpaque()) {
        vtkRenderer *ren = static_cast<vtkRenderer *>(viewport);
        this->render(ren);
        return 1;
    }
    return 0;
}

void gnomonLandmarkActor::render(vtkRenderer *ren)
{
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

void gnomonLandmarkActor::ShallowCopy(vtkProp *prop)
{
    gnomonLandmarkActor *f = gnomonLandmarkActor::SafeDownCast(prop);
    this->vtkActor::ShallowCopy(prop);
}

gnomonLandmarkActor::gnomonLandmarkActor(std::size_t id) : vtkActor(), m_id(id)
{
};

std::size_t gnomonLandmarkActor::id(void) const
{
    return m_id;
}

void gnomonLandmarkActor::setId(std::size_t id)
{
    m_id = id;
}

gnomonLandmarkActor::gnomonLandmarkActor() : m_id(0)
{
    this->device = vtkActor::New();
}

gnomonLandmarkActor::~gnomonLandmarkActor()
{
    this->device->Delete();
}

vtkStandardNewMacro(gnomonLandmarkActor)
