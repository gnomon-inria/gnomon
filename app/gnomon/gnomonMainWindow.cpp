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

class gnomonMainWindowPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonViewVolumic *view;
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

    d->view = new gnomonViewVolumic(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(finder);
    layout->addWidget(d->view);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect (d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect (d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect (d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect (d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    this->setCentralWidget(central);
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    delete d;
}

//
// gnomonMainWindow.cpp ends here
