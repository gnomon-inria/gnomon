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
#include "gnomonWorkspaceBrowser.h"
#include "gnomonViewVolumic.h"

class gnomonWorkspaceBrowserPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonViewVolumic *browse_view;
};

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : QSplitter(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->browse_view = new gnomonViewVolumic(this);

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

    this->addWidget(finder);
    this->addWidget(d->browse_view);
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

//
// gnomonWorkspaceBrowser.cpp ends here
