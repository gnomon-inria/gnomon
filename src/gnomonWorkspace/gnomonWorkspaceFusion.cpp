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

#include "gnomonWorkspaceFusion.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFusionCommand>
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

class gnomonWorkspaceFusionPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageFusionCommand>
{
public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonGridLayout *sources_layout;

public:
    gnomonViewForm *target;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

QString gnomonWorkspaceFusionPrivate::workspace(void) const
{
    return "Image Fusion";
}

QStringList gnomonWorkspaceFusionPrivate::keys(void) const
{
    return gnomonCore::imageFusion::pluginFactory().keys();
}

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("imageFusion");

    d = new gnomonWorkspaceFusionPrivate;

    d->sources_layout = new gnomonGridLayout;
    d->sources_layout->addView();

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::fusion_color);
    d->target->setMinimumWidth(250);

    QWidget *dummy = new QWidget(this);
    dummy->setLayout(d->sources_layout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(dummy);
    splitter->addWidget(d->target);

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
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(d->algorithm);
//        dtkApp->window()->menubar()->addMenu(d->sources_layout->views().last()->menu());
//        dtkApp->window()->menubar()->touch();
    });

    connect(d, &gnomonWorkspaceFusionPrivate::algorithmChanged, [=] (const QString& algorithm) {
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

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

void gnomonWorkspaceFusion::enter(void)
{
//    foreach(gnomonViewForm *form, d->sources_layout->views())
//        dtkApp->window()->menubar()->addMenu(form->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceFusion::leave(void)
{
//    foreach(gnomonViewForm *form, d->sources_layout->views())
//        dtkApp->window()->menubar()->removeMenu(form->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceFusion::apply(void)
{
    if(d->sources_layout->views().isEmpty())
        return;

    d->command->removeImages();
    //d->command->removeLandmarks();
    d->command->undo();

    for(gnomonViewForm *view : d->sources_layout->views()) {
        if (view->image()) {
            d->command->addImage(view->image());
//        d->command->addLandmarks(view->landmarks());
        }
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setImage(d->command->output());
    }
}

void gnomonWorkspaceFusion::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceFusion.cpp ends here
