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
    gnomonViewVolumic *target = nullptr;
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

    d->target  = new gnomonViewVolumic(this);


    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(sources_dummy);
    splitter->addWidget(d->target);

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
               d->command->addImagesSerie(view->imagesSerie().data());
    }
    d->command->redo();

    d->target->setImagesSerie(gnomonImagesSeriePtr(d->command->output()));
}

void gnomonWorkspaceRegistration::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceRegistration.cpp ends here
