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
#include <gnomonVisualization>
#include <gnomonWidgets>

#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudFromImageCommand>

#include <dtkImagingCore>
#include <dtkScript>

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
//    gnomonViewVolumic *source = nullptr;
//    gnomonViewVolumic *target = nullptr;
//
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

gnomonWorkspacePointCloudFromImage::gnomonWorkspacePointCloudFromImage(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonPointCloudFromImage", &stat);

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
    layout->addWidget(d->pane(this));

    connect(d->source, &gnomonViewForm::formAdded, [=] () {
        if(d->command->input() != d->source->imagesSerie())
            d->command->setInput(d->source->imagesSerie());
        else
            qDebug() << "Not changed";
        d->configure(this, d->algorithm);
    });

    connect(d, &gnomonWorkspacePointCloudFromImagePrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setInput(d->source->imagesSerie());
        d->configure(this,algorithm);
    });
}

gnomonWorkspacePointCloudFromImage::~gnomonWorkspacePointCloudFromImage(void)
{
    delete d;
}

void gnomonWorkspacePointCloudFromImage::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->imagesSerie())
        d->command->setInput(d->source->imagesSerie());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setPointCloud(d->command->output());
}

void gnomonWorkspacePointCloudFromImage::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspacePointCloudFromImage.cpp ends here
