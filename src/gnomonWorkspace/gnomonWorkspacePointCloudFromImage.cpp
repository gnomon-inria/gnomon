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

#include "gnomonWorkspacePointCloudFromImage.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudFromImageCommand>
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

class gnomonWorkspacePointCloudFromImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonPointCloudFromImageCommand>
{
public:
     gnomonWorkspacePointCloudFromImagePrivate(void);
    ~gnomonWorkspacePointCloudFromImagePrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;
};

gnomonWorkspacePointCloudFromImagePrivate::gnomonWorkspacePointCloudFromImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonPointCloudFromImageCommand >()
{

}

gnomonWorkspacePointCloudFromImagePrivate::~gnomonWorkspacePointCloudFromImagePrivate(void)
{

}

QString gnomonWorkspacePointCloudFromImagePrivate::workspace(void) const
{
    return "Nuclei Detection";
}

QStringList gnomonWorkspacePointCloudFromImagePrivate::keys(void) const
{
    return gnomonCore::pointCloudFromImage::pluginFactory().keys();
}

gnomonWorkspacePointCloudFromImage::gnomonWorkspacePointCloudFromImage(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonPointCloudFromImage", &stat);

    d = new gnomonWorkspacePointCloudFromImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::pointCloudFromImage_color);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::pointCloudFromImage_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    // layout->addWidget(d->pane(this));

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->command->input() != d->source->image())
            d->command->setInput(d->source->image());
        else
            qDebug() << "Not changed";

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspacePointCloudFromImagePrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->setInput(d->source->image());
        d->configure(algorithm);
    });
}

gnomonWorkspacePointCloudFromImage::~gnomonWorkspacePointCloudFromImage(void)
{
    delete d;
}

void gnomonWorkspacePointCloudFromImage::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->image())
        d->command->setInput(d->source->image());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setForm("gnomonPointCloud",d->command->output());
}

void gnomonWorkspacePointCloudFromImage::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspacePointCloudFromImage.cpp ends here
