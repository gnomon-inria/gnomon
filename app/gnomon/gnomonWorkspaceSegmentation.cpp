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

#include <gnomonCellImage.h>
#include <gnomonSegmentationCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <QtWidgets>

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

// public:
//     gnomonSegmentationCommand *segmentation = nullptr;

// public:
//     QDoubleSpinBox *box_h_min;
//     QDoubleSpinBox *box_gaussian_sigma;
//     QDoubleSpinBox *box_seg_gaussian_sigma;
//     QDoubleSpinBox *box_vol_threshold;
//     QDoubleSpinBox *box_background_level;
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

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->pane(this));

    // connect(button, SIGNAL(clicked()), this, SLOT(apply()));
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

    d->target->setImage(dtkImagePtr(new dtkImage(*d->command->computedImage())));
}

//
// gnomonWorkspaceSegmentation.cpp ends here
