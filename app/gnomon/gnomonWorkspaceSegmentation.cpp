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

#include <gnomonActorMeshCellImage.h>
#include <gnomonCellImage.h>
#include <gnomonSegmentationCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <QtWidgets>

<<<<<<< HEAD
class gnomonWorkspaceSegmentationPrivate : public gnomonWorkspaceTemplatePrivate< gnomonSegmentationCommand >
=======
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

class gnomonWorkspaceSegmentationPrivate
>>>>>>> develop
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

    gnomonOverlayPane *pane = d->pane(this)
    pane->addWidget(visu_item);
    pane->toggle();

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
    // if(d->command)
    //     delete d->command;

    delete d;
}

void gnomonWorkspaceSegmentation::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

void gnomonWorkspaceSegmentation::apply(void)
{
    Q_ASSERT(d->command);

    // if(!d->command)
    //     d->command = new gnomonSegmentationCommand("gnomonCellImageFromTimagetkSegmentation");

    d->command->setImage(d->source->image().data());
    // d->command->setParameter("h_min", d->box_h_min->value());
    // d->command->setParameter("gaussian_sigma", d->box_gaussian_sigma->value());
    // d->command->setParameter("segmentation_gaussian_sigma", d->box_seg_gaussian_sigma->value());
    // d->command->setParameter("volume_threshold", d->box_vol_threshold->value());
    // d->command->setParameter("background_label", d->box_background_level->value());

    d->command->redo();

    d->target->setImage(dtkImagePtr(new dtkImage(*d->segmentation->computedImage()->image())));
}

void gnomonWorkspaceSegmentation::computeCells(void)
{
    if(!d->actor)
        d->actor = gnomonActorMeshCellImage::New();

    d->actor->setCellImage((gnomonCellImage *)d->segmentation->computedImage()->clone());
    d->actor->setInteractor(d->target->interactor());
    d->actor->update();

    d->target->renderer3D()->AddActor(d->actor);
    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
