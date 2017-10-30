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

#include <dtkConfig.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript>
#endif

#include <dtkComposer>
#include <dtkWidgets>

#include <tissueCore>
#include <tissueComposer>
#include <tissueWidgets>
#include <tissueFonts>

class tissueGrowthSimulatorMainWindowPrivate
{
public:
    tissueComposerWidget *composer;

public:
    tissueCodeEditor *editor;

public:
    dtkInterpreter *interpreter_widget;

#if defined(DTK_BUILD_WRAPPERS)
    dtkScriptInterpreterPython *interpreter;
#endif

public:
    dtkViewManager *view_manager;

public:
    QTabWidget *editors;

public:
    dtkSplitter *editor_splitter;

public:
    QToolBar *tool_bar;

    QAction *open_action;
    QAction *run_action;
    QAction *save_action;
    QAction *save_as_action;

public:
    tissueFontAwesome *font;
};

tissueGrowthSimulatorMainWindow::tissueGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new tissueGrowthSimulatorMainWindowPrivate;

    d->font = new tissueFontAwesome(this);
    d->font->initFontAwesome();

    d->view_manager = new dtkViewManager(this);

    d->composer = new tissueComposerWidget(this);

    d->editor = new tissueCodeEditor;

    d->interpreter_widget = new dtkInterpreter;

#if defined(DTK_BUILD_WRAPPERS)
    d->interpreter = new dtkScriptInterpreterPython;

    d->interpreter_widget->registerInterpreter(d->interpreter);
#endif

    d->editor_splitter = new dtkSplitter(this);
    d->editor_splitter->addWidget(d->editor);
    d->editor_splitter->addWidget(d->interpreter_widget);

    d->editors = new QTabWidget(this);
    d->editors->setContentsMargins(0, 0, 0, 0);
    d->editors->addTab(d->composer, "Composer");
    d->editors->addTab(d->editor_splitter, "Python");

    d->tool_bar = this->addToolBar("Main");

    d->open_action = d->tool_bar->addAction(d->font->icon(fa::folderopen), "Open", this, [=] () {
        if (d->editors->currentIndex() == 0)
            d->composer->compositionOpen();
        else
            d->editor->openScript();
    });

    d->run_action = d->tool_bar->addAction(d->font->icon(fa::play), "Run", this, [=] () {
        if (d->editors->currentIndex() == 0) {
            d->composer->composerWidget()->run();
        } else {
            int stat;
            d->interpreter_widget->output(d->interpreter->interpret(d->editor->toPlainText(), &stat));
        }
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->view_manager);
    layout->addWidget(d->editors);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);

    tissueCoreSettings settings;
    settings.beginGroup("main_window");
    this->move(settings.value("position").toPoint());
    this->resize(settings.value("size").toSize());
    settings.endGroup();

    this->setCentralWidget(central);
    this->setWindowTitle("Tissue Growth Simulator");
}

tissueGrowthSimulatorMainWindow::~tissueGrowthSimulatorMainWindow(void)
{
    tissueCoreSettings settings;
    settings.beginGroup("main_window");
    settings.setValue("position", this->pos());
    settings.setValue("size", this->size());
    settings.endGroup();

    delete d;
}

void tissueGrowthSimulatorMainWindow::addEditor(QWidget *editor)
{
    d->editor_splitter->addWidget(editor);
}

//
// tissueGrowthSimulatorMainWindow.cpp ends here
