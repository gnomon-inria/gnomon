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

#include <dtkImagingCore>

#include <QtWidgets>

class gnomonWorkspaceSegmentationPrivate
{
public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;

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
    d->slider_2->setValue(0);

    d->slider_3 = new QSlider(this);
    d->slider_3->setOrientation(Qt::Horizontal);
    d->slider_3->setMinimum(1);
    d->slider_3->setMaximum(255);
    d->slider_3->setValue(0);

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
    delete d;
}

void gnomonWorkspaceSegmentation::apply(void)
{
    dtkImagePtr source = d->source->image();

    qDebug() << Q_FUNC_INFO;
}

//
// gnomonWorkspaceSegmentation.cpp ends here
