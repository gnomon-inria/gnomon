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

#include "gnomonWorkspacePreprocess.h"

#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonImagesSerieFilterCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspacePreprocessPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImagesSerieFilterCommand>
{
public:
     gnomonWorkspacePreprocessPrivate(void);
    ~gnomonWorkspacePreprocessPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >()
{
}

gnomonWorkspacePreprocessPrivate::~gnomonWorkspacePreprocessPrivate(void)
{
}

QString gnomonWorkspacePreprocessPrivate::workspace(void) const
{
    return "Preprocess";
}

QStringList gnomonWorkspacePreprocessPrivate::keys(void) const
{
    return gnomonCore::imagesSerieFilter::pluginFactory().keys();
}

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonImagesSerieFilter", &stat);

    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->pane(this));
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::apply(void)
{
    Q_ASSERT(d->command);

    d->command->setImage(d->source->image().data());

    d->command->redo();
    dtkImage *img = d->command->next();

    if (!img) {
        qDebug() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    d->target->setImage(dtkImagePtr(new dtkImage(*img)));
}

void gnomonWorkspacePreprocess::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspacePreprocess.cpp ends here
