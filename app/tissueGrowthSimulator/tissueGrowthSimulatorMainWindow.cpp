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
    tissueGrowthSimulatorMainWidget *main_widget;
};

tissueGrowthSimulatorMainWindow::tissueGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new tissueGrowthSimulatorMainWindowPrivate;

    d->main_widget = new tissueGrowthSimulatorMainWidget;

    this->setCentralWidget(d->main_widget);
    this->setWindowTitle("Tissue Growth Simulator");

    this->resize(800, 600);
}

tissueGrowthSimulatorMainWindow::~tissueGrowthSimulatorMainWindow(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWindow.cpp ends here
