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

#include "gnomonImageManager.h"
#include "gnomonMainWindow.h"
#include "gnomonToolBar.h"
#include "gnomonWorkspaceBrowser.h"
#include "gnomonWorkspaceFusion.h"
#include "gnomonWorkspaceSegmentation.h"
#include "gnomonWorkspacePreprocess.h"
#include "gnomonWorkspaceRegistration.h"
#include "gnomonWorkspaceSimulation.h"

#include <gnomonStyle>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonMainWindowPrivate
{
public:
    void setdw(void);
    void setup(void);

public:
    gnomonToolBar *menu;

public:
    QStackedWidget *stack;

public:
    gnomonImageManager *manager;

public:
    gnomonMainWindow *q;
};

void gnomonMainWindowPrivate::setup(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    q->move(settings.value("position").toPoint());
    q->resize(settings.value("size",QSize(1024,320)).toSize());
}

void gnomonMainWindowPrivate::setdw(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    settings.setValue("position", q->pos());
    settings.setValue("size", q->size());
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;
    d->q = this;

    d->manager = gnomonImageManager::instance();

    d->stack = new QStackedWidget(this);
    d->stack->addWidget(new gnomonWorkspaceBrowser(this));
    d->stack->setCurrentIndex(0);

    d->menu = new gnomonToolBar(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->manager);
    layout->addWidget(d->stack);
    layout->addWidget(d->menu);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);

    connect(d->menu, SIGNAL(indexChanged(int)), d->stack, SLOT(setCurrentIndex(int)));

    connect(d->menu, &gnomonToolBar::createFusion, [=] (void) {

        gnomonWorkspace *workspace = new gnomonWorkspaceFusion(this);
        workspace->enter();

        d->stack->addWidget(workspace);
        d->stack->setCurrentWidget(workspace);
    });

    connect(d->menu, &gnomonToolBar::createSegmentation, [=] (void) {

        gnomonWorkspace *workspace = new gnomonWorkspaceSegmentation(this);
        workspace->enter();

        d->stack->addWidget(workspace);
        d->stack->setCurrentWidget(workspace);
    });

    connect(d->menu, &gnomonToolBar::createPreprocess, [=] (void) {

        gnomonWorkspace *workspace = new gnomonWorkspacePreprocess(this);
        workspace->enter();

        d->stack->addWidget(workspace);
        d->stack->setCurrentWidget(workspace);
    });

    connect(d->menu, &gnomonToolBar::createRegistration, [=] (void) {

        gnomonWorkspace *workspace = new gnomonWorkspaceRegistration(this);
        workspace->enter();

        d->stack->addWidget(workspace);
        d->stack->setCurrentWidget(workspace);
    });

    connect(d->menu, &gnomonToolBar::createSimulation, [=] (void) {

        gnomonWorkspace *workspace = new gnomonWorkspaceSimulation(this);
        workspace->enter();

        d->stack->addWidget(workspace);
        d->stack->setCurrentWidget(workspace);
    });


    this->setCentralWidget(central);
    this->setStyleSheet(gnomonStyleSheet());

    d->setup();
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    d->setdw();

    delete d;
}

//
// gnomonMainWindow.cpp ends here
