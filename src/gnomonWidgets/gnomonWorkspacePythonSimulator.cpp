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

#include <dtkConfig.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript>
#endif

#include <dtkWidgets>
#include "gnomonWorkspacePythonSimulator.h"

#include <gnomonFonts>

#include "gnomonCodeEditor.h"
#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewForm.h"


class gnomonWorkspacePythonSimulatorPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonCodeEditor *editor;

public:
    dtkInterpreter *terminal;

public:
    gnomonViewForm *view;

public:
    gnomonOverlayPane *pane;

public:
    gnomonFontSourceCodePro *font_source_code_pro;

};

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    d->font_source_code_pro = new gnomonFontSourceCodePro(this);
    d->font_source_code_pro->initFontSourceCodePro();

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    // -- Organizing the finder column --
    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget(d->toolbar);
    toolbar_layout->addWidget(d->path);

    QVBoxLayout *finder_layout = new QVBoxLayout;
    finder_layout->setContentsMargins(0, 0, 0, 0);
    finder_layout->setSpacing(0);
    finder_layout->addLayout(toolbar_layout);
    finder_layout->addWidget(d->finder);

    QWidget *finder = new QWidget(this);
    finder->setLayout(finder_layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));


    d->editor = new gnomonCodeEditor(this);
    d->editor->resize(600, d->editor->height());


    d->view = new gnomonViewForm(this);

    d->terminal = new dtkInterpreter(this);
    d->terminal->setFont(d->font_source_code_pro->font(12));
    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    // -- Organizing the viewer column --
    QVBoxLayout *viewer_layout = new QVBoxLayout;
    viewer_layout->setContentsMargins(0, 0, 0, 0);
    viewer_layout->setSpacing(0);
    viewer_layout->addWidget(d->view);
    viewer_layout->addWidget(d->terminal);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(viewer_layout);


    d->pane = new gnomonOverlayPane(this);

    QPushButton *button = new QPushButton("Apply", parent);
    button->setCheckable(true);

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle("Simulation");
    pane_item_button->addWidget(button);
    pane_item_button->toggle();

    d->pane->addWidget(pane_item_button);

    QObject::connect(button, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->apply();
        parent->setCursor(Qt::ArrowCursor);
    });

    // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(d->editor);
    splitter->addWidget(viewer);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->pane);
}

gnomonWorkspacePythonSimulator::~gnomonWorkspacePythonSimulator(void)
{
    delete d;
}

void gnomonWorkspacePythonSimulator::apply(void)
{
    int stat;
    if (d->terminal)
    {
        d->terminal->output(dtkScriptInterpreterPython::instance()->interpret(d->editor->toPlainText(), &stat));
    }
}


//
// gnomonWorkspacePythonSimulator.cpp ends here
