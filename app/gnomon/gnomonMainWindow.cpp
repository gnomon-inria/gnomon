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

class gnomonMainWindowPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderSideView *side;
    gnomonFinderToolBar *toolbar;
};

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;

    d->finder = new gnomonFinder(this);

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

//    d->side = new gnomonFinderSideView(this);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());
    d->toolbar->setFixedHeight(32);

    QWidget *main = new QWidget(this);

    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget(d->toolbar);
    toolbar_layout->addWidget(d->path);

    QVBoxLayout *main_layout = new QVBoxLayout(main);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addLayout(toolbar_layout);
    main_layout->addWidget(d->finder);

    QWidget *central = new QWidget(this);

    QHBoxLayout *layout= new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
  //  layout->addWidget(d->side);
    layout->addWidget(main);

    this->setCentralWidget(central);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    // connect(d->finder, SIGNAL(changed(QString)), d->side,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    // connect(d->path, SIGNAL(changed(QString)), d->side,    SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    // connect(d->side, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    // connect(d->side, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    // connect(d->side, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    // connect(d->finder, SIGNAL(bookmarked(QString)), d->side, SLOT(addBookmark(QString)));

    connect (d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect (d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect (d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect (d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    delete d;
}

//
// gnomonMainWindow.cpp ends here
