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
#include "gnomonImageManager.h"
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
    gnomonViewVolumic *browse_view;
    gnomonViewVolumic *fusion_view_1;
    gnomonViewVolumic *fusion_view_2;
    gnomonViewVolumic *fusion_view_3;
    gnomonViewVolumic *fusion_view_4;
    gnomonViewVolumic *fusion_view_t;
    gnomonViewVolumic *prepro_view;

public:
    gnomonToolBar *menu;

public:
    QSplitter *browse_workspace;
    QFrame *fusion_workspace;
    QFrame *segmtt_workspace;
    QSplitter *prepro_workspace;
    QFrame *regist_workspace;

public:
    QStackedWidget *stack;

public:
    QComboBox *prepro_box;

public:
    gnomonImageManager *manager;
};

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->browse_view = new gnomonViewVolumic(this);

    d->menu = new gnomonToolBar(this);

    d->manager = gnomonImageManager::instance();

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

    d->browse_workspace = new QSplitter(this);
    d->browse_workspace->addWidget(finder);
    d->browse_workspace->addWidget(d->browse_view);

    d->fusion_view_1 = new gnomonViewVolumic(this);
    d->fusion_view_1->setStyleSheet("gnomonViewVolumic { border: 1px solid red; } QSlider { border: none; }");

    d->fusion_view_2 = new gnomonViewVolumic(this);
    d->fusion_view_2->setStyleSheet("gnomonViewVolumic { border: 1px solid green; } QSlider { border: none; }");

    d->fusion_view_3 = new gnomonViewVolumic(this);
    d->fusion_view_3->setStyleSheet("gnomonViewVolumic { border: 1px solid blue; } QSlider { border: none; }");

    d->fusion_view_4 = new gnomonViewVolumic(this);
    d->fusion_view_4->setStyleSheet("gnomonViewVolumic { border: 1px solid cyan; } QSlider { border: none; }");

    d->fusion_view_t = new gnomonViewVolumic(this);
    d->fusion_view_t->setStyleSheet("gnomonViewVolumic { border: 1px solid yellow; } QSlider { border: none; }");

    QGridLayout *fusion_layout = new QGridLayout;
    fusion_layout->setContentsMargins(0, 0, 0, 0);
    fusion_layout->setSpacing(1);
    fusion_layout->addWidget(d->fusion_view_1, 0, 0);
    fusion_layout->addWidget(d->fusion_view_2, 0, 1);
    fusion_layout->addWidget(d->fusion_view_3, 1, 0);
    fusion_layout->addWidget(d->fusion_view_4, 1, 1);
    fusion_layout->addWidget(d->fusion_view_t, 0, 2, -1, -1);

    d->fusion_workspace = new QFrame(this);
    d->fusion_workspace->setLayout(fusion_layout);

    d->segmtt_workspace = new QFrame(this);
    d->segmtt_workspace->setStyleSheet("background: green;");

    d->prepro_view = new gnomonViewVolumic(this);

    d->prepro_box = new QComboBox(this);
    d->prepro_box->addItem("a");
    d->prepro_box->addItem("b");

    d->prepro_workspace = new QSplitter(this);
    d->prepro_workspace->addWidget(d->prepro_box);
    d->prepro_workspace->addWidget(d->prepro_view);


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
    layout->addWidget(d->manager);
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
