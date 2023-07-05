#include "gnomonVtkDecorator.h"

#include <vtkRenderer.h>

void gnomonVtkDecorator::set2DClippingPlane(vtkSmartPointer<vtkPlane> plane)
{
    m_clippingPlane = plane;

    //it's up to decorators to do things like this or not
    //if(m_mapper2d) {
    //    m_mapper2d->RemoveAllClippingPlanes();
    //    m_mapper2d->AddClippingPlane(plane);
    //    m_mapper2d->Update();
    //}
}

void gnomonVtkDecorator::AddActorIfPossible(void)
{
   if(m_view && m_is_decorating) {
       if(m_actor3d) m_view->renderer3D()->AddActor(m_actor3d);
       if(m_actor2d) m_view->renderer2D()->AddActor(m_actor2d);
   }
}

vtkSmartPointer<vtkActor> gnomonVtkDecorator::actor(void)
{
    return m_actor3d;
}

void gnomonVtkDecorator::removeActor(void)
{
    if(m_view) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d) m_view->renderer2D()->RemoveActor(m_actor2d);
    }
}

void gnomonVtkDecorator::setVisibility(bool visible)
{
    if(m_view) {
        if(m_actor3d) m_actor3d->SetVisibility(visible);
        if(m_actor2d) m_actor2d->SetVisibility(visible);
    }
}


bool gnomonVtkDecorator::isDecorating()
{
    return m_is_decorating;
}

void gnomonVtkDecorator::setView(gnomonVtkView *view)
{
    m_view = view;
    if(m_is_decorating) {
        if(m_actor3d) m_view->renderer3D()->AddActor(m_actor3d);
        if(m_actor2d) m_view->renderer2D()->AddActor(m_actor2d);
    }
}

void gnomonVtkDecorator::unsetView()
{
    if(m_is_decorating) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d) m_view->renderer2D()->RemoveActor(m_actor2d);
    }
    m_is_decorating = false;
    m_view = nullptr;
}

dtkCoreParameters gnomonVtkDecorator::parameters()
{
    return m_parameters;
}
