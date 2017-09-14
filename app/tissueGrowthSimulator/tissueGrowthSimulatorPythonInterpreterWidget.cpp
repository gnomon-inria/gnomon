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
#include <dtkSplitter>
#include <dtkLog.h>

class tissueGrowthSimulatorPythonInterpreterWidgetPrivate
{
public:

    //tissueGrowthSimulatorPythonInterpreterWidgetPrivate() : unsaved(false) { }

    QTextEdit* editor_widget;

    dtkInterpreter* interpreter_widget;

    dtkScriptInterpreterPython* interpreter;

    /*
    QString path;

    bool unsaved;

    bool hasFile()
    {
        return path.isEmpty() == false;
    }
    */
};

tissueGrowthSimulatorPythonInterpreterWidget::tissueGrowthSimulatorPythonInterpreterWidget(QWidget *parent) : QFrame(parent)
{
    d = new tissueGrowthSimulatorPythonInterpreterWidgetPrivate;

    // Create the layout and fill it with widgets.

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    QToolBar* tb = new QToolBar;
    layout->addWidget(tb);

    dtkSplitter* splitter = new dtkSplitter;
    layout->addWidget(splitter, 1);

    // Create the editor and the prompt and add them to the splitter.

    d->editor_widget = new QTextEdit;
    d->editor_widget->setFontFamily("monospace");
    splitter->addWidget(d->editor_widget);

    d->interpreter_widget = new dtkInterpreter;
    d->interpreter = new dtkScriptInterpreterPython;
    d->interpreter_widget->registerInterpreter(d->interpreter);
    splitter->addWidget(d->interpreter_widget);

    // Fill the toolbar.

    QAction* a_open = tb->addAction("Open");
    connect(a_open, SIGNAL(triggered()), this, SLOT(openFile()));

    QAction* a_save = tb->addAction("Save");
    connect(a_save, SIGNAL(triggered()), this, SLOT(saveFile()));

    QAction* a_saveas = tb->addAction("Save as");
    connect(a_saveas, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    QAction* a_run = tb->addAction("Run");
    connect(a_run, SIGNAL(triggered()), this, SLOT(runFile()));
}

tissueGrowthSimulatorPythonInterpreterWidget::~tissueGrowthSimulatorPythonInterpreterWidget(void)
{
    delete d->interpreter_widget;
    delete d->interpreter;
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

    QString output = d->interpreter->interpret(input, &status);

    d->interpreter_widget->output(output);
}

//
// tissueGrowthSimulatorMainWidget.cpp ends here
