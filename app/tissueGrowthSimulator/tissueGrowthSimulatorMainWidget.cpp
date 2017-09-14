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

#include "tissueGrowthSimulatorMainWidget.h"
#include "tissueGrowthSimulatorEditorsPanel.h"
#include <tissueCoreVisuWidget>
#include <QtOpenGL>

class tissueGrowthSimulatorMainWidgetPrivate
{
public:
    //QOpenGLWidget* visualization_widget;
    //QFrame* visualization_widget;
    tissueGrowthSimulatorEditorsPanel* editors_panel;
    tissueCoreVisuWidget* visualization_widget;
};

tissueGrowthSimulatorMainWidget::tissueGrowthSimulatorMainWidget(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    d = new tissueGrowthSimulatorMainWidgetPrivate;

    d->visualization_widget = new tissueCoreVisuWidget();
    layout->addWidget(d->visualization_widget);

    d->editors_panel = new tissueGrowthSimulatorEditorsPanel();
    layout->addWidget(d->editors_panel);
}

tissueGrowthSimulatorMainWidget::~tissueGrowthSimulatorMainWidget(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
