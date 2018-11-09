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
    QSlider *slider_1;
    QSlider *slider_2;
    QSlider *slider_3;
    QSlider *slider_4;

public:
    QSpinBox *box;
};

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->slider_1 = new QSlider(this);
    d->slider_1->setOrientation(Qt::Horizontal);
    d->slider_1->setMinimum(1);
    d->slider_1->setMaximum(255);
    d->slider_1->setValue(2);

    d->slider_2 = new QSlider(this);
    d->slider_2->setOrientation(Qt::Horizontal);
    d->slider_2->setMinimum(0);
    d->slider_2->setMaximum(255);
    d->slider_2->setValue(0.5);

    d->slider_3 = new QSlider(this);
    d->slider_3->setOrientation(Qt::Horizontal);
    d->slider_3->setMinimum(1);
    d->slider_3->setMaximum(255);
    d->slider_3->setValue(0.25);

    d->slider_4 = new QSlider(this);
    d->slider_4->setOrientation(Qt::Horizontal);
    d->slider_4->setMinimum(0);
    d->slider_4->setMaximum(10000);
    d->slider_4->setValue(1000);

    d->box = new QSpinBox(this);
    d->box->setMinimum(0);
    d->box->setMaximum(1000);
    d->box->setValue(1);

    QFormLayout *pane_item_params_layout = new QFormLayout;
    pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_params_layout->addRow("h_min", d->slider_1);
    pane_item_params_layout->addRow("Gaussian Sigma", d->slider_2);
    pane_item_params_layout->addRow("Segmentation Gaussian Sigma", d->slider_3);
    pane_item_params_layout->addRow("Volume Threshold", d->slider_4);
    pane_item_params_layout->addRow("Background Label", d->box);

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
    dtkImagePtr source = d->source->image();

    if(!d->segmentation)
        d->segmentation = new gnomonSegmentationCommand("gnomonCellImageFromTimagetkSegmentation");

    d->segmentation->setImage(source);
    d->segmentation->setParameter("h_min", d->slider_1->value());
    d->segmentation->setParameter("gaussian_sigma", d->slider_2->value());
    d->segmentation->setParameter("segmentation_gaussian_sigma", d->slider_3->value());
    d->segmentation->setParameter("volume_threshold", d->slider_4->value());
    d->segmentation->setParameter("background_label", d->box->value());

    d->segmentation->redo();

    d->target->setImage(d->segmentation->computedImage());
}

//
// gnomonWorkspaceSegmentation.cpp ends here
