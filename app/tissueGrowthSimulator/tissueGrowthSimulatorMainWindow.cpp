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

#include "tissueGrowthSimulatorMainWindow.h"
#include "tissueGrowthSimulatorMainWidget.h"

class tissueGrowthSimulatorMainWindowPrivate
{
public:
    tissueGrowthSimulatorMainWidget* main_widget;
};

tissueGrowthSimulatorMainWindow::tissueGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new tissueGrowthSimulatorMainWindowPrivate;
    d->main_widget = new tissueGrowthSimulatorMainWidget;
    setCentralWidget(d->main_widget);
    setWindowTitle("Tissue Growth Simulator");
    resize(800, 600);
}

tissueGrowthSimulatorMainWindow::~tissueGrowthSimulatorMainWindow(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWindow.cpp ends here
