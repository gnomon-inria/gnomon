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

#include "gnomonGridLayout.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonImagesFusionCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceFusionPrivate : public gnomonWorkspaceTemplatePrivate< gnomonImagesFusionCommand >
{
public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonGridLayout *layout;

public:
    gnomonViewVolumic *target;
};

QString gnomonWorkspaceFusionPrivate::workspace() const
{ return "Fusion"; }

QStringList gnomonWorkspaceFusionPrivate::keys() const
{
    return gnomonCore::imagesFusion::pluginFactory().keys();
}

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonImagesFusion", &stat);

    d = new gnomonWorkspaceFusionPrivate;

    d->layout = new gnomonGridLayout;

    d->target = new gnomonViewVolumic(this);
    d->target->setMinimumWidth(250);

    QWidget *dummy = new QWidget(this);
    dummy->setLayout(d->layout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(dummy);
    splitter->addWidget(d->target);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->pane(this));
}

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

void gnomonWorkspaceFusion::apply(void)
{
    if(d->layout->views().isEmpty()) return;

    for(gnomonViewVolumic *view : d->layout->views()) {
        d->command->addImage(view->image().data());
    }
    d->command->redo();
    d->target->setImage(dtkImagePtr(d->command->output()));
}

void gnomonWorkspaceFusion::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceFusion.cpp ends here
