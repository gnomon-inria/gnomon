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

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspacePreprocessPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageFilterCommand>
{
public:
     gnomonWorkspacePreprocessPrivate(void);
    ~gnomonWorkspacePreprocessPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;
//    gnomonViewVolumic *source = nullptr;
//    gnomonViewVolumic *target = nullptr;
//
public:
    gnomonViewFormPool *pool = nullptr;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonImageFilterCommand >()
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

    dtkScriptInterpreterPython::instance()->interpret("import gnomonImageFilter", &stat);

    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::preprocess_color);
    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::preprocess_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->pane(this));

    connect(d->source, &gnomonViewForm::formAdded, [=] () {
        if(d->command->input() != d->source->image())
            d->command->setInput(d->source->image());
        else
            qDebug() << "Not changed";
        d->configure(this, d->algorithm);
    });

    connect(d, &gnomonWorkspacePreprocessPrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setInput(d->source->image());
        d->configure(this,algorithm);
    });
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->image())
        d->command->setInput(d->source->image());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setImage(d->command->output());
}

void gnomonWorkspacePreprocess::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspacePreprocess.cpp ends here
