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

#include "gnomonWorkspaceMeshFromImage.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonToolBar.h"

//#include "gnomonViewVolumic.h"
//#include "gnomonViewVolumicPool.h"

#include "gnomonViewForm.h"
#include "gnomonViewFormPool.h"

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore/gnomonImagesSerie>
#include <gnomonCore/gnomonMeshFromImageCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceMeshFromImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonMeshFromImageCommand>
{
public:
     gnomonWorkspaceMeshFromImagePrivate(void);
    ~gnomonWorkspaceMeshFromImagePrivate(void);

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

gnomonWorkspaceMeshFromImagePrivate::gnomonWorkspaceMeshFromImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonMeshFromImageCommand >()
{

}

gnomonWorkspaceMeshFromImagePrivate::~gnomonWorkspaceMeshFromImagePrivate(void)
{

}

QString gnomonWorkspaceMeshFromImagePrivate::workspace(void) const
{
    return "Surface Meshing";
}

QStringList gnomonWorkspaceMeshFromImagePrivate::keys(void) const
{
    return gnomonCore::meshFromImage::pluginFactory().keys();
}

gnomonWorkspaceMeshFromImage::gnomonWorkspaceMeshFromImage(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonMeshFromImage", &stat);

    d = new gnomonWorkspaceMeshFromImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::meshFromImage_color);
    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::meshFromImage_color);

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

    connect(d, &gnomonWorkspaceMeshFromImagePrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setInput(d->source->imagesSerie());
        d->configure(this,algorithm);
    });
}

gnomonWorkspaceMeshFromImage::~gnomonWorkspaceMeshFromImage(void)
{
    delete d;
}

void gnomonWorkspaceMeshFromImage::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->imagesSerie())
        d->command->setInput(d->source->imagesSerie());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setMesh(d->command->output());
}

void gnomonWorkspaceMeshFromImage::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceMeshFromImage.cpp ends here
