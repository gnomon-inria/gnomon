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

#include <dtkWidgets/dtkViewController.h>
#include <dtkWidgets/dtkViewManager.h>

#include <QtOpenGL>

class tissueGrowthSimulatorMainWidgetPrivate
{
public:
    tissueGrowthSimulatorEditorsPanel *editors_panel;

public:
    tissueCoreVisuWidget *visualization_widget;

public:
    dtkViewManager *view_manager;
};

tissueGrowthSimulatorMainWidget::tissueGrowthSimulatorMainWidget(QWidget *parent) : QFrame(parent)
{
    d = new tissueGrowthSimulatorMainWidgetPrivate;

    d->view_manager = new dtkViewManager(this);
    d->editors_panel = new tissueGrowthSimulatorEditorsPanel(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->view_manager);
    layout->addWidget(d->editors_panel);

    this->setLayout(layout);

    // ///////////////////////////////////////////////////////////////////
    // Instanciating main vtk view
    // ///////////////////////////////////////////////////////////////////

    d->visualization_widget = new tissueCoreVisuWidget;
    d->visualization_widget->setObjectName("Main View");

    QTimer::singleShot(1000, this, [=] () {
        dtkViewController::instance()->add(d->visualization_widget);
    });
}

tissueGrowthSimulatorMainWidget::~tissueGrowthSimulatorMainWidget(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
