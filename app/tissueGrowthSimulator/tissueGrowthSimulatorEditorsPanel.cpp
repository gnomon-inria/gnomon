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

#include "tissueGrowthSimulatorEditorsPanel.h"
#include "tissueGrowthSimulatorPythonInterpreterWidget.h"
#include "tissueGrowthSimulatorComposerWidget.h"

class tissueGrowthSimulatorEditorsPanelPrivate
{
public:
    QTabBar* tab_bar;
    QStackedWidget* stacked_widget;
};

tissueGrowthSimulatorEditorsPanel::tissueGrowthSimulatorEditorsPanel(QWidget *parent) : QFrame(parent)
{
    d = new tissueGrowthSimulatorEditorsPanelPrivate;

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    d->tab_bar = new QTabBar;
    layout->addWidget(d->tab_bar);

    d->stacked_widget = new QStackedWidget;
    layout->addWidget(d->stacked_widget);

    d->tab_bar->addTab("Composer");
    d->stacked_widget->addWidget(new tissueGrowthSimulatorComposerWidget);

    d->tab_bar->addTab("Python interpreter");
    d->stacked_widget->addWidget(new tissueGrowthSimulatorPythonInterpreterWidget);

    QObject::connect(d->tab_bar, SIGNAL(currentChanged(int)), d->stacked_widget, SLOT(setCurrentIndex(int)));
}

tissueGrowthSimulatorEditorsPanel::~tissueGrowthSimulatorEditorsPanel(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
