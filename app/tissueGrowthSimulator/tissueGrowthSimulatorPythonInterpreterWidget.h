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

#include <dtkSplitter>

class tissueGrowthSimulatorPythonInterpreterWidgetPrivate;

class tissueGrowthSimulatorPythonInterpreterWidget : public dtkSplitter
{
    Q_OBJECT

public:
     tissueGrowthSimulatorPythonInterpreterWidget(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorPythonInterpreterWidget(void);

public slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void runFile();

private:
    tissueGrowthSimulatorPythonInterpreterWidgetPrivate *d;
};

//
// tissueGrowthSimulatorPythonInterpreterWidget.h ends here
