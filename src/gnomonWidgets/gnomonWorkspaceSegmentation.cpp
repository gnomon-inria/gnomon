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

#include "gnomonWorkspaceSegmentation.h"

#include "gnomonViewVolumic.h"
#include "gnomonViewVolumicPool.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonActor2DCellImage.h>
#include <gnomonActorMeshCellImage.h>
#include <gnomonCellImage.h>
#include <gnomonSegmentationCommand>

#include <dtkImagingCore>
#include <dtkScript>

#include <QtWidgets>

#include <vtkRenderer.h>

class gnomonWorkspaceSegmentationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonSegmentationCommand>
{

public:
     gnomonWorkspaceSegmentationPrivate(void);
    ~gnomonWorkspaceSegmentationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;

public:
    gnomonViewVolumicPool *pool;

public:
    gnomonCellImage *cellimage = nullptr;

public:
    gnomonActorMeshCellImage *actor = nullptr;
    gnomonActor2DCellImage *actor2D = nullptr;
};

gnomonWorkspaceSegmentationPrivate::gnomonWorkspaceSegmentationPrivate(void) : gnomonWorkspaceTemplatePrivate<gnomonSegmentationCommand>()
{

}

gnomonWorkspaceSegmentationPrivate::~gnomonWorkspaceSegmentationPrivate(void)
{

}

QString gnomonWorkspaceSegmentationPrivate::workspace(void) const
{
    return "Segmentation";
}

QStringList gnomonWorkspaceSegmentationPrivate::keys(void) const
{
    return gnomonCore::cellImageFromImage::pluginFactory().keys();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageFromImage", &stat);

    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->pool = new gnomonViewVolumicPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    QPushButton *cell_button = new QPushButton("Compute cells", this);

    gnomonOverlayPaneItem *visu_item = new gnomonOverlayPaneItem(this);
    visu_item->setTitle("Segmentation");
    visu_item->addWidget(cell_button);
    visu_item->toggle();

    gnomonOverlayPane *pane = d->pane(this);
    pane->addWidget(visu_item);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(cell_button, SIGNAL(clicked()), this, SLOT(computeCells()));
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    delete d;
}

void gnomonWorkspaceSegmentation::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

void gnomonWorkspaceSegmentation::apply(void)
{
    Q_ASSERT(d->command);

    d->command->setImage(d->source->image().data());
    d->command->redo();

    d->target->setImage(dtkImagePtr(new dtkImage(*d->command->computedImage()->image())));
}

void gnomonWorkspaceSegmentation::computeCells(void)
{
    if(!d->actor)
        d->actor = gnomonActorMeshCellImage::New();
    d->actor->setCellImage((gnomonCellImage *)d->command->computedImage()->clone());
    d->target->renderer3D()->AddActor(d->actor);
    
    if(!d->actor2D)
        d->actor2D = gnomonActor2DCellImage::New();
    d->actor2D->setInteractor(d->target->interactor());
    d->actor2D->setCellImage((gnomonCellImage *)d->command->computedImage()->clone());
    d->target->renderer2D()->AddActor(d->actor2D);

    connect(d->target, &gnomonViewVolumic::sliceOrientationChanged, [=] (int value) {
        d->actor2D->setSliceOrientation(value);
    });

    connect(d->target, &gnomonViewVolumic::sliceChanged, [=] (int value) {
        d->actor2D->setSlice(value);
    });

    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
