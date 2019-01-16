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

#include "gnomonCodeEditor.h"
#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"


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
    gnomonViewVolumic *view;

public:
    gnomonOverlayPane *pane;

};

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->editor = new gnomonCodeEditor(this);

    d->view = new gnomonViewVolumic(this);

    d->terminal = new dtkInterpreter(this);

    d->pane = new gnomonOverlayPane(this);


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


    // -- Organizing the viewer column --
    QVBoxLayout *viewer_layout = new QVBoxLayout;
    viewer_layout->setContentsMargins(0, 0, 0, 0);
    viewer_layout->setSpacing(0);
    viewer_layout->addWidget(d->view);
    viewer_layout->addWidget(d->terminal);


    QWidget *viewer = new QWidget(this);
    viewer->setLayout(viewer_layout);


    // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(d->editor);
    splitter->addWidget(viewer);

    QVBoxLayout *layout = new QVBoxLayout(this);
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

}


//
// gnomonWorkspacePythonSimulator.cpp ends here
