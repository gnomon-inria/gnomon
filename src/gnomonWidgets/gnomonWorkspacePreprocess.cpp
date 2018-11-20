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
#include "gnomonWorkspaceTemplate.h"

#include <gnomonImagesSerieFilterCommand>

#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>

class gnomonWorkspacePreprocessPrivate : public gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >
{
public:
    gnomonWorkspacePreprocessPrivate();
    virtual ~gnomonWorkspacePreprocessPrivate();

public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate() : gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >()
{
}

gnomonWorkspacePreprocessPrivate::~gnomonWorkspacePreprocessPrivate()
{
}

QString gnomonWorkspacePreprocessPrivate::workspace() const
{ return "Preprocess"; }

QStringList gnomonWorkspacePreprocessPrivate::keys() const
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
