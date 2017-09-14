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

#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class tissueGrowthSimulatorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     tissueGrowthSimulatorMainWindow(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorMainWindow(void);

private:
    class tissueGrowthSimulatorMainWindowPrivate *d;
};

//
// tissueGrowthSimulatorMainWindow.h ends here
//
