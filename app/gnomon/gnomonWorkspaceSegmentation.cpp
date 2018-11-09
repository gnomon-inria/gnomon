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

#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceSegmentation.h"

#include <gnomonCellImage.h>
#include <gnomonSegmentationCommand.h>

#include <dtkImagingCore>

#include <QtWidgets>

class gnomonWorkspaceSegmentationPrivate
{
public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;

public:
    gnomonSegmentationCommand *segmentation = nullptr;

public:
    QDoubleSpinBox *box_h_min;
    QDoubleSpinBox *box_gaussian_sigma;
    QDoubleSpinBox *box_seg_gaussian_sigma;
    QDoubleSpinBox *box_vol_threshold;
    QDoubleSpinBox *box_background_level;
};

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->box_h_min = new QDoubleSpinBox(this);
    d->box_h_min->setMinimum(1.);
    d->box_h_min->setMaximum(255.);
    d->box_h_min->setValue(2.);

    d->box_gaussian_sigma = new QDoubleSpinBox(this);
    d->box_gaussian_sigma->setMinimum(0.);
    d->box_gaussian_sigma->setMaximum(255.);
    d->box_gaussian_sigma->setValue(0.5);

    d->box_seg_gaussian_sigma = new QDoubleSpinBox(this);
    d->box_seg_gaussian_sigma->setMinimum(1.);
    d->box_seg_gaussian_sigma->setMaximum(255.);
    d->box_seg_gaussian_sigma->setValue(0.25);

    d->box_vol_threshold = new QDoubleSpinBox(this);
    d->box_vol_threshold->setMinimum(0.);
    d->box_vol_threshold->setMaximum(10000.);
    d->box_vol_threshold->setValue(1000.);

    d->box_background_level = new QDoubleSpinBox(this);
    d->box_background_level->setMinimum(0.);
    d->box_background_level->setMaximum(1000.);
    d->box_background_level->setValue(1.);

    QFormLayout *pane_item_params_layout = new QFormLayout;
    pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_params_layout->addRow("h_min", d->box_h_min);
    pane_item_params_layout->addRow("Gaussian Sigma", d->box_gaussian_sigma);
    pane_item_params_layout->addRow("Segmentation Gaussian Sigma", d->box_seg_gaussian_sigma);
    pane_item_params_layout->addRow("Volume Threshold", d->box_vol_threshold);
    pane_item_params_layout->addRow("Background Label", d->box_background_level);

    gnomonOverlayPaneItem *pane_item_parameters = new gnomonOverlayPaneItem;
    pane_item_parameters->setTitle("Parameters");
    pane_item_parameters->addLayout(pane_item_params_layout);
    pane_item_parameters->toggle();

    QPushButton *button = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *item = new gnomonOverlayPaneItem(this);
    item->setTitle("Segmentation");
    item->addWidget(button);
    item->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_parameters);
    pane->addWidget(item);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(button, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    if(d->segmentation)
        delete d->segmentation;

    delete d;
}

void gnomonWorkspaceSegmentation::apply(void)
{
    if(!d->segmentation)
        d->segmentation = new gnomonSegmentationCommand("gnomonCellImageFromTimagetkSegmentation");

    d->segmentation->setImage(d->source->image().data());
    d->segmentation->setParameter("h_min", d->box_h_min->value());
    d->segmentation->setParameter("gaussian_sigma", d->box_gaussian_sigma->value());
    d->segmentation->setParameter("segmentation_gaussian_sigma", d->box_seg_gaussian_sigma->value());
    d->segmentation->setParameter("volume_threshold", d->box_vol_threshold->value());
    d->segmentation->setParameter("background_label", d->box_background_level->value());

    d->segmentation->redo();

    d->target->setImage(dtkImagePtr(new dtkImage(*d->segmentation->computedImage())));
}

//
// gnomonWorkspaceSegmentation.cpp ends here
