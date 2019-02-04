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
#include "gnomonWorkspaceLSystemSimulator.h"

#include "gnomonCodeEditor.h"
#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewForm.h"

#include "gnomonCoreParameter.h"

#include <gnomonCore/gnomonAbstractEvolutionModel>

#include <gnomonCore/gnomonAbstractForm>
#include <gnomonCore/gnomonLString.h>

class gnomonWorkspaceLSystemSimulatorPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonCodeEditor *editor;

public:
    gnomonViewForm *view;
    dtkInterpreter *terminal;
    QVBoxLayout *viewer_layout = nullptr;

public:
    gnomonOverlayPane *pane;

public:
    gnomonAbstractEvolutionModel * model = nullptr;

};

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import gnomonEvolutionModel", &stat);

    d = new gnomonWorkspaceLSystemSimulatorPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->editor = new gnomonCodeEditor(this);
    d->editor->resize(800, d->editor->height());

    d->view = new gnomonViewForm(this);

    d->terminal = new dtkInterpreter(this);
    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    // -- Organizing the viewer column --
    d->viewer_layout = new QVBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    d->viewer_layout->addWidget(d->view);
    d->viewer_layout->addWidget(d->terminal);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(d->viewer_layout);

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
    finder->resize(600, finder->height());


    d->pane = new gnomonOverlayPane(this);

    QPushButton *button = new QPushButton("Load", parent);
    button->setCheckable(true);

    QPushButton *button_s = new QPushButton("Step", parent);
    button_s->setCheckable(false);

    QPushButton *button_r = new QPushButton("Reset", parent);
    button_r->setCheckable(false);

    QObject::connect(button, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->apply();
        button_s->setCheckable(true);
        button_r->setCheckable(true);
        parent->setCursor(Qt::ArrowCursor);
    });

    QObject::connect(button_s, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->step();
        parent->setCursor(Qt::ArrowCursor);
    });

    QObject::connect(button_r, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->reset();
        parent->setCursor(Qt::ArrowCursor);
    });


    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle("Simulation");
    pane_item_button->addWidget(button);
    pane_item_button->addWidget(button_s);
    pane_item_button->addWidget(button_r);
    pane_item_button->toggle();

    d->pane->addWidget(pane_item_button);


    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

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

gnomonWorkspaceLSystemSimulator::~gnomonWorkspaceLSystemSimulator(void)
{
    delete d;
}

void gnomonWorkspaceLSystemSimulator::apply(void)
{
    d->model = gnomonCore::evolutionModel::pluginFactory().create("gnomonLStringEvolutionModelLPy");
    gnomonCoreParameterString * file = ((gnomonCoreParameterString *)d->model->parameters()["lpy_file"]);
    file->addValue(d->editor->fileName());
    file->setValue(d->editor->fileName());

    d->model->reset();
    gnomonLString *lstring = (gnomonLString *) d->model->forms()["lstring"];
    d->terminal->output(lstring->toString());
}

void gnomonWorkspaceLSystemSimulator::step(void)
{
    d->model->step(0,1);
    gnomonLString *lstring = (gnomonLString *) d->model->forms()["lstring"];
    d->terminal->output(lstring->toString());
}

void gnomonWorkspaceLSystemSimulator::reset(void)
{
    d->model->reset();
    gnomonLString *lstring = (gnomonLString *) d->model->forms()["lstring"];
    d->terminal->output(lstring->toString());
}


//
// gnomonWorkspaceLSystemSimulator.cpp ends here
