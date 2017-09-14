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

class tissueGrowthSimulatorPythonInterpreterWidgetPrivate;

class tissueGrowthSimulatorPythonInterpreterWidget : public QFrame
{
    Q_OBJECT

public:
     tissueGrowthSimulatorPythonInterpreterWidget(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorPythonInterpreterWidget(void);

protected slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void runFile();
private:
    tissueGrowthSimulatorPythonInterpreterWidgetPrivate *d;
};

//
// tissueGrowthSimulatorPythonInterpreterWidget.h ends here
