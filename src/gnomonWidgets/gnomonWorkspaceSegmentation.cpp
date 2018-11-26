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

#include <gnomonVisualization/gnomonActorMeshCellImage.h>

#include <gnomonCore/gnomonCellImage.h>
#include <gnomonCore/gnomonSegmentationCommand.h>

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
    gnomonViewVolumicPool *pool = nullptr;

public:
    gnomonCellImage *cellimage = nullptr;

public:
    gnomonActorMeshCellImage *actor = nullptr;
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

    connect(cell_button, &QPushButton::clicked, [=]() {
                                                    this->setCursor(Qt::BusyCursor);
                                                    this->computeCells();
                                                    this->setCursor(Qt::BusyCursor);});
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

    d->command->setImagesSerie(d->source->imagesSerie().data());
    d->command->redo();

    // d->target->setImagesSerie(d->command->computedImage()->image()); TODO
}

void gnomonWorkspaceSegmentation::computeCells(void)
{
    if(!d->actor)
        d->actor = gnomonActorMeshCellImage::New();

    d->actor->setCellImage((gnomonCellImage *)d->command->computedImage()->clone());
    d->actor->setInteractor(d->target->interactor());
    d->actor->update();

    d->target->renderer3D()->AddActor(d->actor);
    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
