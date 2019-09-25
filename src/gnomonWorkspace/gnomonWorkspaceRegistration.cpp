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

#include "gnomonWorkspaceRegistration.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceRegistrationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageRegistrationCommand>
{
public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonGridLayout *sources_layout;
    gnomonViewForm *target = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

QString gnomonWorkspaceRegistrationPrivate::workspace(void) const
{
    return "Time Registration";
}

QStringList gnomonWorkspaceRegistrationPrivate::keys(void) const
{
    return gnomonCore::imageRegistration::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("gnomonImageRegistration");

    d = new gnomonWorkspaceRegistrationPrivate;

    d->sources_layout = new gnomonGridLayout;
    d->sources_layout->addView();
    d->sources_layout->addView();
    d->sources_layout->addView();

    QWidget *sources_dummy = new QWidget(this);
    sources_dummy->setLayout(d->sources_layout);

    d->target  = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::registration_color);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(sources_dummy);
    splitter->addWidget(d->target);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    dtkWidgetsMenu *menu_1 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 1");
    dtkWidgetsMenuItem *menuitem_11 = menu_1->addItem(fa::circleo, "Cycle through background");
    menu_1->addItem(fa::circleo, "SubLevel 1-2");
    menu_1->addItem(fa::circleo, "SubLevel 1-3");
    menu_1->addSeparator();
    menu_1->addItem(fa::circleo, "SubLevel 1-4");

    dtkWidgetsMenu *menu_2 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 2");
    menu_2->addItem(fa::circleo, "SubLevel 2-1");

    dtkWidgetsMenu *menu_3 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 3");
    menu_3->addItem(fa::circleo, "Sublevel 3-1");
    menu_3->addItem(fa::circleo, "Sublevel 3-2");

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << menu_1 << menu_2 << menu_3);
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

    d->menu_ = d->menu(this);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->sources_layout, &gnomonGridLayout::formAdded, [=] ()
    {
        d->command->undo();
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        dtkApp->window()->menubar()->addMenu(d->sources_layout->views().last()->menu());
        dtkApp->window()->menubar()->touch();
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceRegistrationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->undo();
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
}

void gnomonWorkspaceRegistration::enter(void)
{
    foreach(gnomonViewForm *form, d->sources_layout->views())
        dtkApp->window()->menubar()->addMenu(form->menu());
    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->addMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceRegistration::leave(void)
{
    foreach(gnomonViewForm *form, d->sources_layout->views())
        dtkApp->window()->menubar()->removeMenu(form->menu());
    dtkApp->window()->menubar()->removeMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceRegistration::apply(void)
{
    Q_ASSERT(d->command);

    if(d->sources_layout->views().isEmpty())
        return;

    d->command->undo();

    for(gnomonViewForm *view : d->sources_layout->views())
        d->command->addImage(view->image());

    d->command->redo();

    d->target->setForm("gnomonImage",d->command->output());
}

void gnomonWorkspaceRegistration::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceRegistration.cpp ends here
