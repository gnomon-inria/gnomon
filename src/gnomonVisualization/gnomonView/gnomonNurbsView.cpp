#include "gnomonNurbsView.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// ///////////////////////////////////////////////////////////////////
// gnomonNurbsView
// ///////////////////////////////////////////////////////////////////

gnomonNurbsView::gnomonNurbsView(QObject* parent) : QObject(parent)
{
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
}

void gnomonNurbsView::associate(vtkRenderWindow *window)
{
    m_window = window;
    m_window->AddRenderer(m_renderer);
}

void gnomonNurbsView::addActor(vtkSmartPointer<vtkActor> actor) 
{
    m_renderer->AddActor(actor);
}