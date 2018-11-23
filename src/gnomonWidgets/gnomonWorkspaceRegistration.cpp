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

#include "gnomonGridLayout.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonImagesRegistrationCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceRegistrationPrivate : public gnomonWorkspaceTemplatePrivate< gnomonImagesRegistrationCommand >
{
public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonGridLayout *sources_layout;
    gnomonGridLayout *targets_layout;
};

QString gnomonWorkspaceRegistrationPrivate::workspace() const
{ return "Registration"; }

QStringList gnomonWorkspaceRegistrationPrivate::keys() const
{
    return gnomonCore::imagesRegistration::pluginFactory().keys();
}

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonImagesRegistration", &stat);

    d = new gnomonWorkspaceRegistrationPrivate;

    d->sources_layout = new gnomonGridLayout;
    d->sources_layout->addView();
    d->sources_layout->addView();
    d->sources_layout->addView();

    QWidget *sources_dummy = new QWidget(this);
    sources_dummy->setLayout(d->sources_layout);

    d->targets_layout = new gnomonGridLayout;

    QWidget *targets_dummy = new QWidget(this);
    targets_dummy->setLayout(d->targets_layout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(sources_dummy);
    splitter->addWidget(targets_dummy);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->pane(this));
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
}

void gnomonWorkspaceRegistration::apply(void)
{
    Q_ASSERT(d->command);

    if(d->sources_layout->views().isEmpty()) return;

    d->command->undo();
    for(gnomonViewVolumic *view : d->sources_layout->views()) {
        d->command->addImage(view->image().data());
    }
    d->command->redo();

    d->targets_layout->removeAllViews();
    for(gnomonViewVolumic *view : d->sources_layout->views()) {
        d->targets_layout->addView();
    }

    for(gnomonViewVolumic *view : d->targets_layout->views()) {
        view->setImage(dtkImagePtr(new dtkImage(*d->command->next())));
    }
}

void gnomonWorkspaceRegistration::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceRegistration.cpp ends here
