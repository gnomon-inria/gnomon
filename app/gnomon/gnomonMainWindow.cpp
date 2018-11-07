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

#include <gnomonStyle>

class gnomonMainWindowPrivate
{
public:
    gnomonToolBar *menu;

public:
    QStackedWidget *stack;

public:
    gnomonImageManager *manager;
};

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;

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
        int index = d->stack->currentIndex();
        d->stack->addWidget(new gnomonWorkspaceFusion(this));
        d->stack->setCurrentIndex(index);
    });

    connect(d->menu, &gnomonToolBar::createSegmentation, [=] (void) {
        int index = d->stack->currentIndex();
        d->stack->addWidget(new gnomonWorkspaceSegmentation(this));
        d->stack->setCurrentIndex(index);
    });

    connect(d->menu, &gnomonToolBar::createPreprocess, [=] (void) {
        int index = d->stack->currentIndex();
        d->stack->addWidget(new gnomonWorkspacePreprocess(this));
        d->stack->setCurrentIndex(index);
    });

    connect(d->menu, &gnomonToolBar::createRegistration, [=] (void) {
        int index = d->stack->currentIndex();
        d->stack->addWidget(new gnomonWorkspaceRegistration(this));
        d->stack->setCurrentIndex(index);
    });

    this->setCentralWidget(central);
    this->setStyleSheet(gnomonStyleSheet());
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    delete d;
}

//
// gnomonMainWindow.cpp ends here
