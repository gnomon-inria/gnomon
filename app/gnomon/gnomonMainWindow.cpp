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

    QAction *nextTabAction = new QAction("Switch to next workspace", this);
    QAction *prevTabAction = new QAction("Switch to previous workspace", this);

    nextTabAction->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_PageDown);
    prevTabAction->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_PageUp);

    this->addAction(nextTabAction);
    this->addAction(prevTabAction);

    connect(nextTabAction, &QAction::triggered, [=] (void) {
                                                   int count = d->stack->count();
                                                   int index = (d->stack->currentIndex()+1) % count ;
                                                   d->menu->setCurrentIndex(index);
                                               });

    connect(prevTabAction, &QAction::triggered,  [=] (void) {
                                                     int count = d->stack->count();
                                                     int index = (d->stack->currentIndex()+count-1) % count;
                                                     d->menu->setCurrentIndex(index);
                                                 });

    connect(d->menu, SIGNAL(indexChanged(int)), d->stack, SLOT(setCurrentIndex(int)));
    connect(d->menu, &gnomonToolBar::indexDeleted, [=] (int index) {
            QWidget * widget = d->stack->widget(index);
            if (d->stack->currentIndex() == index) {
                d->menu->setCurrentIndex(0);
            }
            if (widget) {
                d->stack->removeWidget(widget);
                delete widget;
            }
        } );

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

    static int l_h = 0;

    connect(d->manager, &gnomonImageManager::expand, [=] (void) {

        int m_h = d->manager->height();
        int s_h = d->stack->height();

        l_h = s_h;

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(d->stack->height());
        animation->setEndValue(0);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->manager->setFixedHeight(m_h + s_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(d->manager, &gnomonImageManager::shrink, [=] (void) {

        int m_h = d->manager->height();

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(0);
        animation->setEndValue(l_h);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->manager->setFixedHeight(m_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);

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
