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

class tissueGrowthSimulatorMainWindowPrivate
{
public:
};

tissueGrowthSimulatorMainWindow::tissueGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new tissueGrowthSimulatorMainWindowPrivate;

    QWidget *central = new QWidget(this);

    this->setCentralWidget(central);
}

tissueGrowthSimulatorMainWindow::~tissueGrowthSimulatorMainWindow(void)
{
    delete d;
}

//
// tissueGrowthSimulatorMainWindow.cpp ends here
