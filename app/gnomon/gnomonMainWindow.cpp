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

#include "gnomonFinder.h"
#include "gnomonMainWindow.h"
#include "gnomonViewVolumic.h"
#include "gnomonToolBar.h"

#include <gnomonStyle>

class gnomonMainWindowPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonViewVolumic *view;

public:
    gnomonToolBar *menu;

public:
    QFrame *browse_workspace;
    QFrame *fusion_workspace;
    QFrame *segmtt_workspace;
    QFrame *prepro_workspace;
    QFrame *regist_workspace;

public:
    QStackedWidget *stack;
};

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;

    d->finder = new gnomonFinder(this);

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());
    d->toolbar->setFixedHeight(32);

    d->view = new gnomonViewVolumic(this);

    d->menu = new gnomonToolBar(this);

    QWidget *main = new QWidget(this);

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

    QHBoxLayout *browser_layout = new QHBoxLayout;
    browser_layout->setContentsMargins(0, 0, 0, 0);
    browser_layout->setSpacing(0);
    browser_layout->addWidget(finder);
    browser_layout->addWidget(d->view);

    d->browse_workspace = new QFrame(this);
    d->browse_workspace->setLayout(browser_layout);

    d->fusion_workspace = new QFrame(this);
    d->fusion_workspace->setStyleSheet("background: red;");

    d->segmtt_workspace = new QFrame(this);
    d->segmtt_workspace->setStyleSheet("background: green;");

    d->prepro_workspace = new QFrame(this);
    d->prepro_workspace->setStyleSheet("background: blue;");

    d->regist_workspace = new QFrame(this);
    d->regist_workspace->setStyleSheet("background: cyan;");

    // --

    d->stack = new QStackedWidget(this);
    d->stack->addWidget(d->browse_workspace);
    d->stack->addWidget(d->fusion_workspace);
    d->stack->addWidget(d->segmtt_workspace);
    d->stack->addWidget(d->prepro_workspace);
    d->stack->addWidget(d->regist_workspace);
    d->stack->setCurrentWidget(d->browse_workspace);

    // --

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->stack);
    layout->addWidget(d->menu);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    connect(d->menu, SIGNAL(indexChanged(int)), d->stack, SLOT(setCurrentIndex(int)));

    this->setCentralWidget(central);
    this->setStyleSheet(gnomonStyleSheet());
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    delete d;
}

//
// gnomonMainWindow.cpp ends here
