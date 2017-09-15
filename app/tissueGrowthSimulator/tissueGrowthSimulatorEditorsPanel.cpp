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
#include "tissueGrowthSimulatorMainToolBar.h"

class tissueGrowthSimulatorEditorsPanelPrivate
{
public:
    tissueGrowthSimulatorMainToolBar* tool_bar;
    QStackedWidget* stacked_widget;
    tissueGrowthSimulatorComposerWidget* composer_widget;
    tissueGrowthSimulatorPythonInterpreterWidget* script_widget;
    QTabBar* tab_bar;
};

tissueGrowthSimulatorEditorsPanel::tissueGrowthSimulatorEditorsPanel(QWidget *parent) : QFrame(parent)
{
    d = new tissueGrowthSimulatorEditorsPanelPrivate;

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    d->tool_bar = new tissueGrowthSimulatorMainToolBar(this);
    // TODO : set up the tool bar.
    layout->addWidget(d->tool_bar);

    d->stacked_widget = new QStackedWidget(this);
    layout->addWidget(d->stacked_widget);

    d->tab_bar = new QTabBar(this);
    d->tab_bar->setShape(QTabBar::RoundedSouth);
    layout->addWidget(d->tab_bar);
    
    // Create the composer widget.

    d->tab_bar->addTab("Composer");
    d->composer_widget = new tissueGrowthSimulatorComposerWidget;
    d->stacked_widget->addWidget(d->composer_widget);

    // Create the script widget.

    d->tab_bar->addTab("Python");
    d->script_widget = new tissueGrowthSimulatorPythonInterpreterWidget;
    d->stacked_widget->addWidget(d->script_widget);

    connect(d->tab_bar, SIGNAL(currentChanged(int)), d->stacked_widget, SLOT(setCurrentIndex(int)));
    connect(d->tab_bar, SIGNAL(currentChanged(int)), d->tool_bar, SLOT(setMode(int)));
    d->tool_bar->setComposerWidget(d->composer_widget->composerWidget());
    d->tool_bar->setScriptWidget(d->script_widget);

    d->tab_bar->setCurrentIndex(0);
    d->stacked_widget->setCurrentIndex(0);
    d->tool_bar->setMode(0);
}

tissueGrowthSimulatorEditorsPanel::~tissueGrowthSimulatorEditorsPanel(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
