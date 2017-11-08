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

#include "gnomonGrowthSimulatorMainWindow.h"

#include <dtkConfig.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript>
#endif

#include <dtkComposer>
#include <dtkWidgets>

#include <gnomonCore>
#include <gnomonComposer>
#include <gnomonWidgets>
#include <gnomonFonts>

class gnomonGrowthSimulatorMainWindowPrivate
{
public:
    gnomonComposerWidget *composer;

public:
    gnomonCodeEditor *editor;

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
    gnomonFontAwesome *font_awesome;
    gnomonFontSourceCodePro *font_source_code_pro;
};

gnomonGrowthSimulatorMainWindow::gnomonGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonGrowthSimulatorMainWindowPrivate;

    gnomonCoreSettings settings;
    settings.beginGroup("main_window");
    this->move(settings.value("position").toPoint());
    this->resize(settings.value("size").toSize());
    settings.endGroup();

    d->font_awesome = new gnomonFontAwesome(this);
    d->font_awesome->initFontAwesome();

    d->font_source_code_pro = new gnomonFontSourceCodePro(this);
    d->font_source_code_pro->initFontSourceCodePro();

    d->view_manager = new dtkViewManager(this);

    d->composer = new gnomonComposerWidget(this);

    d->editor = new gnomonCodeEditor;
    d->editor->setFont(d->font_source_code_pro->font(12));

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

    d->open_action = d->tool_bar->addAction(d->font_awesome->icon(fa::folderopen), "Open", this, [=] () {
        if (d->editors->currentIndex() == 0)
            d->composer->compositionOpen();
        else
            d->editor->openScript();
    });

    d->run_action = d->tool_bar->addAction(d->font_awesome->icon(fa::play), "Run", this, [=] () {
        if (d->editors->currentIndex() == 0) {
            d->composer->composerWidget()->run();
        } else {
            int stat;
            if (d->interpreter_widget)
                d->interpreter_widget->output(d->interpreter->interpret(d->editor->toPlainText(), &stat));
            else
                d->interpreter->interpret(d->editor->toPlainText(), &stat);
        }
    });

    d->run_action = d->tool_bar->addAction(d->font_awesome->icon(fa::save), "Save", this, [=] () {
            if (d->editors->currentIndex() == 0) {
                d->composer->compositionSave();
            } else {
                qWarning() << "not implemented";
            }
    });
    d->run_action = d->tool_bar->addAction(d->font_awesome->icon(fa::pencil), "Save As", this, [=] () {
            if (d->editors->currentIndex() == 0) {
                d->composer->compositionSaveAs();
            } else {
                qWarning() << "not implemented";
            }
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->view_manager);
    layout->addWidget(d->editors);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);


    this->setCentralWidget(central);
    this->setWindowTitle("gnomon Growth Simulator");

#if defined(Q_OS_MAC)
    this->initialize();
#endif
}

gnomonGrowthSimulatorMainWindow::~gnomonGrowthSimulatorMainWindow(void)
{
    gnomonCoreSettings settings;
    settings.beginGroup("main_window");
    settings.setValue("position", this->pos());
    settings.setValue("size", this->size());
    settings.endGroup();

    delete d;
}

void gnomonGrowthSimulatorMainWindow::addEditor(QWidget *editor)
{
    d->interpreter_widget->hide();
    d->interpreter_widget->deleteLater();
    d->interpreter_widget = Q_NULLPTR;

     d->editor_splitter->addWidget(editor);
}

//
// gnomonGrowthSimulatorMainWindow.cpp ends here
