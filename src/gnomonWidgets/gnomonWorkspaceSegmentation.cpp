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
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate.h"

#include <gnomonActor2DCellImage.h>
#include <gnomonActorMeshCellImage.h>
#include <gnomonCellImage.h>
#include <gnomonSegmentationCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <QtWidgets>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

class gnomonWorkspaceSegmentationPrivate : public gnomonWorkspaceTemplatePrivate< gnomonSegmentationCommand >
{

public:
    gnomonWorkspaceSegmentationPrivate();
    virtual ~gnomonWorkspaceSegmentationPrivate();

public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;

    gnomonCellImage *cellimage = nullptr;
    gnomonActorMeshCellImage *actor = nullptr;
    gnomonActor2DCellImage *actor2D = nullptr;
};

gnomonWorkspaceSegmentationPrivate::gnomonWorkspaceSegmentationPrivate() : gnomonWorkspaceTemplatePrivate< gnomonSegmentationCommand >()
{
}

gnomonWorkspaceSegmentationPrivate::~gnomonWorkspaceSegmentationPrivate()
{
}

QString gnomonWorkspaceSegmentationPrivate::workspace() const
{ return "Segmentation"; }

QStringList gnomonWorkspaceSegmentationPrivate::keys() const
{
    return gnomonCore::cellImageFromImage::pluginFactory().keys();
}

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageFromImage", &stat);

    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

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
    d->actor->update();
    d->target->renderer3D()->AddActor(d->actor);
    
    if(!d->actor2D)
        d->actor2D = gnomonActor2DCellImage::New();
    d->actor2D->setCellImage((gnomonCellImage *)d->command->computedImage()->clone());
    d->actor2D->update();
    d->target->renderer2D()->AddActor(d->actor2D);


    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
