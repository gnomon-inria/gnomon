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

#include "gnomonWorkspaceLSystemSimulator.h"

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>

#include <dtkScript>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

//TODO

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceLSystemSimulatorPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    dtkMacsWidget *editor = nullptr;

public:
//    gnomonViewForm *view = nullptr;
    gnomonViewMatplotlib *mpl_figure = nullptr;

public:
    gnomonInterpreterJupyter *terminal;

public:
    QVBoxLayout *viewer_layout = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
    
public:
    gnomonAbstractEvolutionModel * model = nullptr;
};

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonEvolutionModel", &stat);

    d = new gnomonWorkspaceLSystemSimulatorPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->editor = new dtkMacsWidget(this);
    d->editor->resize(800, d->editor->height());

//    d->view = new gnomonViewForm(this);
    d->mpl_figure = new gnomonViewMatplotlib(this);

    d->terminal = new gnomonInterpreterJupyter(this);
    //d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    // -- Organizing the viewer column --

    d->viewer_layout = new QVBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    // d->viewer_layout->addWidget(d->view);
    d->viewer_layout->addWidget(d->mpl_figure);
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


    QPushButton *button = new QPushButton("Load", parent);
    button->setCheckable(true);

    QPushButton *button_s = new QPushButton("Step", parent);
    button_s->setCheckable(false);

    QPushButton *button_r = new QPushButton("Reset", parent);
    button_r->setCheckable(false);

    QObject::connect(button, &QPushButton::clicked, [=] ()
    {
        parent->setCursor(Qt::BusyCursor);
        this->apply();
        button_s->setCheckable(true);
        button_r->setCheckable(true);
        parent->setCursor(Qt::ArrowCursor);
    });

    QObject::connect(button_s, &QPushButton::clicked, [=] ()
    {
        parent->setCursor(Qt::BusyCursor);
        this->step();
        parent->setCursor(Qt::ArrowCursor);
    });

    QObject::connect(button_r, &QPushButton::clicked, [=] ()
    {
        parent->setCursor(Qt::BusyCursor);
        this->reset();
        parent->setCursor(Qt::ArrowCursor);
    });

    QVBoxLayout *menu_item_layout = new QVBoxLayout;
    menu_item_layout->addWidget(button);
    menu_item_layout->addWidget(button_s);
    menu_item_layout->addWidget(button_r);

    QWidget *menu_item_widget = new QWidget;
    menu_item_widget->setLayout(menu_item_layout);

    dtkWidgetsMenuItem *menu_item_buttons = new dtkWidgetsMenuItem("Simulation", menu_item_widget);

    d->menu_ = new dtkWidgetsMenu(fa::circlethin, "LSystems");
    d->menu_->addItem(menu_item_buttons);

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

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu_);
    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceLSystemSimulator::~gnomonWorkspaceLSystemSimulator(void)
{
    delete d;
}

void gnomonWorkspaceLSystemSimulator::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->view->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLSystemSimulator::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->view->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLSystemSimulator::apply(void)
{
    // d->model = gnomonCore::evolutionModel::pluginFactory().create("gnomonLStringEvolutionModelLPy");

    // gnomonCoreParameterString *file = ((gnomonCoreParameterString *)d->model->parameters()["lpy_file"]);
    // file->addValue(d->editor->fileName());
    // file->setValue(d->editor->fileName());

    // d->model->reset();

    gnomonLStringSeries *lstring = (gnomonLStringSeries *)d->model->forms()["lstring"];

    if (d->terminal)
        d->terminal->output(((gnomonLString *)lstring->current())->toString());
    else
        qDebug() << Q_FUNC_INFO << ((gnomonLString *)lstring->current())->toString();
    d->mpl_figure->setForm("gnomonLString",lstring);
}

void gnomonWorkspaceLSystemSimulator::step(void)
{
    gnomonLStringSeries *lstring = (gnomonLStringSeries *)d->model->forms()["lstring"];

    double time =  lstring->time();
    d->model->step(time, 1);

    if (d->terminal)
        d->terminal->output(((gnomonLString *)lstring->current())->toString());
    else
        qDebug() << Q_FUNC_INFO << ((gnomonLString *)lstring->current())->toString();
    d->mpl_figure->setForm("gnomonLString",lstring);
}

void gnomonWorkspaceLSystemSimulator::reset(void)
{
    // d->model->reset();

    gnomonLStringSeries *lstring = (gnomonLStringSeries *)d->model->forms()["lstring"];

    if (d->terminal)
        d->terminal->output(((gnomonLString *)lstring->current())->toString());
    else
        qDebug() << Q_FUNC_INFO << ((gnomonLString *)lstring->current())->toString();
    d->mpl_figure->setForm("gnomonLString",lstring);
}

//
// gnomonWorkspaceLSystemSimulator.cpp ends here
