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

#include "tissueCoreVisuWidget.h"
#include <dtkWidgets>
#include <dtkComposer/dtkComposerViewManager.h>
#include <dtkComposer/dtkComposerViewWidget.h>
#include <dtkComposer/dtkComposerViewController.h>
#include <QVTKOpenGLWidget.h>
#include <vtkContextView.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

class tissueCoreVisuWidgetPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;
};

tissueCoreVisuWidget::tissueCoreVisuWidget(QWidget *parent) : QFrame(parent)
{
    d = new tissueCoreVisuWidgetPrivate;

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(1, 0, 0);

    d->window = vtkGenericOpenGLRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new QVTKOpenGLWidget(this);
    d->widget->SetRenderWindow(d->window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);

    this->setFixedHeight(300);
    this->setLayout(layout);
}

tissueCoreVisuWidget::~tissueCoreVisuWidget(void)
{
    delete d->widget;
    delete d;
}

//
// tissueCoreVisuWidget.cpp ends here
