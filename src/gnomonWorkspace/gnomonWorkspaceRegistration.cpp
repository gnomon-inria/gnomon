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
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

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
    loadPluginGroup("imageRegistration");

    d = new gnomonWorkspaceRegistrationPrivate;

    d->sources_layout = new gnomonGridLayout;
    d->sources_layout->addView();
//    d->sources_layout->addView();
    d->sources_layout->addView();

    QWidget *sources_dummy = new QWidget(this);
    sources_dummy->setLayout(d->sources_layout);

    d->target  = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::registration_color);

    d->pool = new gnomonViewFormPool(this);
    for(gnomonViewForm *view : d->sources_layout->views()) {
        d->pool->addView(view);
    }
    d->pool->addView(d->target);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Load a Form from the top bar to set the input");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);


    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(sources_dummy);
    splitter->addWidget(d->target_stack);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
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

    connect(d->sources_layout, &gnomonGridLayout::formAdded, [=] ()
    {
        d->command->undo();
    d->target_message->setMessage("Load a Form from the top bar to set the input");
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
                d->target_message->setMessage("Press Apply to display the result of the algorithm");
            }
        }
//        dtkApp->window()->menubar()->addMenu(d->sources_layout->views().last()->menu());
//        dtkApp->window()->menubar()->touch();
        d->configure(d->algorithm);
    });

    connect(d->sources_layout, &gnomonGridLayout::viewAdded, [=] (gnomonViewForm *view)
    {
        d->pool->addView(view);
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
//    foreach(gnomonViewForm *form, d->sources_layout->views())
//        dtkApp->window()->menubar()->addMenu(form->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
//    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceRegistration::leave(void)
{
//    foreach(gnomonViewForm *form, d->sources_layout->views())
//        dtkApp->window()->menubar()->removeMenu(form->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
//    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceRegistration::apply(void)
{
    Q_ASSERT(d->command);

    if(d->sources_layout->views().isEmpty())
        return;

    d->command->undo();

    for(gnomonViewForm *view : d->sources_layout->views())
        if (view->image())
            d->command->addImage(view->image());

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonImage",d->command->output());
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceRegistration::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceRegistration.cpp ends here
