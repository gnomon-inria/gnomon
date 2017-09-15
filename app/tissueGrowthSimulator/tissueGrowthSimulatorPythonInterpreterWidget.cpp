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
#include "tissueGrowthSimulatorCodeEditor.h"

#include <dtkConfig.h>

#include <dtkWidgets/dtkNotificationDisplay.h>
#include <dtkWidgets/dtkNotification.h>
#include <dtkWidgets/dtkInterpreter.h>
#include <dtkWidgets/dtkSplitter.h>
#include <dtkLog/dtkLogger.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript/dtkScriptInterpreterPython.h>
#endif

class tissueGrowthSimulatorPythonInterpreterWidgetPrivate
{
public:

    tissueGrowthSimulatorCodeEditor* editor_widget;

    dtkInterpreter* interpreter_widget;

#if defined(DTK_BUILD_WRAPPERS)
    dtkScriptInterpreterPython* interpreter;
#endif
};

tissueGrowthSimulatorPythonInterpreterWidget::tissueGrowthSimulatorPythonInterpreterWidget(QWidget *parent) : dtkSplitter(parent)
{
    d = new tissueGrowthSimulatorPythonInterpreterWidgetPrivate;

    // Create the editor and the prompt and add them to the splitter.

    d->editor_widget = new tissueGrowthSimulatorCodeEditor;
    addWidget(d->editor_widget);

    d->interpreter_widget = new dtkInterpreter;
#if defined(DTK_BUILD_WRAPPERS)
    d->interpreter = new dtkScriptInterpreterPython;
    d->interpreter_widget->registerInterpreter(d->interpreter);
#endif
    addWidget(d->interpreter_widget);

    // Put some sample python into the editor.

    QString python_sample;
    QTextStream stream(&python_sample);
    stream << "#\n";
    stream << "# " + QDate::currentDate().toString() + "\n";
    stream << "#\n";
    d->editor_widget->setPlainText(python_sample);
}

tissueGrowthSimulatorPythonInterpreterWidget::~tissueGrowthSimulatorPythonInterpreterWidget()
{
    delete d->interpreter_widget;
#if defined(DTK_BUILD_WRAPPERS)
    delete d->interpreter;
#endif
    delete d;
}

void tissueGrowthSimulatorPythonInterpreterWidget::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Open script");

    if(path.isNull() == false)
    {
        QFile file(path);
        if(file.open(QFile::ReadOnly))
        {
            QTextStream stream(&file);
            QString buffer;
            QString line;

            while(stream.readLineInto(&line))
            {
                buffer += line;
                buffer += "\n";
            }

            d->editor_widget->setPlainText(buffer);

            file.close();
        }
    }
}

void tissueGrowthSimulatorPythonInterpreterWidget::saveFile()
{
    ;
}

void tissueGrowthSimulatorPythonInterpreterWidget::saveFileAs()
{
    ;
}

void tissueGrowthSimulatorPythonInterpreterWidget::runFile()
{
    QString input = d->editor_widget->toPlainText();

    int status;

    QString output;

    QTime time;
    time.start();

#if defined(DTK_BUILD_WRAPPERS)
    output = d->interpreter->interpret(input, &status);
#endif

    int elapsed = time.elapsed();

    d->interpreter_widget->output(output);

    dtkNotify(QString("Script executed in %1 ms.").arg(elapsed), 30000);
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
