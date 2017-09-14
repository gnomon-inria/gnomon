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

#include "tissueGrowthSimulatorVisualizationWidget.h"
#include <QtOpenGL>
#include <dtkWidgets>
#include <dtkComposer/dtkComposerViewManager.h>
#include <dtkComposer/dtkComposerViewWidget.h>
#include <dtkComposer/dtkComposerViewController.h>
#include <QVTKOpenGLWidget.h>
#include <vtkContextView.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

class tissueGrowthSimulatorVisualizationWidgetPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;
};

tissueGrowthSimulatorVisualizationWidget::tissueGrowthSimulatorVisualizationWidget(QWidget *parent) : QFrame(parent)
{
    d = new tissueGrowthSimulatorVisualizationWidgetPrivate;

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

tissueGrowthSimulatorVisualizationWidget::~tissueGrowthSimulatorVisualizationWidget(void)
{
    delete d;
}

//
// tissueGrowthSimulatorVisualizationWidget.cpp ends here
