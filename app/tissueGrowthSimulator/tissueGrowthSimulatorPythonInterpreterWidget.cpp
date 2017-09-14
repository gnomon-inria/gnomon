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

#include "tissueGrowthSimulatorPythonInterpreterWidget.h"


#include <dtkGuiSupport/dtkInterpreter.h>
#include <dtkScript/dtkScriptInterpreterPython.h>


class tissueGrowthSimulatorPythonInterpreterWidgetPrivate
{
public:
    QTextEdit* editor;
    dtkScriptInterpreterPython* script_interpreter;
    dtkInterpreter* interpreter;
};

tissueGrowthSimulatorPythonInterpreterWidget::tissueGrowthSimulatorPythonInterpreterWidget(QWidget *parent) : dtkSplitter(parent)
{
    d = new tissueGrowthSimulatorPythonInterpreterWidgetPrivate;

    d->editor = new QTextEdit;
    addWidget(d->editor);

    d->interpreter = new dtkInterpreter;
    d->script_interpreter = new dtkScriptInterpreterPython;
    d->interpreter->registerInterpreter(d->script_interpreter);
    addWidget(d->interpreter);
}

tissueGrowthSimulatorPythonInterpreterWidget::~tissueGrowthSimulatorPythonInterpreterWidget(void)
{
    delete d->interpreter;
    delete d->script_interpreter;
    delete d;
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
