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

#include "tissueView.h"

#include <dtkWidgets>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <QVTKOpenGLWidget.h>

class tissueViewPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;
};

tissueView::tissueView(QWidget *parent) : dtkViewWidget(parent)
{
    d = new tissueViewPrivate;

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(1, 1, 1);

    d->window = vtkGenericOpenGLRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new QVTKOpenGLWidget(this);
    d->widget->SetRenderWindow(d->window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
}

tissueView::~tissueView(void)
{
    d->renderer->Delete();
    d->window->Delete();

    delete d->widget;
    delete d;
}

QWidget *tissueView::widget(void)
{
    return d->widget;
}

//
// tissueView.cpp ends here
