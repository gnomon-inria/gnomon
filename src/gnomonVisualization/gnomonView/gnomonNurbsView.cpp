#include "gnomonNurbsView.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>


class gnomonNurbsViewPrivate {
public:
     gnomonNurbsViewPrivate(void) = default;
    ~gnomonNurbsViewPrivate(void) = default;

public:
    vtkSmartPointer<vtkRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer;

    QColor background_color;
};

// ///////////////////////////////////////////////////////////////////
// gnomonNurbsView
// ///////////////////////////////////////////////////////////////////

gnomonNurbsView::gnomonNurbsView(QObject* parent) : QObject(parent)
{
    d = new gnomonNurbsViewPrivate;
    d->renderer = vtkSmartPointer<vtkRenderer>::New();
}

void gnomonNurbsView::setBackgroundColor(const QColor& color)
{
    if (color != d->background_color) {
        d->background_color = color;
        d->renderer->SetBackground(color.redF(), color.greenF(), color.blueF());
        if (d->window) {
            d->window->Render();
        }
        emit backgroundColorChanged();
    }
}

const QColor& gnomonNurbsView::backgroundColor(void)
{
    return d->background_color;
}

void gnomonNurbsView::associate(vtkRenderWindow *window)
{
    d->window = window;
    d->window->AddRenderer(d->renderer);
}

void gnomonNurbsView::addActor(vtkSmartPointer<vtkActor> actor) 
{
    d->renderer->AddActor(actor);
}

vtkRenderWindow* gnomonNurbsView::renderWindow(void) const
{
    return d->window;
}