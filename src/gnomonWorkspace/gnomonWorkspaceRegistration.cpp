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

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

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
};

QString gnomonWorkspaceRegistrationPrivate::workspace(void) const
{
    return "Time Registration";
}

QStringList gnomonWorkspaceRegistrationPrivate::keys(void) const
{
    return gnomonCore::imageRegistration::pluginFactory().keys();
}

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonImageRegistration", &stat);

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

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    // layout->addWidget(d->pane(this));

    connect(d->sources_layout, &gnomonGridLayout::formAdded, [=] () {
        d->command->undo();
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceRegistrationPrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->undo();
        for(gnomonViewForm *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(algorithm);
    });
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
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
