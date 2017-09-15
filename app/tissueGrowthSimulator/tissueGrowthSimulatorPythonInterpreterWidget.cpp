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

#include <dtkWidgets/dtkInterpreter.h>
#include <dtkSplitter>
#include <dtkLog.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript/dtkScriptInterpreterPython.h>
#endif

class tissueGrowthSimulatorPythonInterpreterWidgetPrivate
{
public:

    //tissueGrowthSimulatorPythonInterpreterWidgetPrivate() : unsaved(false) { }

    //QTextEdit* editor_widget;
    tissueGrowthSimulatorCodeEditor* editor_widget;

    dtkInterpreter* interpreter_widget;

#if defined(DTK_BUILD_WRAPPERS)
    dtkScriptInterpreterPython* interpreter;
#endif

    /*
    QString path;

    bool unsaved;

    bool hasFile()
    {
        return path.isEmpty() == false;
    }
    */
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
}

tissueGrowthSimulatorPythonInterpreterWidget::~tissueGrowthSimulatorPythonInterpreterWidget(void)
{
    delete d->interpreter_widget;
#if defined(DTK_BUILD_WRAPPERS)
    delete d->interpreter;
#endif
    delete d;
}

void tissueGrowthSimulatorPythonInterpreterWidget::openFile()
{
    /*
    if(d->unsaved)
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, "Save changes ?", "Save changes ?", QMessageBox::Yes|QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            //bool ret = saveFile();
        }
    }
    */

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

#if defined(DTK_BUILD_WRAPPERS)
    output = d->interpreter->interpret(input, &status);
#endif

    d->interpreter_widget->output(output);
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
