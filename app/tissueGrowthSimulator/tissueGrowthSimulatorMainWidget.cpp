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
#include <QtOpenGL>

class tissueGrowthSimulatorMainWidgetPrivate
{
public:
    QOpenGLWidget* visualization_widget;
    //QFrame* visualization_widget;
    tissueGrowthSimulatorEditorsPanel* editors_panel;
};

tissueGrowthSimulatorMainWidget::tissueGrowthSimulatorMainWidget(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    d = new tissueGrowthSimulatorMainWidgetPrivate;

    d->visualization_widget = new QOpenGLWidget();
    d->visualization_widget->setFixedHeight(400);
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
