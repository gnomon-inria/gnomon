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
    d->tab_bar->addTab("Python interpreter");
    d->stacked_widget->addWidget(new QTextEdit());
}

tissueGrowthSimulatorEditorsPanel::~tissueGrowthSimulatorEditorsPanel(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
